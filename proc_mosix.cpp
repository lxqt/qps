// proc.C
//
// This program is free software. See the file COPYING for details.
// Author: Mattias Engdegård, 1997-1999

#include <stdio.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sched.h>

#include "qps.h"
#include "proc.h"
#include "svec.cpp"
#include "uidstr.h"
#include "ttystr.h"
#include "wchan.h"
#include "details.h"

#ifdef SOLARIS
#include <sys/swap.h>
#include <sys/sysinfo.h>
#include <sys/mkdev.h>
#include <limits.h>
#endif

#include "proc_common.cpp" 


// socket states, from <linux/net.h> and touched to avoid name collisions
enum {
  SSFREE = 0,			/* not allocated		*/
  SSUNCONNECTED,		/* unconnected to any socket	*/
  SSCONNECTING,			/* in process of connecting	*/
  SSCONNECTED,			/* connected to socket		*/
  SSDISCONNECTING		/* in process of disconnecting	*/
};

char procdir[128] = "/proc";

int kernel_version=0;
int Procinfo::page_k_shift;

Procinfo::Procinfo(int proc_pid)
    : refcnt(1)
{
    details = 0;
    children = 0;
    fd_files = 0;
    maps = 0;

    environ = 0;
    envblock = 0;
    if(readproc(proc_pid) < 0)
	pid = -1;		// invalidate object, will be deleted

    selected = FALSE;
    hidekids = FALSE;
}

Procinfo::~Procinfo()
{
    if(details) {
	details->process_gone();
	details = 0;
    }
    delete environ;
    if(envblock)
	free(envblock);
    if(maps) {
	maps->purge();
	delete maps;
    }
    if(fd_files) {
	fd_files->purge();
	delete fd_files;
    }
    delete children;
}

// miscellaneous static initializations
void Procinfo::init_static()
{

    page_k_shift = 0;
    for(int j = getpagesize(); j > 1024; j >>= 1)
	page_k_shift++;
}

// return number of bytes read if ok, -1 if failed
int Procinfo::read_file(char *name, void *buf, int max)
{
    int fd = open(name, O_RDONLY);
    if(fd < 0) return -1;
    int r = read(fd, buf, max);
    close(fd);
    return r;
}

static inline bool isprintable(unsigned char c)
{
    // assume, somewhat naïvely, that all latin-1 characters are printable
    return (c >= 0x20 && c < 0x7f) || c >= 0xa0;
}

// replace unprintables by spaces
static void make_printable(char *s)
{
    while(*s) {
       if(!isprintable(*s)) *s = ' ';
       ++s;
    }
}

#ifdef LINUX

//  read task and ADD to Proc::procs[] !!
//   eg) /proc/1234/task/*
int Proc::readTaskByPID(int pid)
{
	char path[256];
	struct dirent *e;
    	char *p;
	int TID;
	int thread_n=0;
	
	printf("pid=%d :",pid);
	sprintf(path, "/proc/%d/task", pid);

	DIR *d = opendir(path);
	if(!d) return FALSE;

/*
	if(!fd_files)
		fd_files = new Svec<Fileinfo*>(8);
	fd_files->clear();

#ifdef LINUX
	if(!sock_inodes)
		sock_inodes = new Svec<SockInode>(4);
	sock_inodes->clear();
#endif
*/
//	p = path + strlen(path) + 1;??
//	p[-1] = '/';
	
	while((e = readdir(d)) != 0) {
		if(e->d_name[0] == '.') 	continue;	// skip . and ..

		TID = atoi(e->d_name); // only numbre !!
		printf("tid=%d ",TID);

		if(pid!=TID)
		{
			sprintf(procdir,"/proc/%d/task",pid,TID);
			Procinfo *pi = new Procinfo(TID);
		
			if(pi->pid == -1){
				printf("--");
				delete pi;		// already gone
			}
			else {
				pi->generation = current_gen;
				newproc(pi);
				printf("**");
			}

		}
		
		thread_n++;
//		read_fd(fdnum, path);
	}
	printf("\n");
	closedir(d);				

	return thread_n;
}
	
int Procinfo::readproc(int proc_pid)
{
    char path[256];
    char buf[256];
    char sbuf[4096];		// should be enough to acommodate /proc/X/stat
    char cmdbuf[MAX_CMD_LEN];
    QString tmp_str;
    pid = proc_pid;


    // sprintf(path, "%s/%d", "/proc", pid);
    sprintf(path, "%s/%d", procdir, proc_pid);
    // read /proc/XX/cmdline
    strcpy(buf, path);
    strcat(buf, "/cmdline");
    int cmdlen;
    if((cmdlen = read_file(buf, cmdbuf, MAX_CMD_LEN - 1)) < 0)
	    return -1;
    if(cmdlen == 0) {
	cmdline = "";
    } else {
	for(int i = 0; i < cmdlen; i++)
	    if(!cmdbuf[i]) cmdbuf[i] = ' ';
	int j = cmdlen - 1;
	while(j >= 0 && cmdbuf[j] == ' ')
	    j--;
	cmdbuf[j + 1] = '\0';
	make_printable(cmdbuf);
	tmp_str = cmdbuf;
	cmdline = UniString(tmp_str); // for Non-ascii locale language
    }
    
    // read /proc/XX/stat
    strcpy(buf, path);
    strcat(buf, "/stat");
    int statlen;
    if((statlen = read_file(buf, sbuf, sizeof(sbuf) - 1)) <= 0) 
	    return -1;
    sbuf[statlen] = '\0';
    char *p = strrchr(sbuf, ')');
    *p = '\0';			// split in two parts
    command = strchr(sbuf, '(') + 1;
    
    //
    // Not all values from /proc/#/stat are interesting; the ones left out
    // have been retained in comments to see where they should go, in case
    // they are needed again.
    //
    // In Linux 2.2.x, timeout has been removed, and the signal information
    // here is obsolete (/proc/#/status has real-time signal info).
    //
    // There are undocumented values after wchan, unused so far:
    // nswap		pages swapped out since process started
    // cnswap		nswap of children
    // exit_signal	(2.2.x) signal sent to parent when process exits
    // (The latter could provide a way to detect cloned processes since
    //  they usually have exit_signal != SIGCHLD, but I prefer waiting
    //  for real TIDs before implementing thread support.)
    //
    long stime, cstime;
    int i_tty;
    sscanf(p + 2, "%c %d %d %d %d %d %lu %lu %lu %lu %lu "
	   "%ld %ld %ld %ld %d %d %*s %*s %lu %*s %*s %*s %*s %*s %*s %*s %*s "
	   "%*s %*s %*s %*s %lu",
	   &state, &ppid, &pgrp, &session, &i_tty, &tpgid,
	   &flags, &minflt, &cminflt, &majflt, &cmajflt,
	   &utime, &stime, &cutime, &cstime, &priority, &nice,
	   /* timeout, itrealvalue */
	   &starttime,
	   /* vsize */
	   /* rss */
	   /* rlim, startcode, endcode, startstack kstkesp kstkeip,
	      signal, blocked, sigignore, sigcatch */
	   &wchan);
    
    tty = (dev_t)i_tty;
    utime += stime;		// we make no user/system time distinction 
    cutime += cstime;
    
    // read /proc/XX/statm
    strcpy(buf, path);
    strcat(buf, "/statm");
    if((statlen = read_file(buf, sbuf, sizeof(sbuf) - 1)) <= 0) 
    {
	    printf("33");
	    return -1;
    }
    
    sbuf[statlen] = '\0';
    sscanf(sbuf, "%lu %lu %lu %lu %lu %lu %lu",
	   &size, &resident, &share, &trs, &lrs, &drs, &dt);
    size <<= page_k_shift;
    resident <<= page_k_shift;
    share <<= page_k_shift;
    trs <<= page_k_shift;
    lrs <<= page_k_shift;
    drs <<= page_k_shift;
    
    pmem = 100.0 * resident / mem_total;
    
#ifdef MOSIX
    if(mosix_running) {
	    // Read /proc/XX/where
	    strcpy(buf, path);
	    strcat(buf, "/where");
	    if((statlen = read_file(buf, sbuf, sizeof(sbuf) - 1)) <= 0) 
		    where = -1;
	    else { 
		    sbuf[statlen] = '\0';
		    sscanf(sbuf, "%d", &where);
	    }
	    // Read /proc/XX/cantmove
	    strcpy(buf, path);
	    strcat(buf, "/cantmove");
	    if((statlen = read_file(buf, sbuf, sizeof(sbuf) - 1)) > 0) {
		    sbuf[statlen] = '\0';
		    p = strchr(sbuf, '\n');
		    if(p) {
			    *p = '\0';
			    cantmove = sbuf;
		    }
	    }
	    // Read /proc/XX/nmigs
	    strcpy(buf, path);
	    strcat(buf, "/nmigs");
	    if((statlen = read_file(buf, sbuf, sizeof(sbuf) - 1)) <= 0) 
		    nmigs = -1; 
	    else {
		    sbuf[statlen] = '\0';
		    sscanf(sbuf, "%d", &nmigs);
	    }
	    // Read /proc/XX/lock
	    strcpy(buf, path);
	    strcat(buf, "/lock");
	    if((statlen = read_file(buf, sbuf, sizeof(sbuf) - 1)) <= 0)
		    locked = -1;
	    else {
		    sbuf[statlen] = '\0';
		    sscanf(sbuf, "%d", &locked);
	}

	// See if this is a remote job
	// Read /proc/mosix/remote/xx/statm
	char path2[256];
	sprintf(path2, "%s/mosix/remote/%d", procdir, pid);
	// There are only two files to read: stats and from
	// (statm is identical to /proc/xx/statm).
	strcpy(buf, path2);
	strcat(buf, "/stats");
	if((statlen = read_file(buf, sbuf, sizeof(sbuf) - 1)) > 0) {
	    isremote = TRUE;	// This process is a visitor

	    // The following variables are available, but are not used at the
	    // moment, so they are merely placeholders right now.
	    char state2;
	    long nswap, cnswap, rss, vsize;
	    if(sscanf(sbuf, "utime=%ld cutime=%ld nice=%d state=%c vsize=%ld"
		      " rss=%ld nswap=%ld cnswap=%ld",
		      &utime, &cutime, &nice, &state2, &vsize, &rss,
		      &nswap, &cnswap) != 8)
		return -1;

	    // read "from"
	    strcpy(buf, path2);
	    strcat(buf, "/from");
	    if((statlen = read_file(buf, sbuf, sizeof(sbuf) - 1)) <= 0)
		return -1;
	    sbuf[statlen] = '\0';
	    if(sscanf(sbuf, "%d", &from) != 1) return -1;

	    // command is parsed wrong (contains "remote(xx)")
	    sscanf(command, "remote(%d)", &remotepid);
	    command = cmdline; // grab comm from there...
	} else {
		isremote = FALSE;
		remotepid = -1;
		from = -1;
	}
	if(from > 0)
		sprintf(buf, "%d>", from);
	else if(where > 0) 
		sprintf(buf, ">%d", where);
	else
		strcpy(buf, "-");
	migr = buf;
    }
#endif // MOSIX

    // read /proc/XX/status
    strcpy(buf, path);  // /proc/$PID
    strcat(buf, "/status");
    if((statlen = read_file(buf, sbuf, sizeof(sbuf) - 1)) <= 0) return -1;
    sbuf[statlen] = '\0';
    if(!(p = strstr(sbuf, "Uid:")))
	    return -1;
    sscanf(p, "Uid: %d %d %d %d Gid: %d %d %d %d",
	   &uid, &euid, &suid, &fsuid,
	   &gid, &egid, &sgid, &fsgid);

    which_cpu = 0;
    per_cpu_times = 0;

    // only works kernel 2.4.x 
    if(num_cpus > 1 ) {
	per_cpu_times = new unsigned long[num_cpus];
	if(per_cpu_times == NULL)
		return -1;
	if(kernel_version < 20600) { // less than version 2.6.0  and SMP
	    strcpy(buf, path); // /proc/$PID
	    strcat(buf, "/cpu"); // /proc/$PID/cpu
	    if( (statlen = read_file(buf, sbuf, sizeof(sbuf) - 1)) <= 0)
	       return -1;
            sbuf[statlen] = '\0';
            p = sbuf;
	    for(unsigned cpu = 0; cpu < num_cpus; cpu++) {
	        p = strchr(p, '\n');
	        if (!p) {
		    for(cpu = 0; cpu < num_cpus; cpu++)
		       per_cpu_times[cpu] = 0;
		    break;
	        }
	        p++;
	        unsigned long utime, stime;
	        sscanf(p, "%*s %lu %lu", &utime, &stime);
	        per_cpu_times[cpu] = utime + stime;
	   }
        }
    }

    gettimeofday(&tv, 0);
    policy = -1;		// will get it when needed
    rtprio = -1;		// ditto
  
    return pid;
}

#endif // LINUX

#ifdef SOLARIS
int Procinfo::readproc(int proc_pid)
{
    char path[256];

    pid = proc_pid;

    sprintf(path, "%s/%d/psinfo", procdir, proc_pid);
    psinfo_t psi;
    if(read_file(path, (void *)&psi, sizeof(psi)) < (int)sizeof(psi))
	return -1;

    sprintf(path, "%s/%d/usage", procdir, proc_pid);
    prusage_t pru;
    if(read_file(path, (void *)&pru, sizeof(pru)) < (int)sizeof(pru))
	return -1;

    uid = psi.pr_uid;
    euid = psi.pr_euid;
    gid = psi.pr_gid;
    egid = psi.pr_egid;

    make_printable(psi.pr_psargs);
    cmdline = psi.pr_psargs;

    state = psi.pr_lwp.pr_sname;
    command = (state == 'Z') ? "<zombie>" : psi.pr_fname;
    ppid = psi.pr_ppid;
    pgrp = psi.pr_pgid;
    session = psi.pr_sid;
    tty = psi.pr_ttydev;	// type?
    flags = psi.pr_flag;
    const int ns_ticks = 1000000000 / HZ;
    utime = psi.pr_time.tv_sec * HZ + psi.pr_time.tv_nsec / ns_ticks;
    cutime = psi.pr_ctime.tv_sec * HZ + psi.pr_ctime.tv_nsec / ns_ticks;
    priority = psi.pr_lwp.pr_pri;
    nice = psi.pr_lwp.pr_nice;
    if(Qps::normalize_nice)
	nice -= NZERO;
    starttime = (psi.pr_start.tv_sec - boot_time) * HZ
	        + psi.pr_start.tv_nsec / ns_ticks;
    wchan = psi.pr_lwp.pr_wchan;
    minflt = pru.pr_minf;
    majflt = pru.pr_majf;
    size = psi.pr_size;
    resident = psi.pr_rssize;
    nthreads = psi.pr_nlwp;
    addr_bits = psi.pr_dmodel == PR_MODEL_ILP32 ? 32 : 64;
    which_cpu = psi.pr_lwp.pr_onpro;
    env_ofs = psi.pr_envp;

    // pr_pctcpu and pr_pctmem are scaled so that 1.0 is stored as 0x8000.
    // We rescale pcpu so a CPU-bound process is shown as 100%. (This means
    // that wcpu may exceed 100% with several LWPs.)
    wcpu = psi.pr_pctcpu * (1 / 327.68) * num_cpus;
    pmem = psi.pr_pctmem * (1 / 327.68);

    gettimeofday(&tv, 0);
    rtprio = -1;		// ditto
    policy_name[0] = psi.pr_lwp.pr_clname[0];
    policy_name[1] = psi.pr_lwp.pr_clname[1];

    return pid;
}
#endif // SOLARIS

float Procinfo::loadavg[] = {0.0, 0.0, 0.0};
int Procinfo::mem_total = 0;
int Procinfo::mem_free = 0;
#ifdef LINUX
//int Procinfo::mem_shared = 0; // only linux kernel 2.4.x
int Procinfo::mem_buffers = 0;
int Procinfo::mem_cached = 0;
#endif
int Procinfo::swap_total = 0;
int Procinfo::swap_free = 0;
unsigned *Procinfo::cpu_times_vec = 0;
unsigned *Procinfo::old_cpu_times_vec = 0;
long Procinfo::boot_time = 0;
#ifdef LINUX
Q3IntDict<Sockinfo> Procinfo::socks(17);
bool Procinfo::socks_current = FALSE;
Q3IntDict<UnixSocket> Procinfo::usocks(17);
bool Procinfo::usocks_current = FALSE;
#endif
unsigned int Procinfo::num_cpus = 0;
unsigned int Procinfo::old_num_cpus = 0;
#ifdef SOLARIS
kstat_ctl_t *Procinfo::kc = 0;
#endif
#ifdef MOSIX
bool Procinfo::mosix_running;
#endif

#ifdef LINUX
// just grab the load averages
void Procinfo::read_loadavg()
{
    char path[80];
    char buf[512];
    strcpy(path, procdir);
    strcat(path, "/loadavg");
    int n;
    if((n = read_file(path, buf, sizeof(buf) - 1)) <= 0) {
	fprintf(stderr,	"qps: Cannot open /proc/loadavg"
		        " (make sure /proc is mounted)\n");
	exit(1);
    }
    buf[n] = '\0';
    sscanf(buf, "%f %f %f", &loadavg[0], &loadavg[1], &loadavg[2]);
}
#endif // LINUX

#ifdef SOLARIS
static float getscaled(kstat_t *ks, const char *name)
{
    // load avgs are scaled by 256
    kstat_named_t *kn = (kstat_named_t *)kstat_data_lookup(ks, (char *)name);
    return kn ? kn->value.ui32 * (1 / 256.0) : 0.0;
}

void Procinfo::read_loadavg()
{
    kstat_chain_update(kc);

    kstat_t *ks = kstat_lookup(kc, (char *)"unix", 0, (char *)"system_misc");
    if(!ks || kstat_read(kc, ks, 0) == -1) {
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
    if(kn)
	boot_time = kn->value.ui32;
}
#endif // SOLARIS

#ifdef LINUX
// read information common to all processes
// !!! should be simplified !!
void Procinfo::read_common()
{
	char path[80];
	char buf[4096 + 1];

	char *p;
	int n;

	int i1,i2,i3,i4;
	//read kernel version
	strcpy(path, "/proc/version");
	if((n = read_file(path, buf, sizeof(buf) - 1)) <= 0) return;
	buf[n] = '\0';
	sscanf(buf, "Linux version %d.%d.%d", &i1,&i2,&i3);
	kernel_version=i1*10000+i2*100+i3*1;
	// printf("DEBUG: kernel_version =%d \n",kernel_version);

	// read memory info
	strcpy(path, procdir);
	strcat(path, "/meminfo");
	if((n = read_file(path, buf, sizeof(buf) - 1)) <= 0) return;
	buf[n] = '\0';

	// Skip the old /meminfo cruft, making this work in post-2.1.42 kernels
	// as well.  (values are now in kB)
	p = strstr(buf, "MemTotal:");
	sscanf(p, "MemTotal: %d kB\nMemFree: %d kB\b\nBuffers: %d kB\nCached: %d kB\n",
			&mem_total, &mem_free,  &mem_buffers, &mem_cached);
	p = strstr(buf, "SwapTotal:");
	sscanf(p, "SwapTotal: %d kB\nSwapFree: %d kB\n",
			&swap_total, &swap_free);

	// read system status
	strcpy(path, procdir);
	strcat(path, "/stat"); // /proc/stat
	if((n = read_file(path, buf, sizeof(buf) - 1)) <= 0) return;
	buf[n] = '\0';

	old_num_cpus = num_cpus;
	if(!num_cpus) {
		// count cpus
#ifdef FAKE_SMP
		num_cpus = 4;
#else
		char *p;
		p = strstr(buf, "cpu");
		while(p < buf + sizeof(buf) - 4 && strncmp(p, "cpu", 3) == 0) {
			num_cpus++;
			if(strncmp(p, "cpu0", 4) == 0)
				num_cpus--;
			p = strchr(p, '\n');
			if(p)
				p++;
		}
#endif
		cpu_times_vec = new unsigned[CPUTIMES * num_cpus];
		old_cpu_times_vec = new unsigned[CPUTIMES * num_cpus];
	}

	for(unsigned cpu = 0; cpu < num_cpus; cpu++)
		for(int i = 0; i < CPUTIMES; i++)
			old_cpu_times(cpu, i) = cpu_times(cpu, i);

	if(num_cpus == 1) {
		unsigned iowait, irq, sftirq;
		sscanf(buf, "cpu %u %u %u %u %u %u %u",
				&cpu_times(0, CPUTIME_USER), &cpu_times(0, CPUTIME_NICE),
				&cpu_times(0, CPUTIME_SYSTEM), &cpu_times(0, CPUTIME_IDLE),
				&iowait, &irq, &sftirq);
		cpu_times(0, CPUTIME_SYSTEM)+= (irq+sftirq);
		cpu_times(0, CPUTIME_IDLE)+=iowait;
	} else {
#ifdef FAKE_SMP
		sscanf(buf, "cpu %u %u %u %u",
				&cpu_times(0, CPUTIME_USER), &cpu_times(0, CPUTIME_NICE),
				&cpu_times(0, CPUTIME_SYSTEM), &cpu_times(0, CPUTIME_IDLE));
		for(unsigned cpu = 1; cpu < num_cpus; cpu++) {
			for(int i = 0; i < CPUTIMES; i++)
				cpu_times(cpu, i) = cpu_times(0, i);
		}
#else
		// SMP
		for(unsigned cpu = 0; cpu < num_cpus; cpu++) {
			char cpu_buf[10];
			sprintf(cpu_buf, "cpu%d", cpu);
			if((p = strstr(buf, cpu_buf)) != 0) {
				sscanf(p, "%*s %u %u %u %u",
						&cpu_times(cpu, CPUTIME_USER), 
						&cpu_times(cpu, CPUTIME_NICE),
						&cpu_times(cpu, CPUTIME_SYSTEM), 
						&cpu_times(cpu, CPUTIME_IDLE));
			} else {
				fprintf(stderr, "Error reading info for cpu %d\n", cpu);
				abort();
			}
		}
#endif
	}

	// 2.0.x kernels (at least up to 2.0.33) have an SMP bug that reports
	// cpu_time(CPUTIME_IDLE) incorrectly, since it doesn't take the number of
	// cpus into account. This is fixed in 2.1.x kernels, and since 2.0.x
	// is rather old (and unsuited for SMP anyway) we don't work around it.

	p = strstr(buf, "btime") + 6;
	sscanf(p, "%lu", &boot_time);
}
#endif // LINUX

#ifdef SOLARIS
void Procinfo::read_common()
{
    // memory info: this is easy - just use sysconf

    mem_total = sysconf(_SC_PHYS_PAGES) << page_k_shift;
    mem_free = sysconf(_SC_AVPHYS_PAGES) << page_k_shift;

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
    for(int i = 0; i < nswaps; i++) {
	swap_total += st->swt_ent[i].ste_pages;
	swap_free += st->swt_ent[i].ste_free;
    }
    swap_total <<= page_k_shift;
    swap_free <<= page_k_shift;
    free(st);

    if(cpu_times_vec) {
	if(old_cpu_times_vec) free(old_cpu_times_vec);
	old_cpu_times_vec = cpu_times_vec;
	cpu_times_vec = (unsigned *)malloc(sizeof(unsigned)
					   * num_cpus * CPUTIMES);
    }
    old_num_cpus = num_cpus;

    // cpu states: are stored as kstats named "cpu_statN", where N is the
    // cpu number. Unfortunately, the cpu numbers are not guessable so we
    // sweep the kstat chain for all of them, assuming (foolishly?)
    // that they are in order.

    kstat_chain_update(kc);
    int cpu = 0;
    for(kstat_t *ks = kc->kc_chain; ks; ks = ks->ks_next) {
	if(strncmp(ks->ks_name, "cpu_stat", 8) == 0) {
	    if(kstat_read(kc, ks, NULL) == -1) {
		perror("kstat_read");
		exit(1);
	    }
	    cpu_stat_t *cs = (cpu_stat_t *)ks->ks_data;
	    if(cpu + 1 >= (int)num_cpus) {
		num_cpus = cpu + 1;
		cpu_times_vec = (unsigned *)realloc(cpu_times_vec,
						    num_cpus * CPUTIMES
						    * sizeof(unsigned));
	    }
	    cpu_times(cpu, CPUTIME_USER) = cs->cpu_sysinfo.cpu[CPU_USER];
	    cpu_times(cpu, CPUTIME_SYSTEM) = cs->cpu_sysinfo.cpu[CPU_KERNEL];
	    cpu_times(cpu, CPUTIME_WAIT) = cs->cpu_sysinfo.cpu[CPU_WAIT];
	    cpu_times(cpu, CPUTIME_IDLE) = cs->cpu_sysinfo.cpu[CPU_IDLE];
	    cpu++;
	}
    }
}
#endif // SOLARIS

#ifdef MOSIX
void Procinfo::check_for_mosix()
{
    char path[256];
    strcpy(path, procdir);
    strcat(path, "/mosix");
    DIR *d = opendir(path);
    if(d) {
	closedir(d);
	mosix_running = TRUE;
	return;
    }
    mosix_running = FALSE;
}

Svec<int> Procinfo::mosix_nodes()
{
    Svec<int> nodes;
    char path[256];
    strcpy(path, procdir);
    strcat(path, "/mosix/nodes");
    DIR *d = opendir(path);
    if(d) {
	struct dirent *e;
	while((e = readdir(d)) != 0) {
	    int num;
	    if(sscanf(e->d_name, "%d", &num) == 1)
		nodes.add(num);
	}
	closedir(d);
    }
    return nodes;
}
#endif // MOSIX

int Procinfo::get_policy()
{
    if(policy == -1)
	policy = sched_getscheduler(pid);
    return policy;
}

int Procinfo::get_rtprio()
{
    if(rtprio == -1) {
	struct sched_param p;
	if(sched_getparam(pid, &p) == 0)
	    rtprio = p.sched_priority;
    }
    return rtprio;
}

#ifdef LINUX
void Procinfo::read_fd(int fdnum, char *path)
{
    int len;
    char buf[80];
    struct stat sb;

    // The fd mode is contained in the link permission bits
    if(lstat(path, &sb) < 0)
	return;
    int mode = 0;
    if(sb.st_mode & 0400) mode |= OPEN_READ;
    if(sb.st_mode & 0200) mode |= OPEN_WRITE;

    if( (len = readlink(path, buf, sizeof(buf) - 1)) > 0) {
	buf[len] = '\0';
	unsigned long dev, ino;
	if((buf[0] == '['	// Linux 2.0 style /proc/fd
	    && sscanf(buf, "[%lx]:%lu", &dev, &ino) == 2
	    && dev == 0)
	   || sscanf(buf, "socket:[%lu]", &ino) > 0) { // Linux 2.1
	    Sockinfo *si = Procinfo::socks[ino];
	    char buf[80];
	    if(si) {
		// a TCP or UDP socket
		sock_inodes->add(SockInode(fdnum, ino));
		sprintf(buf, "%sp socket %lu",
			si->proto == Sockinfo::TCP ? "tc" : "ud", ino);
		fd_files->add(new Fileinfo(fdnum, buf, mode));
		return;
	    } else {
		// maybe a unix domain socket?
		read_usockets();
		UnixSocket *us = Procinfo::usocks[ino];
		if(us) {
		    char *tp = "?", *st = "?";
		    switch(us->type) {
		    case SOCK_STREAM: tp = "stream"; break;
		    case SOCK_DGRAM: tp = "dgram"; break;
		    }
		    switch(us->state) {
		    case SSFREE: st = "free"; break;
		    case SSUNCONNECTED: st = "unconn"; break;
		    case SSCONNECTING: st = "connecting"; break;
		    case SSCONNECTED: st = "connected"; break;
		    case SSDISCONNECTING: st = "disconn"; break;
		    }
		    sprintf(buf, "unix domain socket %lu (%s, %s) ",
			    ino, tp, st);
		    QString s = buf;
		    s.append(us->name);
		    fd_files->add(new Fileinfo(fdnum, s, mode));
		    return;
		}
	    }
	}
	// assume fds will be read in increasing order
	fd_files->add(new Fileinfo(fdnum, buf, mode));
    }
}
#endif // LINUX

#ifdef SOLARIS
void Procinfo::read_fd(int fdnum, char *path)
{
    struct stat sb;
    if(lstat(path, &sb) < 0) {
	// The file has been closed, or we could really not stat it despite
	// having it open (could be a fd passed from another process).

	fd_files->add(new Fileinfo(fdnum, "(no info available)"));
	return;
    }
    // We could in principle find out more about the fd, such as its mode
    // (RDONLY, RDWR etc) and flags, but it's messy. pfiles uses an agent lwp
    // for this, but I don't know how to do it.
    QString s;
    const char *n;
    switch(sb.st_mode & S_IFMT) {
    case S_IFCHR:
	// if it is a tty, we might know its real name
	if(sb.st_rdev != (dev_t)-1) {
	    QString t = Ttystr::name(sb.st_rdev);
	    if(t[0] != '?') {
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
	if(chdir(path) >= 0) {
	    char buf[512];
	    if(getcwd(buf, sizeof(buf)) >= 0) {
		s.append(buf);
		break;
	    }
	}
	s.append("(unknown)");
	break;

    default:
	switch(sb.st_mode & S_IFMT) {
	case S_IFIFO:		// fifo or anonymous pipe
	    n = "pipe"; break;
	case S_IFDIR:		// this shouldn't happen
	    n = "directory"; break;
	case S_IFREG:
	    n = "file"; break;
	case S_IFSOCK:
	    n = "unix domain socket"; break;
	case S_IFDOOR:
	    n = "door"; break;
	default:
	    n = "unknown"; break;
	}
	s.sprintf("%s, dev %u:%u inode %u", n, (unsigned)major(sb.st_dev),
		  (unsigned)minor(sb.st_dev), (unsigned)sb.st_ino);
	break;
    }
    fd_files->add(new Fileinfo(fdnum, s));
}
#endif // SOLARIS

// return TRUE if /proc/PID/fd could be read, FALSE otherwise
// store fileinfo, and also socket inodes separately
bool Procinfo::read_fds()
{
    char path[80], *p;
    sprintf(path, "%s/%d/fd", procdir, pid);

    DIR *d = opendir(path);
    if(!d) return FALSE;

    if(!fd_files)
	fd_files = new Svec<Fileinfo*>(8);
    fd_files->clear();

#ifdef LINUX
    if(!sock_inodes)
	sock_inodes = new Svec<SockInode>(4);
    sock_inodes->clear();
#endif

    p = path + strlen(path) + 1;
    p[-1] = '/';

    struct dirent *e;
    while((e = readdir(d)) != 0) {
	if(e->d_name[0] == '.')
	    continue;		// skip . and ..
	strcpy(p, e->d_name);
	int fdnum = atoi(p);
	read_fd(fdnum, path);
    }
    closedir(d);
    return TRUE;
}

#ifdef LINUX

bool Procinfo::read_socket_list(Sockinfo::proto_t proto, char *pseudofile)
{
    char path[80];
    strcpy(path, procdir);
    strcat(path, "/net/");
    strcat(path, pseudofile);
    FILE *f = fopen(path, "r");
    if(!f) return FALSE;

    char buf[256];
    fgets(buf, sizeof(buf), f);	// skip header
    while(fgets(buf, sizeof(buf), f) != 0) {
	Sockinfo *si = new Sockinfo;
	si->proto = proto;
	unsigned local_port, rem_port, st, tr;
	sscanf(buf + 6, "%x:%x %x:%x %x %x:%x %x:%x %x %d %d %d",
	       &si->local_addr, &local_port, &si->rem_addr, &rem_port,
	       &st, &si->tx_queue, &si->rx_queue,
	       &tr, &si->tm_when, &si->rexmits,
	       &si->uid, &si->timeout, &si->inode);
	// fix fields that aren't sizeof(int)
	si->local_port = local_port;
	si->rem_port = rem_port;
	si->st = st;
	si->tr = tr;

	socks.insert(si->inode, si);
	if(socks.count() > socks.size() * 3)
	    socks.resize(socks.count());
    }
    fclose(f);
    return TRUE;
}

bool Procinfo::read_usocket_list()
{
    char path[80];
    strcpy(path, procdir);
    strcat(path, "/net/unix");
    FILE *f = fopen(path, "r");
    if(!f) return FALSE;

    char buf[256];
    fgets(buf, sizeof(buf), f);	// skip header
    while(fgets(buf, sizeof(buf), f)) {
	if(buf[0])
	    buf[strlen(buf) - 1] = '\0'; // chomp newline
	UnixSocket *us = new UnixSocket;
	unsigned q;
	unsigned type, state;
	int n;
	sscanf(buf, "%x: %x %x %x %x %x %ld %n",
	       &q, &q, &q, &us->flags, &type, &state, &us->inode, &n);
	us->name = buf + n;
	us->type = type;
	us->state = state;
	usocks.insert(us->inode, us);
	if(usocks.count() > usocks.size() * 3)
	    usocks.resize(usocks.count());
    }
    fclose(f);
    return TRUE;
}

void Procinfo::read_sockets()
{
    if(socks_current)
	return;

    socks.clear();
    if(!read_socket_list(Sockinfo::TCP, "tcp")
       || !read_socket_list(Sockinfo::UDP, "udp"))
	return;

    socks_current = TRUE;
}

void Procinfo::read_usockets()
{
    if(usocks_current)
	return;

    usocks.clear();
    if(!read_usocket_list())
	return;

    usocks_current = TRUE;
}

void Procinfo::invalidate_sockets()
{
    socks_current = usocks_current = FALSE;
}

// return TRUE if /proc/XX/maps could be read, FALSE otherwise
bool Procinfo::read_maps()
{
    // idea: here we could readlink /proc/XX/exe to identify the executable
    // when running 2.0.x
    char name[80];
    sprintf(name, "%s/%d/maps", procdir, pid);
    FILE *f = fopen(name, "r");
    if(!f) return FALSE;
    char line[1024];		// lines can be this long, or longer
    if(!maps)
	maps = new Svec<Mapsinfo *>;
    else
	maps->clear();
    
    while(fgets(line, sizeof(line), f)) {
	Mapsinfo *mi = new Mapsinfo;
	int n;
	sscanf(line, "%lx-%lx %4c %lx %x:%x %lu%n",
	       &mi->from, &mi->to, mi->perm, &mi->offset,
	       &mi->major, &mi->minor, &mi->inode, &n);
	if(line[n] != '\n') {
	    int len = strlen(line);
	    if(line[len - 1] == '\n')
		line[len - 1] = '\0';
	    while(line[n] == ' ' && line[n]) n++;
	    mi->filename = line + n;
	} else if((mi->major | mi->minor | mi->inode) == 0)
	    mi->filename = "(anonymous)";
	maps->add(mi);
    }
    fclose(f);
    return TRUE;
}

// return TRUE if /proc/XX/environ could be read, FALSE otherwise
bool Procinfo::read_environ()
{
    int bs = 4096;		// good start
    if(envblock) free(envblock);
    envblock = (char *)malloc(bs + 1);
    char path[128];
    sprintf(path, "%s/%d/environ", procdir, pid);
    int fd = open(path, O_RDONLY);
    if(fd < 0) {
	free(envblock);
	envblock = 0;
	return FALSE;
    }
    int n;
    int ofs = 0;
    while((n = read(fd, envblock + ofs, bs - ofs)) == bs - ofs) {
	ofs = bs;
	envblock = (char *)realloc(envblock, (bs += 4096) + 1);
    }
    close(fd);
    if(n < 0) {
	free(envblock);
	envblock = 0;
	return FALSE;
    }
    n += ofs;
    envblock[n] = '\0';
    if(!environ)
	environ = new Svec<NameValue>(64);
    else
	environ->clear();
    for(int i = 0; i < n;) {
	char *p = strchr(envblock + i, '=');
	if(p)
	    *p++ = '\0';
	else	// degenerate variable: treat as name with empty value
	    p = envblock + i + strlen(envblock + i);
	make_printable(envblock + i);
	make_printable(p);
	environ->add(NameValue(envblock + i, p));
	i = p - envblock + strlen(p) + 1;
    }
    return TRUE;
}

#endif // LINUX

#ifdef SOLARIS

// return TRUE if the process environment could be read, FALSE otherwise
bool Procinfo::read_environ()
{
    int fd;
    char file[128];
    sprintf(file, "/proc/%d/as", pid);
    if( (fd = open(file, O_RDONLY)) < 0)
       return FALSE;

    // Just read the first 8K from the environment. Adaptive code here is
    // possible, but not really worth the effort.
    int bs = 8192;
    if(envblock) free(envblock);
    envblock = (char *)malloc(bs);
    if(pread(fd, envblock, bs, env_ofs) < 0) {
       free(envblock);
       envblock = 0;
       return FALSE;
    }
    close(fd);
    envblock[bs - 1] = '\0';

    if(!environ)
       environ = new Svec<NameValue>(64);
    else
       environ->clear();

    for(int i = 0; i * (int)sizeof(char *) < bs && ((char **)envblock)[i];
       i++) {
       int b = ((char **)envblock)[i] - (char *)env_ofs;
       if(b < 0 || b >= bs)
           continue;           // outside retrieved memory block
       char *val = strchr(envblock + b, '=');
       if(val)
           *val++ = '\0';
       else
           val = (char *)"";	// degenerate: treat as name with empty value
       make_printable(envblock + b);
       make_printable(val);
       environ->add(NameValue(envblock + b, val));
    }
    return TRUE;
}

// Try using /proc/bin/pmap to add file names to the memory map
void Procinfo::read_pmap_maps()
{
    char buf[256];
    sprintf(buf, "/usr/proc/bin/pmap %d 2>/dev/null", pid);
    FILE *pmap = popen(buf, "r");
    if(!pmap)
	return;

    // skip first line
    if(!fgets(buf, sizeof buf, pmap)) {
	pclose(pmap);
	return;
    }

    int map_num = 0;
    while(fgets(buf, sizeof buf, pmap)) {
	// Each output line from pmap looks like
	// <address> <size>K <mode> <file>
	// We use <address> and <size> only to match with previously read
	// map info, and only use <file> here.

	unsigned long addr;
	unsigned len;
	int next;
	char *p;
	if(sscanf(buf, "%lx %dK%n", &addr, &len, &next) != 2)
	    continue;

	// Correlate this with info already gathered. Assume they are in the
	// same order (ascending by address).
	Mapsinfo *mi;
	int i = map_num;
	while(i < maps->size() && (mi = (*maps)[i])->from != addr)
	    i++;
	// Mismatches can happen since changes can have taken place since
	// we read the maps. If so, skip this mapping and try the next.
	if(mi->to != addr + ((unsigned long)len << 10) || i == maps->size())
	    continue;
	map_num = i + 1;

	while(buf[next] == ' ')
	    next++;
	while(buf[next] && buf[next] != ' ')
	    next++;
	while(buf[next] == ' ')
	    next++;
	// At this point  we are looking at a file name, or at a suitable
	// designator like [ heap ], [ anon ] or [ stack ]. Use it right away
	// (after peeling off the newline)
	int l = strlen(buf + next);
	if(buf[next + l - 1] == '\n')
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
    if(!f) return FALSE;
    if(!maps)
	maps = new Svec<Mapsinfo *>;
    else
	maps->clear();

    prmap_t pm;
    while(fread(&pm, sizeof(pm), 1, f) == 1) {
	Mapsinfo *mi = new Mapsinfo;
	mi->from = pm.pr_vaddr;
	mi->to = pm.pr_vaddr + pm.pr_size;
	mi->offset = pm.pr_offset;
	mi->perm[0] = pm.pr_mflags & MA_READ   ? 'r' : '-';
	mi->perm[1] = pm.pr_mflags & MA_WRITE  ? 'w' : '-';
	mi->perm[2] = pm.pr_mflags & MA_EXEC   ? 'x' : '-';
	mi->perm[3] = pm.pr_mflags & MA_SHARED ? 's' : 'p';

	if(pm.pr_mapname[0]) {
	    // To find device/inode, stat the file in /proc/#/object:
	    char obj[128];
	    sprintf(obj, "%s/%d/object/%s", procdir, pid, pm.pr_mapname);
	    struct stat sb;
	    if(lstat(obj, &sb) < 0) {
		delete mi;
		continue;
	    }
	    mi->major = major(sb.st_dev);
	    mi->minor = minor(sb.st_dev);
	    mi->inode = sb.st_ino;
	    if(strcmp(pm.pr_mapname, "a.out") == 0)
		mi->filename = "(executable)";
	} else {
	    mi->major = mi->minor = mi->inode = 0;
	    mi->filename = "(anonymous)";
	}

	maps->add(mi);
    }
    fclose(f);

    // If desired and possible, use /usr/proc/bin/pmap to get the
    // names of the mapped files
    static int myeuid = geteuid();
    if(Qps::use_pmap && (myeuid == 0 || myeuid == euid))
	read_pmap_maps();

    return TRUE;
}

#endif // SOLARIS

Category::~Category()
{}

int Category::compare(Procinfo *a, Procinfo *b)
{
#if QT_VERSION < 200
    return strcmp(string(a), string(b));
#else
    return string(a).compare(string(b));
#endif
}

Cat_int::Cat_int(const char *heading, const char *explain,
		 int w, int Procinfo::*member)
       : Category(heading, explain), int_member(member), field_width(w)
{}

QString Cat_int::string(Procinfo *p)
{
    QString s;
    s.setNum(p->*int_member);	
    return s;
}

int Cat_int::compare(Procinfo *a, Procinfo *b)
{
    // qsort() only cares about the sign of the number returned by the
    // comparison function; only a subtraction is necessary
    return a->*int_member - b->*int_member;
}

Cat_uintl::Cat_uintl(const char *heading, const char *explain, int w,
		     unsigned long Procinfo::*member)
        : Category(heading, explain), uintl_member(member), field_width(w)
{}

QString Cat_uintl::string(Procinfo *p)
{
    QString s;
    s.setNum(p->*uintl_member);	
    return s;
}

int Cat_uintl::compare(Procinfo *a, Procinfo *b)
{
    int bu = b->*uintl_member, au = a->*uintl_member;
    return bu >= au ? (bu == au ? 0 : 1) : -1;
}

Cat_hex::Cat_hex(const char *heading, const char *explain, int w,
		 unsigned long Procinfo::*member)
        : Cat_uintl(heading, explain, w, member)
{}

QString Cat_hex::string(Procinfo *p)
{
    QString s;
    s.sprintf("%8x", (unsigned)(p->*uintl_member));
    return s;
}

Cat_swap::Cat_swap(const char *heading, const char *explain)
        : Category(heading, explain)
{}

QString Cat_swap::string(Procinfo *p)
{
    QString s;
    // It can actually happen that size < resident (XSun under Solaris 2.6)
    s.setNum(p->size > p->resident ? p->size - p->resident : 0);
    return s;
}

int Cat_swap::compare(Procinfo *a, Procinfo *b)
{
    return (b->size - b->resident) - (a->size - a->resident);
}

Cat_string::Cat_string(const char *heading, const char *explain,
		       QString Procinfo::*member)
        : Category(heading, explain), str_member(member)
{}

QString Cat_string::string(Procinfo *p)
{
    return p->*str_member;
}

Cat_user::Cat_user(const char *heading, const char *explain)
        : Cat_string(heading, explain)
{}

QString Cat_user::string(Procinfo *p)
{
#ifdef MOSIX
    if(p->isremote)
	return "-mosix-";
#endif
    if(p->uid == p->euid)
	return Uidstr::userName(p->uid);
    else {
	QString s = Uidstr::userName(p->uid);
#if QT_VERSION < 200
	s.detach();
#endif
	s.append(p->euid == 0 ? "*" : "+");
	return s;
    }
}

Cat_group::Cat_group(const char *heading, const char *explain)
        : Cat_string(heading, explain)
{}

QString Cat_group::string(Procinfo *p)
{
    if(p->gid == p->egid)
	return Uidstr::groupName(p->gid);
    else {
	QString s = Uidstr::groupName(p->gid);
#if QT_VERSION < 200
	s.detach();
#endif
	s.append("*");
	return s;
    }
}

Cat_wchan::Cat_wchan(const char *heading, const char *explain)
        : Cat_string(heading, explain)
{}

QString Cat_wchan::string(Procinfo *p)
{
    return Wchan::name(p->wchan);
}

Cat_cmdline::Cat_cmdline(const char *heading, const char *explain)
            : Cat_string(heading, explain)
{}

QString Cat_cmdline::string(Procinfo *p)
{
    if(p->cmdline.isEmpty()) {
	QString s("(");
	s.append(p->command);
	s.append(")");
	return s;
    } else {
	if(Qps::show_file_path)
	    return p->cmdline;
	else {
	    QString s(p->cmdline);
#if QT_VERSION < 200
	    s.detach();
#endif
	    int i = s.find(' ');
	    if(i < 0)
		i = s.length();
	    if(i > 0) {
		i = s.findRev('/', i - 1);
		if(i >= 0)
		    s.remove(0, i + 1);
	    }
	    return s;
	}
    }
}

Cat_dir::Cat_dir(const char *heading, const char *explain, const char *dirname,
		 QString Procinfo::*member)
    : Cat_string(heading, explain),
      dir(dirname),
      cache(member)
{}

QString Cat_dir::string(Procinfo *p)
{
    if((p->*cache).isNull()) {
	char path[128], buf[512];
	sprintf(path, "%s/%d/%s", procdir, p->pid, dir);

#ifdef LINUX
	int n = readlink(path, buf, sizeof(buf) - 1);
	if(n < 0) {
	    // Either a kernel process, or access denied.
	    // A hyphen is visually least disturbing here.
	    p->*cache = "-";
	    return p->*cache;
	} else if(buf[0] != '[') {
	     // linux >= 2.1.x: path name directly in link
	    buf[n] = '\0';
	    p->*cache = buf;
	    return p->*cache;
	}
#endif

	// Either a Linux 2.0 link in [device]:inode form, or a Solaris link.
	// To resolve it, we just chdir() to it and see where we end up.
	// Perhaps we should change back later?
	if(chdir(path) < 0) {
           p->*cache = "-";    // Most likely access denied
	} else {
	    // getcwd() is fairly expensive, but this is cached anyway
	    if(!getcwd(buf, sizeof(buf))) {
		p->*cache = "(deleted)";
	    } else 
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
#ifdef SOLARIS
    if(p->state == 'Z')
	return s;
#endif
    s[1] = (p->resident == 0 && p->state != 'Z') ? 'W' : ' ';
    int ni = p->nice;
#ifdef SOLARIS
    if(!Qps::normalize_nice)
	ni -= NZERO;
#endif
    s[2] = (ni > 0) ? 'N' : ((ni < 0) ? '<' : ' ');
    return s;
}

Cat_policy::Cat_policy(const char *heading, const char *explain)
           : Category(heading, explain)
{}

QString Cat_policy::string(Procinfo *p)
{
    QString s;
#ifdef LINUX
    switch(p->get_policy()) {
    case SCHED_FIFO:
	s = "FI"; break;	// first in, first out
    case SCHED_RR:
	s = "RR"; break;	// round-robin
    case SCHED_OTHER:
	s = "TS"; break;	// time-sharing
    default:
	s = "??"; break;
    }
#endif
#ifdef SOLARIS
    s = "  ";
    s[0] = p->policy_name[0];
    s[1] = p->policy_name[1];
#endif
    return s;
}

int Cat_policy::compare(Procinfo *a, Procinfo *b)
{
#ifdef LINUX
    return b->get_policy() - a->get_policy();
#endif
#ifdef SOLARIS
    int r = b->policy_name[0] - a->policy_name[0];
    return r ? r : b->policy_name[1] - a->policy_name[1];
#endif
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
    if(Qps::cumulative)
	ticks += p->cutime;
    int t = ticks / HZ;		// seconds
    if(t < 10) {
	int hundreds = ticks / (HZ / 100) % 100;
	s.sprintf("%1d.%02ds", t, hundreds);
    } else if(t < 100 * 60) {
	s.sprintf("%2d:%02d", t / 60, t % 60);
    } else if(t < 100 * 3600) {
	int h = t / 3600;
	t %= 3600;
	s.sprintf("%2d:%02dh", h, t / 60);
    } else {
	int d = t / 86400;
	t %= 86400;
	s.sprintf("%dd%dh", d, t / 3600);
    }
    return s;
}

int Cat_time::compare(Procinfo *a, Procinfo *b)
{
    int at = a->utime, bt = b->utime;
    if(Qps::cumulative) {
	at += a->cutime;
	bt += b->cutime;
    }
    return bt - at;
}

Cat_start::Cat_start(const char *heading, const char *explain)
          : Category(heading, explain)
{}

QString Cat_start::string(Procinfo *p)
{
#ifdef SOLARIS
    if(p->state == 'Z')
	return "-";		// Solaris zombies have no valid start time
#endif
    time_t start = p->boot_time + p->starttime / (unsigned)HZ;
    QString s;
    char *ct = ctime(&start);
    if(p->tv.tv_sec - start < 86400) {
	ct[16] = '\0';
	s = ct + 11;
    } else {
	ct[10] = '\0';
	s = ct + 4;
    }
    return s;
}

int Cat_start::compare(Procinfo *a, Procinfo *b)
{
    unsigned long bs = b->starttime, as = a->starttime;
    return bs >= as ? (bs == as ? 0 : 1) : -1;
}

Cat_percent::Cat_percent(const char *heading, const char *explain, int w,
			 float Procinfo::*member)
        : Category(heading, explain), float_member(member), field_width(w)
{}

QString Cat_percent::string(Procinfo *p)
{
    QString s;
    s.sprintf("%01.2f", (double)(p->*float_member));
    return s;
}

int Cat_percent::compare(Procinfo *a, Procinfo *b)
{
    float at = a->*float_member, bt = b->*float_member;
    return at < bt ? 1 : (at > bt ? -1 : 0);
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
    // Note: When adding/removing/changing the fields, the save file
    // version must be increased!
    char *command, *command_line;
    allcats.set(F_PID, new Cat_int("PID", "Process ID", 6, &Procinfo::pid));
    allcats.set(F_PPID, new Cat_int("PPID", "Parent process ID", 6,
				    &Procinfo::ppid));
    allcats.set(F_PGID, new Cat_int("PGID", "Process group ID", 6,
				    &Procinfo::pgrp));
    allcats.set(F_SID, new Cat_int("SID", "Session ID", 6,
				   &Procinfo::session));
    allcats.set(F_TTY, new Cat_tty("TTY", "Controlling tty"));
#ifdef LINUX
    allcats.set(F_TPGID, new Cat_int("TPGID", "Process group ID of tty owner",
				     6, &Procinfo::tpgid));
#endif
#ifdef MOSIX
    allcats.set(F_MIGR, new Cat_string("MIGR",
				       "Process migrated to (>node) "
				       "or from (node>)", &Procinfo::migr));
    allcats.set(F_LOCKED, new Cat_int("LOCK",
				      "Whether this process is locked to"
				      " this node", 6, &Procinfo::locked));
    allcats.set(F_NMIGS, new Cat_int("NMIGS",
				     "How many times this process"
				     " has migrated", 6, &Procinfo::nmigs));
    allcats.set(F_NOMOVE, new Cat_string("NOMOVE",
					 "Reason for why process can't move",
					 &Procinfo::cantmove));
    allcats.set(F_RPID, new Cat_int("RPID",
					 "Process ID on home node", 6,
					 &Procinfo::remotepid));
#endif

    allcats.set(F_USER, new Cat_user("USER",
				    "Owner (*=suid root, +=suid other user"
#ifdef MOSIX
				     " -mosix-=immigrated from other node"
#endif
				     ")"));
    allcats.set(F_GROUP, new Cat_group("GROUP", "Group name (*=sgid other)"));
    allcats.set(F_UID, new Cat_int("UID", "Real user ID", 6, &Procinfo::uid));
    allcats.set(F_EUID, new Cat_int("EUID", "Effective user ID", 6,
				    &Procinfo::euid));
#ifdef LINUX
    allcats.set(F_SUID, new Cat_int("SUID", "Saved user ID (Posix)", 6,
				    &Procinfo::suid));
    allcats.set(F_FSUID, new Cat_int("FSUID", "File system user ID", 6,
				     &Procinfo::fsuid));
#endif
    allcats.set(F_GID, new Cat_int("GID", "Real group ID", 6, &Procinfo::gid));
    allcats.set(F_EGID, new Cat_int("EGID", "Effective group ID", 6,
				    &Procinfo::egid));
#ifdef LINUX
    allcats.set(F_SGID, new Cat_int("SGID", "Saved group ID (Posix)", 6,
				    &Procinfo::sgid));
    allcats.set(F_FSGID, new Cat_int("FSGID", "File system group ID", 6,
				     &Procinfo::fsgid));
#endif
    allcats.set(F_PRI, new Cat_int("PRI", "Dynamic priority", 4,
				   &Procinfo::priority));
    allcats.set(F_NICE, new Cat_int("NICE",
				"Scheduling favour (higher -> less cpu time)",
				    4, &Procinfo::nice));
    allcats.set(F_PLCY, new Cat_policy("PLCY",
				       "Scheduling policy"));
    allcats.set(F_RPRI, new Cat_rtprio("RPRI",
				  "Realtime priority (0-99, more is better)"));
#ifdef SOLARIS
    allcats.set(F_NLWP, new Cat_int("NLWP", "Number of threads in process",
				    5, &Procinfo::nthreads));
    allcats.set(F_ARCH, new Cat_int("ARCH", "Architecture (address bits)",
				    2, &Procinfo::addr_bits));
#endif
    allcats.set(F_MAJFLT, new Cat_uintl("MAJFLT",
					"Number of major faults (disk access)",
					8, &Procinfo::majflt));
    allcats.set(F_MINFLT, new Cat_uintl("MINFLT",
				     "Number of minor faults (no disk access)",
					8, &Procinfo::minflt));
#ifdef LINUX
    allcats.set(F_TRS, new Cat_uintl("TRS", "Text resident set size in Kbytes",
				     8, &Procinfo::trs));
    allcats.set(F_DRS, new Cat_uintl("DRS", "Data resident set size in Kbytes",
				     8, &Procinfo::drs));
#endif
    allcats.set(F_SIZE, new Cat_uintl("SIZE",
				     "Virtual image size of process in Kbytes",
				      8, &Procinfo::size));
    allcats.set(F_SWAP, new Cat_swap("SWAP", "Kbytes on swap device"));
    allcats.set(F_RSS, new Cat_uintl("RSS",
				     "Resident set size; Kbytes of program "
				     "in memory", 8, &Procinfo::resident));
#ifdef LINUX
    allcats.set(F_SHARE, new Cat_uintl("SHARE", "Shared memory in Kbytes",
				       8, &Procinfo::share));
    allcats.set(F_DT, new Cat_uintl("DT",
				    "Number of dirty (non-written) pages",
				    7, &Procinfo::dt));
#endif
    allcats.set(F_STAT, new Cat_state("STAT", "State of the process"));
    allcats.set(F_FLAGS, new Cat_hex("FLAGS", "Process flags (hex)", 9,
				     &Procinfo::flags));
    allcats.set(F_WCHAN, new Cat_wchan("WCHAN",
				 "Kernel function where process is sleeping"));
    allcats.set(F_WCPU, new Cat_percent("%WCPU",
				   "Weighted percentage of CPU (30 s average)",
					6, &Procinfo::wcpu));
    allcats.set(F_CPU, new Cat_percent("%CPU",
				    "Percentage of CPU used since last update",
				       6, &Procinfo::pcpu));
    allcats.set(F_MEM, new Cat_percent("%MEM",
				  "Percentage of memory used (RSS/total mem)",
				       6, &Procinfo::pmem));
    allcats.set(F_START, new Cat_start("START", "Time process started"));
    allcats.set(F_TIME, new Cat_time("TIME",
				     "Total CPU time used since start"));
    allcats.set(F_CPUNUM, new Cat_int("CPU", "CPU the process is executing on",
				      3, &Procinfo::which_cpu));
#ifdef OLD_LABEL        
    command="COMM"; // label in linux source code --> /proc/XXX/stat
#else        
    command="COMMAND";
#endif
    allcats.set(F_COMM, new Cat_string(command,
				       "Command that started the process",
				       &Procinfo::comm));
    allcats.set(F_CWD, new Cat_dir("CWD", "Current working directory",
				   "cwd", &Procinfo::cwd));
    allcats.set(F_ROOT, new Cat_dir("ROOT", "Root directory of process",
				    "root", &Procinfo::root));
#ifdef OLD_LABEL        
    command_line="CMDLINE"; //reference to /proc/XXX/cmdline
#else        
    command_line="COMMAND_LINE";
#endif
    allcats.set(F_CMDLINE, new Cat_cmdline(command_line,
				     "Command line that started the process"));

    for(int i = 0; i < allcats.size(); i++)
	allcats[i]->index = i;

    Procinfo::init_static();

    current_gen=0; // !
}

void Proc::newproc(Procinfo *p)
{
    Procinfo *oldp = procs[p->pid];
    if(oldp) {
	// calculate pcpu (and wcpu for Linux) from previous procinfo
	int dt = (p->tv.tv_usec - oldp->tv.tv_usec) / (1000000 / HZ)
	         + (p->tv.tv_sec - oldp->tv.tv_sec) * HZ;
	int dcpu = p->utime - oldp->utime;
	p->pcpu = 100.0 * dcpu / dt;
	if(p->pcpu > 99.99) p->pcpu = 99.99;

#if defined(LINUX)
	const float a = Procview::avg_factor;
	p->wcpu = a * oldp->wcpu + (1 - a) * p->pcpu;
#endif

	// propagate some fields to new incarnation
	p->selected = oldp->selected;
	p->details = oldp->details;
	p->hidekids = oldp->hidekids;
	oldp->details = 0;
	if(p->details)
	    p->details->set_procinfo(p);

#ifdef LINUX
	if(Procinfo::num_cpus > 1) {
	    // SMP: see which processor was used the most
	    int best_cpu = -1;
	    unsigned long most = 0;
	    for(unsigned cpu = 0; cpu < Procinfo::num_cpus; cpu++) {
		unsigned long delta =
		    p->per_cpu_times[cpu] - oldp->per_cpu_times[cpu];
		if(delta > most) {
		    most = delta;
		    best_cpu = cpu;
		}
		// if no cpu time has been spent, use previous value
		p->which_cpu = (best_cpu >= 0) ? best_cpu : oldp->which_cpu;
	    }
	}
#endif
	oldp->deref();
    } else {
	// New process
#ifdef LINUX
	// %cpu first time = (cpu time since start) / (time since start)
	int jiffies_since_boot = p->tv.tv_usec / (1000000 / HZ)
	    + (p->tv.tv_sec - p->boot_time) * HZ;
	int dt = jiffies_since_boot - p->starttime;
	int dcpu = p->utime;
	p->pcpu = 100.0 * dcpu / dt;
	if(dt == 0 || p->pcpu > 99.99 || p->pcpu < 0)
	    p->pcpu = 0.0;
	p->selected = FALSE;
#endif
	p->wcpu = p->pcpu;	// just a start

#ifdef LINUX
	if(Procinfo::num_cpus > 1) {
	    // first tick: count times from 0
	    unsigned long most = 0;
	    for(unsigned cpu = 0; cpu < Procinfo::num_cpus; cpu++) {
		unsigned long t = p->per_cpu_times[cpu];
		if(t > most) {
		    most = t;
		    p->which_cpu = cpu;
		}
	    }
	}
#endif
    }
    procs.replace(p->pid, p);
    if(procs.count() > procs.size())
	procs.resize(procs.count() * 2 - 1);
}

// update the process list
void Proc::refresh()
{
	current_gen=!current_gen; // 1,2,3,4....

	Procinfo::read_common();

	int pid;

	DIR *d = opendir(procdir);
	struct dirent *e;
	while((e = readdir(d)) != 0) {
		if(e->d_name[0] >= '0' && e->d_name[0] <= '9') { // good idea !

			pid=atoi(e->d_name);
			Procinfo *pi = new Procinfo(pid);

			if(pi->pid == -1)
				delete pi;		// already gone
			else {
				pi->generation = current_gen;
				newproc(pi);
			}
		}
	}
	closedir(d);

#ifdef MOSIX
	if(Procinfo::mosix_running) {
		char path[256];
		strcpy(path, procdir);
		strcat(path, "/mosix/remote");
		d = opendir(path);
		if(d) {
			while((e = readdir(d)) != 0) {
				if(e->d_name[0] >= '0' && e->d_name[0] <= '9') {
					Procinfo *pi = new Procinfo(atoi(e->d_name));
					if(pi->pid == -1)
						delete pi;		// already gone
					else {
						pi->generation = current_gen;
						newproc(pi);
					}
				}
			}
			closedir(d);
		}
	}
#endif // MOSIX

    // remove Procinfos of nonexisting processes
    for(Q3IntDictIterator<Procinfo> it(procs); it.current();) {
	Procinfo *p = it.current();
	if(p->generation != current_gen) {
	    procs.remove(p->pid);
	    p->deref();
	} else
	    ++it;
    }
}

Category *Proc::cat_by_name(const char *s)
{
    if(s) {
	for(int i = 0; i < allcats.size(); i++)
	    if(strcmp(allcats[i]->name, s) == 0)
		return allcats[i];
    }
    return 0;
}

int  Proc::field_id_by_name(const char *s)
{
    if(s) {
	for(int i = 0; i < allcats.size(); i++)
	    if(strcmp(allcats[i]->name, s) == 0)
		return i;
    }
    return -1;
}

int Procview::custom_fields[18] = {F_PID, F_TTY, F_USER, F_NICE,
#ifdef SOLARIS
			       F_NLWP,
#endif
			       F_SIZE, F_RSS,
			       F_STAT, F_CPU, F_START, F_TIME,
			       F_CMDLINE, F_END};


int Procview::user_fields[] = {F_PID, F_TTY, F_USER, F_NICE,
#ifdef SOLARIS
			       F_NLWP,
#endif
			       F_SIZE, F_RSS,
			       F_STAT, F_CPU, F_START, F_TIME,
			       F_CMDLINE, F_END};

#ifdef MOSIX
int Procview::user_fields_mosix[] = {F_PID, F_TTY, F_USER, F_NICE,
				     F_MIGR, F_NMIGS, 
				     F_SIZE, F_RSS,
				     F_STAT, F_CPU, F_START, F_TIME,
				     F_CMDLINE, F_END};
#endif

int Procview::jobs_fields[] = {F_PID, F_PPID, F_PGID, F_SID, 
			       F_TTY,
#ifdef LINUX
			       F_TPGID,
#endif
			       F_STAT, F_UID, F_TIME, F_CMDLINE, F_END};
#ifdef MOSIX
int Procview::jobs_fields_mosix[] = {F_PID, F_PPID, F_PGID, F_SID, 
				     F_MIGR, F_NMIGS, F_LOCKED, F_NOMOVE,
				     F_TTY,
				     F_TPGID,
				     F_STAT, F_UID, F_TIME, F_CMDLINE, F_END};
#endif

int Procview::mem_fields[] = {F_PID, 
			      F_TTY, F_MAJFLT, F_MINFLT,
#ifdef LINUX
			      F_TRS, F_DRS,
#endif
			      F_SIZE, F_SWAP, F_RSS,
#ifdef LINUX
			      F_SHARE, F_DT,
#endif
			      F_CMDLINE, 
			      F_END};
#ifdef MOSIX
int Procview::mem_fields_mosix[] = {F_PID, 
				    F_MIGR,
				    F_TTY, F_MAJFLT, F_MINFLT,
				    F_TRS, F_DRS,
				    F_SIZE, F_SWAP, F_RSS,
				    F_SHARE, F_DT,
				    F_CMDLINE, 
				    F_END};
#endif

float Procview::avg_factor = 1.0;

Procview::Procview(Proc *p)
        : proc(p)
{
    sortcat = p->allcats[F_WCPU];
    reversed = FALSE;
    viewproc = ALL;
    viewfields = USER;
    treeview = TRUE;	// init_mode by fasthyun 
    set_fields();
}

// read new process info
void Procview::refresh()
{
    for(int i = 0; i < old_procs.size(); i++)
	old_procs[i]->deref();
    old_procs = procs;
    procs.clear();
    proc->refresh(); // update the process list ,read "/proc"
    rebuild();
}

bool Procview::accept_proc(Procinfo *p)
{
    static int my_uid = getuid();
    return viewproc == ALL
	|| viewproc == OWNED && p->uid == my_uid
	|| viewproc == NROOT && p->uid != 0
	|| viewproc == RUNNING && strchr("ORDW", p->state) != 0
#ifdef MOSIX      
	|| viewproc == RUNNING && p->where > 0
#endif
	;
}

// be called by Procview::rebuild()
void Procview::build_tree()
{
	if(root_procs.size() > 0) {
		Procinfo *p;
		for(Q3IntDictIterator<Procinfo> it(proc->procs); (p = it.current()); ++it)
			if(p->children)
				p->children->clear();
		root_procs.clear();
	}
	Procinfo *p;
	for(Q3IntDictIterator<Procinfo> it(proc->procs); (p = it.current()); ++it) {
		if(accept_proc(p)) {
			Procinfo *parent = 0;

			if(p->ppid && (parent = proc->procs[p->ppid])
					&& accept_proc(parent)) {
				if(!parent->children)
					parent->children = new Svec<Procinfo *>(4);
				parent->children->add(p);
			} 
			else
			{		     
				root_procs.add(p);
			}

		} else
			p->selected = FALSE;
	}
}

// re-sort the process info
void Procview::rebuild()
{
    for(int i = 0; i < procs.size(); i++)
	procs[i]->deref(); // delete procs ?
    procs.clear();
    if(treeview) {
	build_tree();
	parent_rows.clear();
	linearize_tree(&root_procs, 0, -1);
    } else {
	for(Q3IntDictIterator<Procinfo> it(proc->procs); it.current(); ++it) {
	    Procinfo *p = it.current();
	    if(accept_proc(p))
		procs.add(p->ref());
	    else
		p->selected = FALSE;
	}
	static_sortcat = sortcat;
	procs.sort(reversed ? compare_backwards : compare);
    }
}

//  ????
void Procview::linearize_tree(Svec<Procinfo *> *ps, int level, int prow)
{
    static_sortcat = sortcat;
    ps->sort(reversed ? compare_backwards : compare);
    for(int i = 0; i < ps->size(); i++) {
	Procinfo *p = (*ps)[i];
	p->level = level;
	p->lastchild = FALSE;
	procs.add(p->ref());
	parent_rows.add(prow);
	if(p->children && !p->hidekids)
	    linearize_tree(p->children, level + 1, procs.size() - 1);
    }
    if(ps->size() > 0)
	(*ps)[ps->size() - 1]->lastchild = TRUE;
}

void Procview::set_fields_list(int fields[])
{
    cats.clear();
    for(int i = 0; fields[i] != F_END; i++)
    {
	cats.add(proc->allcats[fields[i]]);
	//printf("DEBUG: %d %s \n", i,proc->allcats[fields[i]]->name);
    }
}

void Procview::set_fields()
{
	switch(viewfields) {
		case USER:
#ifdef MOSIX
			if(Procinfo::mosix_running) {
				set_fields_list(user_fields_mosix);
				break;
			}
#endif
			set_fields_list(user_fields);
			break;
		case JOBS:
#ifdef MOSIX
			if(Procinfo::mosix_running) {
				set_fields_list(jobs_fields_mosix);
				break;
			} 
#endif
			set_fields_list(jobs_fields);
			break;
		case MEM:
#ifdef MOSIX
			if(Procinfo::mosix_running) {
				set_fields_list(mem_fields_mosix);
				break;
			} 
#endif
			set_fields_list(mem_fields);
			break;
		case CUSTOM:
			set_fields_list(custom_fields);
			break;

	}

}

// return the column number of a field, or -1 if not displayed
int Procview::findCol(int field)
{
    for(int i = 0; i < cats.size(); i++)
	if(cats[i] == proc->allcats[field])
	    return i;
    return -1;
}

// add a category (last)
void Procview::add_cat(Category *c)
//void Procview::add_category(Category *c)
{
    cats.add(c);
}

void Procview::remove_cat(int index)
{
    cats.remove(index);
}

// deduce whether the currently selected fields correspond to a field list
void Procview::deduce_fields()
{
    return;

    if(viewfields != CUSTOM)
	return;
    Procview::fieldstates tags[3] = {USER, JOBS, MEM};
    int *lists[3] = {user_fields, jobs_fields, mem_fields};
#ifdef MOSIX
    if(Procinfo::mosix_running) {
	lists[0] = user_fields_mosix;
	lists[1] = jobs_fields_mosix;
	lists[2] = mem_fields_mosix;
    }
#endif
    for(int i = 0; i < 3; i++) {
	int *l = lists[i];
	int j;
	for(j = 0; l[j] != F_END; j++)
	    if(findCol(l[j]) < 0)
		break;
	if(l[j] == F_END && j == cats.size()) {
	    viewfields = tags[i];
	    return;
	}
    }
}


Category *Procview::static_sortcat = 0;

int Procview::compare(Procinfo *const *a, Procinfo *const *b)
{
    int r = static_sortcat->compare(*a, *b);
    return (r == 0) ? ((*a)->pid > (*b)->pid ? 1 : -1) : r;
}

int Procview::compare_backwards(Procinfo *const *a, Procinfo *const *b)
{
    int r = static_sortcat->compare(*b, *a);
    return (r == 0) ? ((*b)->pid > (*a)->pid ? 1 : -1) : r;
}

