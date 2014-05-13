// proc.cpp for Solaris (SunOS)
//
// This program is free software. See the file COPYING for details.
// Author: Mattias Engdegård, 1997-1999
//		   José Luis Sánchez, 2005

#include <stdio.h>
#include <sys/types.h>
#include <sys/param.h> //HZ
#include <sys/socket.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sched.h>

#include "qps.h"
#include "proc.h"
#include "uidstr.h"
#include "ttystr.h"
#include "wchan.h"
#include "details.h"


// Solaris...
#include <sys/swap.h>
#include <sys/sysinfo.h>
#include <sys/mkdev.h>
#include <limits.h>

//#include <sys/procfs.h>
#define _STRUCTURED_PROC 1
#include <sys/procfs.h> //	psinfo_t ,
/*
 *
If you want an "#ifdef solaris", the portable way is

 #if defined (__SVR4) && defined (__sun)

This should work on gcc, sun cc, and lots o other compilers, on both sparc and intel.
If for some reason, you want to know that Sun forte CC (c++) compiler is being used, something that seems to work is

#if defined(__SUNPRO_CC)

Whereas for forte cc (regular C), you can use

#if defined(__SUNPRO_C)
 
 */
#include "proc_common.cpp"

char procdir[128] = "/proc";

int page_k_shift; //

extern int		flag_thread_ok;
extern bool		flag_show_thread;	
extern bool		flag_devel;	

kstat_ctl_t *Proc::kc = 0;

int proc_PID_fd(const int pid)
{
	char path[128];
	int numfd;
	
	sprintf(path, "/proc/%d/fd", pid);

	QDir qdir(path);
	numfd = qdir.count();
	
	///num_opened_files += numfd;
	//	  printf("PID %d: %d opened files\n", pid, numfd);
	return TRUE;
}



Procinfo::Procinfo(Proc *system_proc,int process_id,int thread_id) : refcnt(1)
{
	proc=system_proc; // 
	first_run=true;
	clone=false;
	
	if(thread_id<0)
	{
		pid=process_id;
		tgid=process_id;

	}
	else {
		pid=thread_id;
		tgid=process_id;
	}
	

	detail = NULL;	  
 ///   children = 0;

	envblock = 0;
//	  if( readproc(proc_pid) < 0 ) pid = -1;		// invalidate object, will be deleted

	
	ppid=0;   // no parent important!!
	selected = false;
	hidekids = false;
	envblock = 0; //!!

	table_child_seq=-1;
	child_seq_prev=-1;
	
	lastchild=0;
	generation=-1;
	detail = 0;


	size=0;
	resident=0;
	//trs=0;
	//drs=0;
	//stack=0;
	//share=0;
	mem=0;
	
	io_read_prev=0;
	io_write_prev=0;

	//tgid=0;
	pcpu=0;
	pmem=0;

	command="noname";
	tty=0;
	nice=0;
	starttime=0;
	state='Z';
	cutime=utime=0;

	old_utime=0;
}


//COMMON?
Procinfo::~Procinfo()
{
	if( detail )
	{
		detail->process_gone();
		detail = 0;
	}
	
	/*
	if( envblock )
		free(envblock); 
	if( maps )
	{
	 //   maps->purge();
		delete maps;
	}
	if( fd_files )
	{
	   // fd_files->purge();
		delete fd_files;
	} */
 ///   delete children;
}

// miscellaneous static initializations
void Proc::init_static()
{
	if( !kc )
	{
		kc = kstat_open();
		if( !kc )
		{
			perror("kstat_open");
			exit(1);
		}
	}
	
	int pagesize=sysconf(_SC_PAGESIZE); // same getpagesize()  in <unistd.h>
	printf("pagesize=%d, %d\n ",getpagesize(), sysconf(_SC_PAGESIZE)); //4027

	page_k_shift = 0;
	for( int j = getpagesize(); j > 1024; j >>= 1 )
		page_k_shift++;
}

// return number of bytes read if ok, -1 if failed
int read_file(char *name, void *buf, int max)
{
	int fd = open(name, O_RDONLY);
	if( fd < 0 )
		return -1;
	int r = read(fd, buf, max);
	close(fd);
	return r;
}

// SOLARIS 
//	Description : read /proc/1234/task/*  tasks(thread,LWP)  
//			  add to Proc::procs[]	
int Proc::read_pid_tasks(int pid)
{
	char	path[256];
	char	*p;
	struct	dirent *e;
	int		thread_pid;
	int		thread_n=0;

	//	printf("DEBUG:pid=%d :",pid);
	sprintf(path, "%s/%d/lwp",procdir, pid);

	DIR *d = opendir(path);
	if(!d) return -1;


	while((e = readdir(d)) != 0) {
		if(e->d_name[0] == '.')		continue;	// skip "." , ".."

		Procinfo *pi;
		thread_pid = atoi(e->d_name); // only number !!

		if (pid==thread_pid) continue; // skip
		
		/*
			Procinfo *pi = new Procinfo(this,pid, thread_id);

			if(pi->pid == -1)
				delete pi;		// already gone

			else {
				pi->generation = current_gen;
				pi->cmdline="(thread)";
			  //  newproc(pi);
			}
		*/
		pi=procs.value(thread_pid,NULL);

		if(pi==NULL)
		{
			pi = new Procinfo(this,pid,thread_pid);
			procs.insert(thread_pid,pi);
		}
		if(pi->readproc()>=0)
		{
			pi->generation = current_gen;
	//		if(pid!=thread_pid)	pi->cmdline="(thread)";


		}

		thread_n++;
	}
	///printf("\n");
	closedir(d);				
	return thread_n;
}

//DEL ( because of 2byte language)
static inline bool isprintable(unsigned char c)
{
	// assume, somewhat na.vely, that all latin-1 characters are printable
	return (c >= 0x20 && c < 0x7f) || c >= 0xa0;
}

//DEL replace unprintables by spaces
static void make_printable(char *s)
{
	while(*s) {
		if(!isprintable(*s)) *s = ' ';
		++s;
	}
}

// what this fuck..??? move to readproc
/* void Proc::newproc(Procinfo *p)
{
	Procinfo *oldp = procs[p->pid];

	//if(oldp && flag_thread_ok && (previous_flag_show_thread != flag_show_thread) ){
	//	  oldp->deref();
	//	  oldp =NULL;
   // }
	{
		// New process
		p->wcpu = p->pcpu;	// just a start
	} 
} */


// read proc/PID Solaris 10, not tested in Solaris 9
int Procinfo::readproc()
{
	char path[256];

//	Procinfo:pid;
	proc_PID_fd(pid);	// check!!
	sprintf(path, "%s/%d/psinfo", procdir, pid);
	
	psinfo_t psi;
	if( read_file(path, (void *)&psi, sizeof(psi)) < (int)sizeof(psi) )
		return -1;

	sprintf(path, "%s/%d/usage", procdir, pid);
	prusage_t pru;
	if( read_file(path, (void *)&pru, sizeof(pru)) < (int)sizeof(pru) )
		return -1;
	
	if(first_run)
	{
		
		first_run=false;
	}
	minflt = pru.pr_minf;
	majflt = pru.pr_majf;

	uid =	psi.pr_uid; //ok
	euid =	psi.pr_euid; //ok
	gid =	psi.pr_gid;		//ok
	egid =	psi.pr_egid;	//ok

//	  make_printable(psi.pr_psargs);
	cmdline =	psi.pr_psargs;	  //ok
	command =	psi.pr_fname;	
   
	ppid =		psi.pr_ppid; // ok
	pgrp =		psi.pr_pgid; // ok
	session =	psi.pr_sid;   // ok
	tty =		psi.pr_ttydev;	// ok type?
   
	nthreads =	psi.pr_nlwp; // num of threads


	//------- no checked 
	const int ns_ticks = 1000000000 /HZ; // HZ=100, clk_tick
	//printf("ns_ticks=%d\n",ns_ticks);// 10000000 
//	  starttime = (psi.pr_start.tv_sec) * HZ   + psi.pr_start.tv_nsec / ns_ticks;
	starttime = psi.pr_start.tv_sec; // discard tv_nsec
	env_ofs =	psi.pr_envp;
	addr_bits = psi.pr_dmodel == PR_MODEL_ILP32 ? 32 : 64;

	gettimeofday(&tv, 0); // current_time, sys/time , tv.tv_sec, tv.tv_usec
 
	// god dam!!!! if that dont reel in¿ the ladies then nothing will!!!! 
 
	// No
	state = psi.pr_lwp.pr_sname; // no 
	command = (state == 'Z') ? "<zombie>" : psi.pr_fname;
	flags =		psi.pr_flag;	// no
   
	// No 
	utime = psi.pr_time.tv_sec * HZ + psi.pr_time.tv_nsec / ns_ticks; //??
	cutime = psi.pr_ctime.tv_sec * HZ + psi.pr_ctime.tv_nsec / ns_ticks;
   
	
//	printf("[%d] utime=%d\n",pid,utime);

	int dcpu;
	if(old_utime==0 and utime==0)
	{
		pcpu=0;

	}
	else
	{
		dcpu = utime - old_utime;

		// calculate pcpu (and wcpu for Linux) from previous procinfo
		int dt = (tv.tv_usec - old_tv.tv_usec) / (1000000 / HZ) 
			+ (tv.tv_sec - old_tv.tv_sec) * HZ;

		if(dt==0) pcpu=0;
		else pcpu = 100.0 * dcpu / dt;

		//	if( pcpu > 99.99 )	pcpu = 99.99;

		old_utime=utime;
		old_tv=tv;
	}

	priority = psi.pr_lwp.pr_pri;
	nice = psi.pr_lwp.pr_nice;
	if( Qps::normalize_nice ) //???
		nice -= NZERO;
	
	wchan = psi.pr_lwp.pr_wchan;

	//MEM
	size = psi.pr_size;
	resident = psi.pr_rssize; 
	which_cpu = psi.pr_lwp.pr_onpro;

	// pr_pctcpu and pr_pctmem are scaled so that 1.0 is stored as 0x8000.
	// We rescale pcpu so a CPU-bound process is shown as 100%. (This means
	// that wcpu may exceed 100% with several LWPs.)
	wcpu = psi.pr_pctcpu * (1 / 327.68) * proc->num_cpus;
	pmem = psi.pr_pctmem * (1 / 327.68);

	rtprio = -1;		// ditto
	policy_name[0] = psi.pr_lwp.pr_clname[0];
	policy_name[1] = psi.pr_lwp.pr_clname[1];

	if (dcpu==0) return 1;
	return 2; // this process consumed jiffi of cpu
}

// thread	Solaris 10 
int Procinfo::readproc(int proc_pid, int thread)
{
	char ppath[256];
	char upath[256];

	pid = proc_pid;

	proc_PID_fd(pid);
	
	if (!flag_thread_ok || !flag_show_thread) 
	{
		return readproc();
	}
	// get the process stuff
	//readproc();

	pid = thread; //*****
	
	sprintf(ppath, "%s/%d/lwp/%d/lwpsinfo", procdir, proc_pid, thread);
	sprintf(upath, "%s/%d/lwp/%d/lwpusage", procdir, proc_pid, thread);

	lwpsinfo_t psi;
	if( read_file(ppath, (void *)&psi, sizeof(psi)) < (int)sizeof(psi) ) {
		return -1;
	}

	prusage_t pru;
	if( read_file(upath, (void *)&pru, sizeof(pru)) < (int)sizeof(pru) ) {
		return -1;
	}

	state = psi.pr_sname;
	flags = psi.pr_flag;
	const int ns_ticks = 1000000000 / HZ;
	utime = psi.pr_time.tv_sec * HZ + psi.pr_time.tv_nsec / ns_ticks;
	priority = psi.pr_pri;
	nice = psi.pr_nice;
	if( Qps::normalize_nice )
		nice -= NZERO;
	starttime = (psi.pr_start.tv_sec - proc->boot_time) * HZ
		+ psi.pr_start.tv_nsec / ns_ticks;
	wchan = psi.pr_wchan;

	minflt = pru.pr_minf;
	majflt = pru.pr_majf;
	which_cpu = psi.pr_onpro;

	// pr_pctcpu and pr_pctmem are scaled so that 1.0 is stored as 0x8000.
	// We rescale pcpu so a CPU-bound process is shown as 100%. (This means
	// that wcpu may exceed 100% with several LWPs.)
	if (psi.pr_pctcpu) {
		wcpu = (psi.pr_pctcpu  / 327.68 ) * proc->num_cpus;
	} else {
		wcpu = 0.0;
	}
	gettimeofday(&tv, 0); // 
	rtprio = -1;		// ditto
	policy_name[0] = psi.pr_clname[0];
	policy_name[1] = psi.pr_clname[1];
	///num_process++;
	return pid;
	// thread Solaris10
}


static float getscaled(kstat_t *ks, const char *name)
{
	// load avgs are scaled by 256
	kstat_named_t *kn = (kstat_named_t *)kstat_data_lookup(ks, (char *)name);
	return kn ? kn->value.ui32 * (1 / 256.0) : 0.0;
}

void Proc::read_loadavg()
{
	kstat_chain_update(kc);

	kstat_t *ks = kstat_lookup(kc, (char *)"unix", 0, (char *)"system_misc");
	if( !ks || kstat_read(kc, ks, 0) == -1 )
	{
		perror("kstat_lookup/read");
		exit(1);
	}

	loadavg[0] = getscaled(ks, "avenrun_1min");
	loadavg[1] = getscaled(ks, "avenrun_5min");
	loadavg[2] = getscaled(ks, "avenrun_15min");

	// we might as well get the boot time too since it's in the same kstat
	// (not that it is going to change)
	kstat_named_t *kn;
	kn = (kstat_named_t *)kstat_data_lookup(ks, (char *)"boot_time");
	if( kn )
		boot_time = kn->value.ui32;
}

// read /proc
// called by Proc::fresh();
int Proc::read_system()
{
	static bool first_run=true;
	if(first_run)
	{
		flag_thread_ok = true;
	
		// memory info: this is easy - just use sysconf
		mem_total = sysconf(_SC_PHYS_PAGES) << page_k_shift;
		mem_free = sysconf(_SC_AVPHYS_PAGES) << page_k_shift;
	 
		// Max SMP 1024 cpus . COMMON
		int max_cpus=1024;
		// cpu_times_vec = (unsigned *)malloc(sizeof(unsigned) *  num_cpus * CPUTIMES);
		cpu_times_vec = new unsigned[CPUTIMES * max_cpus];
		old_cpu_times_vec = new unsigned[CPUTIMES * max_cpus];
		
		//init
		for(int cpu = 0; cpu < max_cpus ; cpu++)
			for(int i = 0; i < CPUTIMES; i++)
			{	
				cpu_times(cpu, i)=0;
				old_cpu_times(cpu, i) =0;
			}

/*
	if(cpu_times_vec)
	{
		if(old_cpu_times_vec)  free(old_cpu_times_vec);
		old_cpu_times_vec = cpu_times_vec;
		cpu_times_vec = (unsigned *)malloc(sizeof(unsigned) *  num_cpus * CPUTIMES);
	}
		old_num_cpus = num_cpus;
 */	
		first_run=0;
	}

	 // get swap info: somewhat trickier - we have to count all swap spaces
	int nswaps = swapctl(SC_GETNSWP, 0);
	swaptbl_t *st = (swaptbl_t *)malloc(sizeof(int)
			+ nswaps * sizeof(swapent_t));
	st->swt_n = nswaps;
	// We are not interested in the paths, just the values, so we allocate
	// one scratch buffer for all paths to keep swapctl happy.
	char path_buf[PATH_MAX + 1];

	for(int i = 0; i < nswaps; i++)
		st->swt_ent[i].ste_path = path_buf;
	swapctl(SC_LIST, st);

	// count the swap spaces
	swap_total = swap_free = 0;
	for(int i = 0; i < nswaps; i++)
	{
		swap_total += st->swt_ent[i].ste_pages;
		swap_free += st->swt_ent[i].ste_free;
	}
	swap_total <<= page_k_shift;
	swap_free <<= page_k_shift;
	free(st);

	 
	// cpu states: are stored as kstats named "cpu_statN", where N is the
	// cpu number. Unfortunately, the cpu numbers are not guessable so we
	// sweep the kstat chain for all of them, assuming (foolishly?)
	// that they are in order.

	//kstat_chain_update(kc);
	int cpu = 0;

	for( kstat_t *ks = kc->kc_chain; ks; ks = ks->ks_next )
	{
	   // printf("kstat name :%s\n",ks->ks_name);

		if( strncmp(ks->ks_name, "cpu_stat", 8) == 0 )
		{
			if( kstat_read(kc, ks, NULL) == -1 )
			{
				perror("kstat_read");
				exit(1);
			}
			cpu_stat_t *cs = (cpu_stat_t *)ks->ks_data;
			cpu_times(cpu, CPUTIME_USER) = cs->cpu_sysinfo.cpu[CPU_USER];
			cpu_times(cpu, CPUTIME_SYSTEM) = cs->cpu_sysinfo.cpu[CPU_KERNEL];
			cpu_times(cpu, CPUTIME_WAIT) = cs->cpu_sysinfo.cpu[CPU_WAIT];
			cpu_times(cpu, CPUTIME_IDLE) = cs->cpu_sysinfo.cpu[CPU_IDLE];
			
			cpu++;
		}
	}
	Proc::num_cpus=cpu;
	
	// exit(1);
	dt_total=0;
	dt_used=0;
	
	// dt_used= user + system;
	// dt_total= user + system + nice + idle
	// dt_used+=cpu_times(cpu, CPUTIME_USER) + cpu_times(cpu, CPUTIME_SYSTEM);
	// dt_total=dt_used
			
	cpu_times(num_cpus, CPUTIME_USER)=0;
	cpu_times(num_cpus, CPUTIME_SYSTEM)=0;
	cpu_times(num_cpus, CPUTIME_WAIT)=0;
	cpu_times(num_cpus, CPUTIME_IDLE)=0;
	for(int cpu = 0; cpu < num_cpus ; cpu++) {
			cpu_times(num_cpus, CPUTIME_USER)+= cpu_times(cpu, CPUTIME_USER); 
			cpu_times(num_cpus, CPUTIME_SYSTEM)+= cpu_times(cpu, CPUTIME_SYSTEM); 
			cpu_times(num_cpus, CPUTIME_WAIT)+= cpu_times(cpu, CPUTIME_WAIT); 
			cpu_times(num_cpus, CPUTIME_IDLE)+= cpu_times(cpu, CPUTIME_IDLE); 
	}
	
	cpu=num_cpus;
	dt_used+=cpu_times(cpu, CPUTIME_USER) + cpu_times(cpu, CPUTIME_SYSTEM); // Kernel
	dt_total+= cpu_times(cpu, CPUTIME_USER) + cpu_times(cpu, CPUTIME_SYSTEM) +
				cpu_times(cpu, CPUTIME_WAIT) +
				cpu_times(cpu, CPUTIME_IDLE);
	
	load_cpu=(float)Proc::dt_used/Proc::dt_total;  
		
	// Hotplugging Detection : save total_cpu 
	if(Proc::num_cpus != Proc::old_num_cpus) {
		//	for(int i = 0; i < CPUTIMES; i++)
		//		cpu_times(num_cpus, i) = cpu_times(Proc::old_num_cpus, i);
		//		Proc::old_num_cpus=Proc::num_cpus;
	}
}

int Procinfo::get_policy()
{
	if( policy == -1 )
		policy = sched_getscheduler(pid);
	return policy;
}

int Procinfo::get_rtprio()
{
	if(rtprio == -1)
	{
		struct sched_param p;
		if( sched_getparam(pid, &p) == 0 )
			rtprio = p.sched_priority;
	}
	return rtprio;
}

// called by bool Procinfo::read_fds()
void Procinfo::read_fd(int fdnum, char *path)
{
	struct stat sb;

	if( lstat(path, &sb) < 0 )
	{
		// The file has been closed, or we could really not stat it despite
		// having it open (could be a fd passed from another process).
		fd_files.append(new Fileinfo(fdnum, "(no info available)"));
		return;
	}
	// We could in principle find out more about the fd, such as its mode
	// (RDONLY, RDWR etc) and flags, but it's messy. pfiles uses an agent lwp
	// for this, but I don't know how to do it.
	QString s;
	const char *n;
	switch( sb.st_mode & S_IFMT )
	{
		case S_IFCHR:
			// if it is a tty, we might know its real name
			if( sb.st_rdev != (dev_t)-1 )
			{
				QString t = Ttystr::name(sb.st_rdev);
				if( t[0] != '?' )
				{
					s = "/dev/";
					s.append(t);
					break;
				}
			}
			s.sprintf("char device %u:%u",
					(unsigned)major(sb.st_rdev), (unsigned)minor(sb.st_rdev));
			break;

		case S_IFBLK:
			s.sprintf("block device %u:%u",
					(unsigned)major(sb.st_rdev), (unsigned)minor(sb.st_rdev));
			break;

		case S_IFLNK:
			// Directories appear as symlinks in /proc/#/fd; we chdir() to it
			// and see where we end up. Not efficient though.
			// Besides, we change cwd a lot in unpredictable ways. This makes
			// core dumps hard to find, if they are generated at all.
			s = "directory ";
			if( chdir(path) >= 0 )
			{
				char buf[512];
				if( getcwd(buf, sizeof(buf)) >= 0 )
				{
					s.append(buf);
					break;
				}
			}
			s.append("(unknown)");
			break;

		default:
			switch( sb.st_mode & S_IFMT )
			{
				case S_IFIFO:		// fifo or anonymous pipe
					n = "pipe";
					break;
				case S_IFDIR:		// this shouldn't happen
					n = "directory";
					break;
				case S_IFREG:
					n = "file";
					break;
				case S_IFSOCK:
					n = "unix domain socket";
					break;
				case S_IFDOOR:
					n = "door";
					break;
				default:
					n = "unknown";
					break;
			}
			s.sprintf("%s, dev %u:%u inode %u", n, (unsigned)major(sb.st_dev),
					(unsigned)minor(sb.st_dev), (unsigned)sb.st_ino);
			break;
	}
	fd_files.append( new Fileinfo(fdnum, s) );
}

// return TRUE if /proc/PID/fd could be read, FALSE otherwise
// store fileinfo, and also socket inodes separately
bool Procinfo::read_fds()
{
	char path[80], *p;

	sprintf(path, "%s/%d/fd", procdir, pid);

	DIR *d = opendir(path);
	if( !d )
		return FALSE;

	p = path + strlen(path) + 1;
	p[-1] = '/';

	struct dirent *e;
	while( (e = readdir(d)) != 0 )
	{
		if( e->d_name[0] == '.' )
			continue;		// skip . and ..
		strcpy(p, e->d_name);
		int fdnum = atoi(p);
		read_fd(fdnum, path);
	}
	closedir(d);
	return TRUE;
}

// return TRUE if the process environment could be read, FALSE otherwise
bool Procinfo::read_environ()
{
	int fd;
	char file[128];
	
	return 0; 

	sprintf(file, "/proc/%d/as", pid);
	if( (fd = open(file, O_RDONLY)) < 0 )
		return FALSE;

	// Just read the first 8K from the environment. Adaptive code here is
	// possible, but not really worth the effort.
	int bs = 8192;
	if( envblock )
		free(envblock);
	envblock = (char *)malloc(bs);
	if( pread(fd, envblock, bs, env_ofs) < 0 )
	{
		free(envblock);
		envblock = 0;
		return FALSE;
	}
	close(fd);
	envblock[bs - 1] = '\0';

///    environ.purge();

	for( int i = 0; i * (int)sizeof(char *) < bs && ((char **)envblock)[i];
			i++ )
	{
		int b = ((char **)envblock)[i] - (char *)env_ofs;
		if( b < 0 || b >= bs )
			continue;			// outside retrieved memory block
		char *val = strchr(envblock + b, '=');
		if( val )
			*val++ = '\0';
		else
			val = (char *)"";	// degenerate: treat as name with empty value
		make_printable(envblock + b);
		make_printable(val);
		environ.append( NameValue(envblock + b, val) ); // ?????
	}
	return TRUE;
}

// Try using /proc/bin/pmap to add file names to the memory map
void Procinfo::read_pmap_maps()
{
	char buf[256];

	sprintf(buf, "/usr/proc/bin/pmap %d 2>/dev/null", pid);

	FILE *pmap = popen(buf, "r");
	if( !pmap )
		return;

	// skip first line
	if( !fgets(buf, sizeof buf, pmap) )
	{
		pclose(pmap);
		return;
	}

	int map_num = 0;
	while( fgets(buf, sizeof buf, pmap) )
	{
		// Each output line from pmap looks like
		// <address> <size>K <mode> <file>
		// We use <address> and <size> only to match with previously read
		// map info, and only use <file> here.

		unsigned long addr;
		unsigned len;
		int next;
		char *p;

		if( sscanf(buf, "%lx %dK%n", &addr, &len, &next) != 2 )
			continue;

		// Correlate this with info already gathered. Assume they are in the
		// same order (ascending by address).
		Mapsinfo *mi;
		int i = map_num;
		while( i < maps.size() && (mi = maps[i])->from != addr )
			i++;
		// Mismatches can happen since changes can have taken place since
		// we read the maps. If so, skip this mapping and try the next.
		if( mi->to != addr + ((unsigned long)len << 10) || i == maps.size() )
			continue;
		map_num = i + 1;

		while( buf[next] == ' ' )
			next++;
		while( buf[next] && buf[next] != ' ' )
			next++;
		while( buf[next] == ' ' )
			next++;
		// At this point  we are looking at a file name, or at a suitable
		// designator like [ heap ], [ anon ] or [ stack ]. Use it right away
		// (after peeling off the newline)
		int l = strlen(buf + next);
		if( buf[next + l - 1] == '\n' )
			buf[next + l - 1] = '\0';
		mi->filename = buf + next;
	}

	pclose(pmap);
	return;
}

// return TRUE if /proc/XX/map could be read, FALSE otherwise
bool Procinfo::read_maps()
{
	char name[128];
	sprintf(name, "%s/%d/map", procdir, pid);

	FILE *f = fopen(name, "r");
	if( !f )
		return FALSE;

  
	prmap_t pm;
	while( fread(&pm, sizeof(pm), 1, f) == 1 )
	{
		Mapsinfo *mi = new Mapsinfo;
		mi->from = pm.pr_vaddr;
		mi->to = pm.pr_vaddr + pm.pr_size;
		mi->offset = pm.pr_offset;
		mi->perm[0] = pm.pr_mflags & MA_READ   ? 'r' : '-';
		mi->perm[1] = pm.pr_mflags & MA_WRITE  ? 'w' : '-';
		mi->perm[2] = pm.pr_mflags & MA_EXEC   ? 'x' : '-';
		mi->perm[3] = pm.pr_mflags & MA_SHARED ? 's' : 'p';

		if( pm.pr_mapname[0] )
		{
			// To find device/inode, stat the file in /proc/#/object:
			char obj[128];
			sprintf(obj, "%s/%d/object/%s", procdir, pid, pm.pr_mapname);
			struct stat sb;
			if( lstat(obj, &sb) < 0 )
			{
				delete mi;
				continue;
			}
			mi->major = major(sb.st_dev);
			mi->minor = minor(sb.st_dev);
			mi->inode = sb.st_ino;
			if( strcmp(pm.pr_mapname, "a.out") == 0 )
				mi->filename = "(executable)";
		}
		else
		{
			mi->major = mi->minor = mi->inode = 0;
			mi->filename = "(anonymous)";
		}

		maps.append(mi);
	}
	fclose(f);

	// If desired and possible, use /usr/proc/bin/pmap to get the
	// names of the mapped files
	static int myeuid = geteuid();
	if( Qps::use_pmap && (myeuid == 0 || myeuid == euid) )
		read_pmap_maps();

	return TRUE;
}


Cat_dir::Cat_dir(const char *heading, const char *explain, const char *dirname,
		QString Procinfo::*member)
: Cat_string(heading, explain),
	dir(dirname),
	cache(member)
{}

QString Cat_dir::string(Procinfo *p)
{
	if( (p->*cache).isNull() )
	{
		char path[128], buf[512];
		sprintf(path, "/proc/%d/%s", p->pid, dir);

		// Either a Linux 2.0 link in [device]:inode form, or a Solaris link.
		// To resolve it, we just chdir() to it and see where we end up.
		// Perhaps we should change back later?
		if( chdir(path) < 0 )
		{
			p->*cache = "-";	// Most likely access denied
		}
		else
		{
			// getcwd() is fairly expensive, but this is cached anyway
			if( !getcwd(buf, sizeof(buf)) )
			{
				p->*cache = "(deleted)";
			}
			else 
				p->*cache = buf;
		}
	}
	return p->*cache;
}

	Cat_state::Cat_state(const char *heading, const char *explain)
: Category(heading, explain)
{}

QString Cat_state::string(Procinfo *p)
{
	QString s("   ");
	s[0] = p->state;
	if( p->state == 'Z' )
		return s;
	s[1] = (p->resident == 0 && p->state != 'Z') ? 'W' : ' ';
	int ni = p->nice;
	if( !Qps::normalize_nice )
		ni -= NZERO;
	s[2] = (ni > 0) ? 'N' : ((ni < 0) ? '<' : ' ');
	return s;
}

	Cat_policy::Cat_policy(const char *heading, const char *explain)
: Category(heading, explain)
{}

QString Cat_policy::string(Procinfo *p)
{
	QString s;
	s = "  ";
	s[0] = p->policy_name[0];
	s[1] = p->policy_name[1];
	return s;
}

int Cat_policy::compare(Procinfo *a, Procinfo *b)
{
	int r = b->policy_name[0] - a->policy_name[0];
	return r ? r : b->policy_name[1] - a->policy_name[1];
}

	Cat_rtprio::Cat_rtprio(const char *heading, const char *explain)
: Category(heading, explain)
{}

QString Cat_rtprio::string(Procinfo *p)
{
	QString s;
	s.setNum(p->get_rtprio());
	return s;
}

int Cat_rtprio::compare(Procinfo *a, Procinfo *b)
{
	return b->get_rtprio() - a->get_rtprio();
}

	Cat_time::Cat_time(const char *heading, const char *explain)
: Category(heading, explain)
{}

QString Cat_time::string(Procinfo *p)
{
	QString s;
	int ticks = p->utime;
	if( Procview::flag_cumulative )
		ticks += p->cutime;
	int t = ticks / HZ;		// seconds
	if( t < 10 )
	{
		int hundreds = ticks / (HZ / 100) % 100;
		s.sprintf("%1d.%02ds", t, hundreds);
	}
	else
		if(t < 100 * 60)
		{
			s.sprintf("%2d:%02d", t / 60, t % 60);
		}
		else
			if( t < 100 * 3600 )
			{
				int h = t / 3600;
				t %= 3600;
				s.sprintf("%2d:%02dh", h, t / 60);
			}
			else
			{
				int d = t / 86400;
				t %= 86400;
				s.sprintf("%dd%dh", d, t / 3600);
			}
	return s;
}

int Cat_time::compare(Procinfo *a, Procinfo *b)
{
	int at = a->utime, bt = b->utime;
	if( Procview::flag_cumulative )
	{
		at += a->cutime;
		bt += b->cutime;
	}
	return bt - at;
}

Cat_tty::Cat_tty(const char *heading, const char *explain)
: Cat_string(heading, explain)
{}

QString Cat_tty::string(Procinfo *p)
{
	return Ttystr::name(p->tty);
}

Proc::Proc()
{
	
	categories.insert(F_PID,  new Cat_int("PID", "Process ID", 6, &Procinfo::pid));
	categories.insert(F_PPID, new Cat_int("PPID", "Parent process ID", 6, &Procinfo::ppid));
	categories.insert(F_PGID, new Cat_int("PGID", "Process group ID", 6, &Procinfo::pgrp));
	categories.insert(F_SID,  new Cat_int("SID", "Session ID", 6, &Procinfo::session));
	categories.insert(F_TTY,  new Cat_tty("TTY", "Controlling tty"));
	categories.insert(F_USER, new Cat_user("USER", "Owner (*=suid root, +=suid other user)"));
	categories.insert(F_GROUP, new Cat_group("GROUP", "Group name (*=sgid other)"));
	categories.insert(F_UID,  new Cat_int("UID", "Real user ID", 6, &Procinfo::uid));
	categories.insert(F_EUID, new Cat_int("EUID", "Effective user ID", 6,&Procinfo::euid));
	categories.insert(F_GID,  new Cat_int("GID", "Real group ID", 6, &Procinfo::gid));
	categories.insert(F_EGID, new Cat_int("EGID", "Effective group ID", 6,&Procinfo::egid));
	categories.insert(F_PRI,  new Cat_int("PRI", "Dynamic priority", 4, &Procinfo::priority));
	categories.insert(F_NICE, new Cat_int("NICE", "Scheduling favour (higher -> less cpu time)", 4, &Procinfo::nice));
	categories.insert(F_PLCY, new Cat_policy("PLCY", "Scheduling policy"));
	categories.insert(F_RPRI, new Cat_rtprio("RPRI", "Realtime priority (0-99, more is better)"));
	categories.insert(F_NLWP, new Cat_int("NLWP", "Number of threads in process", 5, &Procinfo::nthreads));
	categories.insert(F_ARCH, new Cat_int("ARCH", "Architecture (address bits)", 2, &Procinfo::addr_bits));
	categories.insert(F_MAJFLT, new Cat_uintl("MAJFLT", "Number of major faults (disk access)", 8, &Procinfo::majflt));
	categories.insert(F_MINFLT, new Cat_uintl("MINFLT", "Number of minor faults (no disk access)", 8, &Procinfo::minflt));
	
	//Memory
	categories.insert(F_SIZE, new Cat_memory("SIZE",   "Virtual image size of process in Kbytes", 8, &Procinfo::size));
	categories.insert(F_SWAP, new Cat_swap("SWAP",	  "Kbytes on swap device"));  
	categories.insert(F_RSS,  new Cat_memory("RSS",		"Resident set size; Kbytes of program in memory", 8, &Procinfo::resident));


	categories.insert(F_STAT, new Cat_state("STAT",   "State of the process"));
	categories.insert(F_FLAGS, new Cat_hex("FLAGS",   "Process flags (hex)", 9, &Procinfo::flags));
	categories.insert(F_WCHAN, new Cat_wchan("WCHAN",  "Kernel function where process is sleeping"));

	categories.insert(F_WCPU, new Cat_percent("%WCPU","Weighted percentage of CPU (30 s average)",6, &Procinfo::wcpu));
	categories.insert(F_CPU,  new Cat_percent(" %CPU","Percentage of CPU used since last update", 6, &Procinfo::pcpu));
	categories.insert(F_MEM,  new Cat_percent(" %MEM", "Percentage of memory used (RSS/total mem)", 6, &Procinfo::pmem));
	categories.insert(F_START, new Cat_start("START", "Time process started"));
	categories.insert(F_TIME, new Cat_time("TIME",	  "Total CPU time used since start"));
	categories.insert(F_CPUNUM, new Cat_int("CPU", "CPU the process is executing on", 3, &Procinfo::which_cpu));
	categories.insert(F_CMD, new Cat_string("COMMAND","Command that started the process",  &Procinfo::command));
	categories.insert(F_CWD,  new Cat_dir("CWD", "Current working directory", "cwd", &Procinfo::cwd));
	categories.insert(F_ROOT, new Cat_dir("ROOT", "Root directory of process", "root", &Procinfo::root));
	categories.insert(F_CMDLINE, new Cat_cmdline("COMMAND_LINE","Command line that started the process"));
	
	commonPostInit();

	Proc::init_static(); 
}


// update the process list
// called by void Proc::refresh()
void Proc::read_proc_all()
{

	DIR *d = opendir(procdir);
	struct dirent *e;
	while( (e = readdir(d)) != 0 )
	{
		if( e->d_name[0] >= '0' && e->d_name[0] <= '9') // good idea!
		{
			
		/* if(flag_thread_ok && flag_show_thread)
				read_pid_tasks(pid);

			*/
		
			Procinfo *pi;
			int pid;
			
			pid=atoi(e->d_name);	
			
			pi=procs.value(pid,NULL);	// if not found pid then, return Null 
			
			if (pi==NULL)	// new process
			{
				pi = new Procinfo(this,pid);	 
				procs.insert(pid,pi);
			}
			int ret=pi->readproc();
			if(ret>0)
			{
				pi->generation = current_gen; // this process is alive
				
				//if(flag_show_thread and flag_thread_ok ) 
				//	read_pid_tasks(pid);   //for threads 
			
				// add to History /// COMMONZ
				if(ret==2)
				{
					Procinfo *p=new Procinfo(*pi); // copy 
					p->clone=true;
					hprocs->insert(pid,p);
				}
			}
			else
			{
				// already gone.  /proc/PID dead! 
				// later remove this process ! not yet 
			}
		}		 
	}
	closedir(d);

}

int Procview::custom_fields[] = {F_PID, F_TTY, F_USER, F_NICE,
	F_NLWP,
	F_SIZE, F_RSS,
	F_STAT, F_CPU, F_START, F_TIME,
	F_CMDLINE, F_END};

int Procview::basic_fields[] = {F_PID, F_TTY, F_USER, F_NICE,
	F_NLWP,
	F_SIZE, F_RSS,
	F_STAT, F_CPU, F_START, F_TIME,
	F_CMDLINE, F_END};
int Procview::jobs_fields[] = {F_PID, F_PPID, F_PGID, F_SID, 
	F_TTY,
	F_STAT, F_UID, F_TIME, F_CMDLINE, F_END};

int Procview::mem_fields[] = {F_PID, 
	F_TTY, F_MAJFLT, F_MINFLT,
	F_SIZE, F_SWAP, F_RSS,
	F_CMDLINE, 
	F_END};


void Procview::set_fields()
{
	switch(viewfields)
	{
		case USER:
			set_fields_list(basic_fields);
			break;
		case JOBS:
			set_fields_list(jobs_fields);
			break;
		case MEM:
			set_fields_list(mem_fields);
			break;
		case CUSTOM:
			set_fields_list(custom_fields);
			break;
	}
}


//SOLARIS:
//deduce whether the currently selected fields correspond to a field list
void Procview::deduce_fields()
{
	return;
/*
	if( viewfields != CUSTOM )
		return;

	Procview::fieldstates tags[3] = {USER, JOBS, MEM};
	int *lists[3] = {user_fields, jobs_fields, mem_fields};
	for( int i = 0; i < 3; i++ )
	{
		int *l = lists[i];
		int j;
		for( j = 0; l[j] != F_END; j++ )
			if( findCol(l[j]) < 0 )
				break;
		if( l[j] == F_END && j == cats.size() )
		{
			viewfields = tags[i];
			return;
		}
	} */
}

void check_system_requirement()
{}

bool Procinfo::isThread()
{
	return pid!=tgid; // not a thread !
}

Proc::~Proc()
{
}


