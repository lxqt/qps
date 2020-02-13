/*
 * fm-search-uri.c
 * This file is part of qps -- Qt-based visual process status monitor
 *
 * Copyright 2015 Hong Jen Yee (PCMan) <pcman.tw@gmail.com>
 * Copyright 2012-2014 Andriy Grytsenko (LStranger) <andrej@rep.kiev.ua>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

// for Non-ASCII locale
//#include <global.h>

//#include <qtextcodec.h>
// extern QTextCodec * codec ;
//#define UniString(str)   codec->toUnicode(str)

int flag_24_ok; // we presume a kernel 2.4.x

#include <grp.h>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h> // log()
#include <string.h>
#include <QString>

// COMMON
bool Procview::treeview = 0; // true
bool Procview::flag_show_file_path = false;
bool Procview::flag_cumulative = false;  // times cumulative with children's
bool Procview::flag_pcpu_single = false; // %CPU= pcpu/num_cpus

// COMMON?
// return username from /etc/passwd
const QString userName(int uid, int euid)
{
    char buff[128];
    struct passwd *pw = getpwuid(uid);
    if (!pw)
    {
        // dont have name !
        sprintf(buff, "%d", uid);
    }
    else
        strcpy(buff, pw->pw_name);

    if (uid != euid)
        strcat(buff, euid == 0 ? "*" : "+");
    return QString(buff);
}

// return group name (possibly numeric)
const QString groupName(int gid, int /*egid*/)
{
    QString res;
    struct group *gr = getgrgid(gid);
    if (!gr)
        res = QString::number(gid);
    else
        res = QString::fromLocal8Bit(gr->gr_name);
    return res;
}

Category::~Category() {}

int Category::compare(Procinfo *a, Procinfo *b)
{
    return string(a).compare(string(b));
}

Cat_int::Cat_int(const QString &heading, const QString &explain, int w,
                 int Procinfo::*member)
    : Category(heading, explain), int_member(member), field_width(w)
{
}

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

// COMMON
Cat_percent::Cat_percent(const QString &heading, const QString &explain, int w,
                         float Procinfo::*member)
    : Category(heading, explain), float_member(member), field_width(w)
{
}

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
// added 2006/05/07
Cat_memory::Cat_memory(const QString &heading, const QString &explain, int w,
                       unsigned long Procinfo::*member)
    : Category(heading, explain), uintl_member(member), field_width(w)
{
}

QString Cat_memory::string(Procinfo *p)
{
    QString s;
    long sizeK = p->*uintl_member;
    double sizeM = static_cast<double>(sizeK) / 1024;
    if (sizeM > 1.0)
        s = QString::number(sizeM, 'f', 1) + QStringLiteral("M");
    else
        s = QString::number(sizeK) + QStringLiteral("K");

    if (sizeK == 0)
        s = QStringLiteral("0");
    return s;
}

int Cat_memory::compare(Procinfo *a, Procinfo *b)
{
    int bu = b->*uintl_member, au = a->*uintl_member;
    return bu >= au ? (bu == au ? 0 : 1) : -1;
}

Cat_uintl::Cat_uintl(const QString &heading, const QString &explain, int w,
                     unsigned long Procinfo::*member)
    : Category(heading, explain), uintl_member(member), field_width(w)
{
}

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

Cat_hex::Cat_hex(const QString &heading, const QString &explain, int w,
                 unsigned long Procinfo::*member)
    : Cat_uintl(heading, explain, w, member)
{
}

// QString.sprintf 2x speed than glibc.sprintf (by fasthyun@magicn.com)
// but QString structuring & destructring eat more time
QString Cat_hex::string(Procinfo *p)
{
    QString s;
    s.sprintf("%8x", (unsigned)(p->*uintl_member));
    return s;
}

// COMMON,
Cat_swap::Cat_swap(const QString &heading, const QString &explain)
    : Category(heading, explain)
{
}

QString Cat_swap::string(Procinfo *p)
{
    QString s;
    // It can actually happen that size < resident (Sun under Solaris 2.6)
    // Possible with Linux ?

    long sizeK, sizeM;
#ifdef LINUX
    sizeK = p->swap;
#else
    sizeK = p->size > p->resident ? p->size - p->resident : 0;
#endif
    sizeM = sizeK / 1024;

    if (sizeM > 0)
        s = QString::number(sizeM) + "M";
    else
        s = QString::number(sizeK) + "K";

    if (sizeK == 0)
        s = "0";
    return s;
}

int Cat_swap::compare(Procinfo *a, Procinfo *b)
{
#ifdef LINUX
    return b->swap - a->swap;
#else
    return (b->size - b->resident) - (a->size - a->resident);
#endif
}

Cat_string::Cat_string(const QString &heading, const QString &explain,
                       QString Procinfo::*member)
    : Category(heading, explain), str_member(member)
{
}

QString Cat_string::string(Procinfo *p) { return p->*str_member; }

Cat_user::Cat_user(const QString &heading, const QString &explain)
    : Cat_string(heading, explain)
{
}

QString Cat_user::string(Procinfo *p)
{
    if (p->uid == p->euid)
        return Uidstr::userName(p->uid);
    else
    {
        QString s = Uidstr::userName(p->uid);
        s.append(p->euid == 0 ? "*" : "+");
        return s;
    }
}

Cat_group::Cat_group(const QString &heading, const QString &explain)
    : Cat_string(heading, explain)
{
}

QString Cat_group::string(Procinfo *p)
{
    if (p->gid == p->egid)
        return Uidstr::groupName(p->gid);
    else
    {
        QString s = Uidstr::groupName(p->gid);
        s.append("*");
        return s;
    }
}

Cat_wchan::Cat_wchan(const QString &heading, const QString &explain)
    : Cat_string(heading, explain)
{
}

QString Cat_wchan::string(Procinfo *p)
{
#ifdef LINUX
    return p->wchan_str;
#else
    return Wchan::name(p->wchan);
#endif
}

Cat_cmdline::Cat_cmdline(const QString &heading, const QString &explain)
    : Cat_string(heading, explain)
{
}

QString Cat_cmdline::string(Procinfo *p)
{
    if (p->cmdline.isEmpty())
    {
        QString s("(");
        s.append(p->command);
        s.append(")");
        return s;
    }
    else
    {
        if (Procview::flag_show_file_path)
            return p->cmdline;
        else
        {
            QString s(p->cmdline);

            int i = s.indexOf(' ');
            if (i < 0)
                i = s.length();
            if (i > 0)
            {
                i = s.lastIndexOf('/', i - 1);
                if (i >= 0)
                    s.remove(0, i + 1);
            }
            return s;
        }
    }
}

// hmm COMMON? almost same...but Solaris's zombie process...
Cat_start::Cat_start(const QString &heading, const QString &explain)
    : Category(heading, explain)
{
}
// don't let your confidence be shaken, just learn what you can from this young
// master.
QString Cat_start::string(Procinfo *p)
{
#ifdef SOLARIS
    if (p->state == 'Z')
        return "-"; // Solaris zombies have no valid start time
#endif
    Proc *proc = p->proc;
    //	time_t start = boot_time + p->starttime / (unsigned)HZ;
    QString s;
    // time_t start = proc->boot_time + p->starttime / proc->clk_tick;
    time_t start = p->starttime;
    char *ct = ctime(&start);                    // ctime(sec)
    time_t sec = p->starttime - proc->boot_time; // secs
    // if(p->tv.tv_sec - start < 86400) {
    if (sec < 86400)
    { // 24hours
        ct[16] = '\0';
        s = ct + 11; // Hour:minu
    }
    else
    {
        ct[10] = '\0';
        s = ct + 4; // Date
    }
    return s;
}

int Cat_start::compare(Procinfo *a, Procinfo *b)
{
    unsigned long bs = b->starttime, as = a->starttime;
    return bs >= as ? (bs == as ? 0 : 1) : -1;
}

/* ========================  Procview ============================  */
float Procview::avg_factor = 1.0;

Procview::Procview()
{
    // Proc::Proc(); //call once more ?
    cats.clear();
    reversed = false;
    viewproc = ALL;
    treeview = false;
    idxF_CMD = 0; // **** important ****
    viewfields = USER;
    set_fields();

    sortcat = nullptr; // categories[F_PID];
    sort_column = -1;
    sortcat_linear = NULL;
    enable = true;
    maxSizeHistory = 1200;
    refresh(); // before reading Watchdog list
}

// COMMON:
// Description :
// 		View mode : ALL , OWNER, NO-ROOOT , HIDDEN, NETWORK
bool Procview::accept_proc(Procinfo *p)
{
    QString pid;
    static int my_uid = getuid();
    bool result;

    result = true;

    // BAD
    if (false and viewproc == NETWORK)
    {
        /*
        p->read_fds();
        if(p->sock_inodes.size()==0)
                result=false;
        for(int i=0;i<p->sock_inodes.size();i++)
        {
                SockInode *sn=p->sock_inodes[i];
                Sockinfo *si=Procinfo::socks.value(sn->inode,NULL);
                if(si)
                {
                        si->pid=p->pid;
                        linear_socks.append(si);
                }
        }
        */
    }
    else if (viewproc == ALL)
        result = true;
    else if (viewproc == OWNED)
        result = (p->uid == my_uid);
    else if (viewproc == NROOT)
        result = (p->uid != 0);
    else if (viewproc == RUNNING)
        result = strchr("ORDW", p->state) != 0;

    /*
    if ( viewproc == HIDDEN)
    {
            result=false;
            for(int j=0;j<hidden_process.size();j++)
                    if(hidden_process[j]==p->command)
                            result=true ;
    }
    else
    {
            for(int j=0;j<hidden_process.size();j++)
                    if(hidden_process[j]==p->command)
                            result=false;
    } */

    if (result == false)
        return false; // dont go further !! for better speed

    /// if(search_box==NULL)		return result;
    if (filterstr == "")
        return result;

    if (filterstr == "*")
        return true;

    // Notice:
    //  1. some process name maybe different CMDLINE.  ex) Xorg
    if (p->cmdline.contains(filterstr,
                            Qt::CaseInsensitive)) // search_box->text()
        return true;
    if (p->command.contains(filterstr, Qt::CaseInsensitive))
        return true;
    if (p->username.contains(filterstr, Qt::CaseInsensitive))
        return true;

    pid = pid.setNum(p->pid); //=QString::number(p->pid);
    if (pid.contains(filterstr, Qt::CaseInsensitive))
        return true;

    /// printf("search_Box =%s , %s
    /// \n",search_box->text().toAscii().data(),p->command.toAscii().data());

    return false;
}

extern "C" {
typedef int (*compare_func)(const void *, const void *);
}

/*
template<class T>
void Svec<T>::sort(int (*compare)(const T *a, const T *b))
{
    qsort(vect, used, sizeof(T), (compare_func)compare);
}
*/

// table view - sort
void Procview::linearize_tree(QVector<Procinfo *> *ps, int level, int prow,
                              bool hide)
{
    static_sortcat = sortcat;
    // ps->sort(reversed ? compare_backwards : compare);
    if (reversed)
        qsort(ps->data(), ps->size(), sizeof(Procinfo *),
              (compare_func)compare_backwards);
    else
        qsort(ps->data(), ps->size(), sizeof(Procinfo *),
              (compare_func)compare);

    int size = ps->size();
    // printf("level=%d prow=%d size=%d\n",level,prow,size);
    for (int i = 0; i < size; i++)
    {
        Procinfo *p = (*ps)[i];
        //	if (p->pid<6 )	printf("pid=%d level=%d
        // parent_row=%d\n",p->pid,p->pid,p->parent_row);
        p->level = level;
        p->lastchild = false;
        p->table_child_seq = i; // ************* where using ? -> sequence
        p->parent_row = prow;
        if (!hide)
            linear_procs.append(p);   // need !!
        if (p->table_children.size()) // and !p->hidekids)
            linearize_tree(&p->table_children, level + 1,
                           linear_procs.size() - 1, hide | p->hidekids);
    }

    if (size > 0)
        (*ps)[size - 1]->lastchild = true;
}

/// basic,memory,job fields
void Procview::set_fields_list(int fields[])
{
    cats.clear();
    for (int i = 0; fields[i] != F_END; i++)
    {
        if (fields[i] == F_CPUNUM && Proc::num_cpus < 2)
            continue; // does not work correctly

        Category *c = categories.value(fields[i], nullptr);
        if (c)
            cats.append(c);
    }
}

// return the column number of a field, or -1 if not displayed
int Procview::findCol(int field_id)
{
    for (int i = 0; i < cats.size(); i++)
        if (cats[i]->id == field_id)
            return i;
    return -1;
}

// basis
// returns the place where the field is added
// (-1 if it already exists)
// called by
// 	void Procview::fieldArrange();
// 	qps();
//
int Procview::addField(int Fid, int where)
{
    if (where < 0 || where > cats.size())
    {
        // this is default
        where = cats.size();
    }

    if (Fid == F_CMDLINE)
        where = cats.size(); // always should be the last column
    else if (Fid == F_PID)
    {
        if (treeview == true)
            where = 0; // always should be the first column with the tree
        else if (!cats.isEmpty() && where == cats.size()
                 && cats[cats.size() - 1]->index == F_CMDLINE)
        {
            -- where;
        }
    }
    else if (!cats.isEmpty())
    {
        if (where == cats.size())
        {
            if (cats[cats.size() - 1]->index == F_CMDLINE)
              -- where;
        }
        else if (where == 0 && treeview == true && cats[0]->index == F_PID)
            where = 1;
    }

    Category *newcat = categories[Fid];
    if (cats.indexOf(newcat) < 0) // if not in the list
    {
        cats.insert(where, newcat);
        return where;
    }
    return -1; // nothings is added
}

// add a category to last by name
void Procview::addField(char *name)
{
    int id = field_id_by_name(name);
    if (id >= 0)
        addField(id); // add to last
}

void Procview::removeField(int field_id)
{
    for (int i = 0; i < cats.size();)
    {
        if (cats[i]->id == field_id)
        {
            cats.remove(i);
            break;
        }
        else
            i++;
    }
}

//	called by
//		1. write_settings()
//	DEL? -> not yet
void Procview::update_customfield()
{
    customfields.clear();

    for (int i = 0; i < cats.size(); i++)
    {
        customfields.append(cats[i]->name);
        customFieldIDs.append(cats[i]->id);
    }
}

// Description : FIELD movement by mouse drag
//				  From col To place
void Procview::moveColumn(int col, int place)
{
    int s = cats.size();
    if (col < 0 || place < 0 || s <= col || s <= place)
    {
        printf("QPS code bugs!! : moveColumn() col=%d place=%d "
               "cats.size=%d\n",
               col, place, s);
        return;
    }
    if (col == place)
        return;

    if (cats[col]->index == F_PID)
    { // PID should always be the first field in the tree model
        if (treeview == true)
            place = 0;
    }
    else if (cats[col]->index == F_CMDLINE)
        place = s; // COMMAND_LINE should always be the last field

    // first insert it into the new place; then remove it from the old one
    Category *cat = cats[col];
    if (place < col)
        ++col;
    else// if (place > col)
        place = qMin(place + 1, s);
    cats.insert(place, cat);
    cats.remove(col);
}

// always called when linear to tree
// DEL
void Procview::saveCOMMANDFIELD() {}

// call by
//		void Pstable::moveCol(int col, int place)
// 		void Pstable::setTreeMode(bool treemode)
//		void Procview::set_fields()
//	TODO: checkField();
void Procview::fieldArrange()
{
    int s = cats.size();

    // PID should always be the first field in the tree model
    if (treeview == true && cats[0]->index != F_PID)
    {
        bool found = false;
        for (int i = 1; i < s; i++)
        {
            if (cats[i]->index == F_PID)
            {
                found = true;
                moveColumn(i, 0);
                break;
            }
        }
        if (!found)
            addField(F_PID, 0);
    }

    // COMMAND_LINE should always be the last field
    if (cats[s - 1]->index != F_CMDLINE)
    {
        for (int i = 0; i < s - 1; i++)
        {
            if (cats[i]->index == F_CMDLINE)
            {
                moveColumn(i, 0); // will go to the end; see Procview::moveColumn
                break;
            }
        }
    }
}

void Procview::setTreeMode(bool /*b*/)
{
    if (treeview == false)
    {
        if (sortcat_linear == NULL)
            sortcat_linear = sortcat;
        else
            sortcat = sortcat_linear;
    }
}

void Procview::setSortColumn(int col, bool keepSortOrder)
{
    if (col < 0 || col >= cats.size()) // restore defaults
    {
        if (col >= 0)
            qDebug("hmmm error? col>=cats.size() %d", col);
        sort_column = -1;
        sortcat = nullptr;
        reversed = false;
        return;
    }

    // if(!procview->treeview) ?
    if (!keepSortOrder)
    {
        if (col == sort_column)
            reversed = !reversed;
        else
            reversed = false;
    }
    sortcat = cats[col];
    sort_column = col;
}

Category *Procview::static_sortcat = 0;
int Procview::compare(Procinfo *const *a, Procinfo *const *b)
{
    if (static_sortcat == 0)
        return 0;
    int r = static_sortcat->compare(*a, *b);
    return (r == 0) ? ((*a)->pid > (*b)->pid ? 1 : -1) : r;
}

int Procview::compare_backwards(Procinfo *const *a, Procinfo *const *b)
{
    if (static_sortcat == 0)
        return 0;
    int r = static_sortcat->compare(*b, *a);
    return (r == 0) ? ((*b)->pid > (*a)->pid ? 1 : -1) : r;
}

// COMMON
Category *Proc::cat_by_name( const QString &s )
{

    if ( ! s.isNull() )
    {
        // java style
        QHashIterator<int, Category *> i(categories);
        while (i.hasNext())
        {
            i.next();
            const QString &p = i.value()->name;
            int index = p.indexOf( QRegExp( "\\S" ) );
            if ( p.indexOf( s, index ) == index )
                return i.value();
            //     cout << i.key() << ": " << i.value() << endl;
        }
    }
    return 0;
}

// COMMON
// call by
int Proc::field_id_by_name(const QString &s)
{
    if ( ! s.isNull() )
    {
        // STL style
        QHash<int, Category *>::iterator i = categories.begin();
        while (i != categories.end())
        {
            if ( i.value()->name == s )
                return i.key(); // cout << i.key() << ": " <<
                                // i.value() << endl;
            ++i;
        }
    }
    return -1;
}

// postInit
void Proc::commonPostInit()
{
    QHash<int, Category *>::iterator i = categories.begin();
    while (i != categories.end())
    {
        i.value()->index = i.key();
        i.value()->id = i.key();
        ++i;
    }

    num_opened_files = 0;    // test
    num_process = 0;         //	64bit
    num_network_process = 0; // 64bit

    dt_total = 0; // diff system tick
    dt_used = 0;  // for infobar

    loadavg[0] = loadavg[1] = loadavg[2] =
        0.0; // CPU load avg 1min,5min,15minute

    Proc::num_cpus = 0;
    Proc::old_num_cpus = 0;

    Proc::mem_total = 0;
    Proc::mem_free = 0;

    Proc::mem_buffers = 0;
    Proc::mem_cached = 0;

    Proc::swap_total = 0;
    Proc::swap_free = 0;

    Proc::qps_pid = -1;
    Proc::loadQps = 0.0;

    Proc::cpu_times_vec = 0; // array.
    Proc::old_cpu_times_vec = 0;

    Proc::boot_time = 0;
    Proc::clk_tick = 100; // for most system

    current_gen = 0; // !
    mprocs = NULL;
    hprocs = NULL;

    clk_tick =
        sysconf(_SC_CLK_TCK); //****** The  number  of  clock ticks per second.
                              //
                              // printf("Qps: hz=%d\n",clk_tick);

    ///	Procinfo::init_static();
}

// COMMON?
// 	called by Procview::rebuild()
// 	using procs, makes root_procs for TABLE_View
void Procview::build_tree(Proclist &procs)
{
    Procinfo *p;
    int proc_n = 0;

    // children clear
    root_procs.clear();

    QHash<int, Procinfo *>::iterator i;
    for (i = procs.begin(); i != procs.end();)
    {
        Procinfo *p = i.value();
        p->table_children.clear();
        if ((p->accepted = accept_proc(p)))
            proc_n++;
        ++i;
    }

    Proc::num_process = proc_n; // count process

    // find parent of a process
    for (i = procs.begin(); i != procs.end(); ++i)
    {
        p = i.value(); // always not NULL

        if (p->accepted)
        {
            Procinfo *parent = 0;
            int virtual_parent_pid;

            if (p->isThread())
                virtual_parent_pid = p->tgid; // thread's leader ID.
            else
                virtual_parent_pid = p->ppid;

            if (p->pid < p->ppid)
            {
                // this occurs !! a reporter mailed to me
            }

            parent = procs.value(virtual_parent_pid,
                                 NULL); // if pid not found, then return NULL.

            //	printf("thread_leader=0 (%d)
            //%s\n",p->tgid,p->command.ascii());
            if (treeview and parent and parent->accepted)
            {
                // p->table_child_seq=parent->table_children.size();
                parent->table_children.append(p);
            }
            else
            {
                // 1.init(pid=1) process
                // 2.some process which parent not accepted
                // 3.(null) thread has TGID=0,PPID=0
                // 4.when not tree mode
                root_procs.append(p);
            }
        }
    }
}

// COMMON
// BOTTLENECK No.3   0.5%
// re-sort the process list , ::rebuild() for Table
// called by Pstable::refresh()
void Procview::rebuild()
{
    linear_procs.clear(); // Svec<Procinfo *> procs in Proview
#ifdef LINUX
    linear_socks.clear();
#endif

    // Procinfo *pi = getProcinfoByPID(Procinfo::qps_pid);
    // if(pi) Procinfo::loadQps=pi->pcpu;
    //	printf("rebuild\n");

    /* if(mprocs)
    {
            build_tree(*mprocs);
    }
    else */
    build_tree(Proc::procs);

    linearize_tree(&root_procs, 0, -1);
}

// COMMON: CORE
// Description: update the process list      BottleNeck 1.5%
// 		read /proc/*
// 		called by Procview::refresh(), every UPDATE .
void Proc::refresh()
{
    current_gen++;

    // TEST for Process History
    SysHistory *s = new SysHistory;

    history.append(s);

    if(hprocs)
    {
        qDeleteAll(hprocs->begin(), hprocs->end());
        hprocs->clear();
    }
    hprocs = &(s->procs);

    // init
    /// num_opened_files=0;
    Proc::num_process = 0;
    Proc::num_network_process = 0;

    Proc::read_loadavg();
    Proc::read_system(); // **** should be every refresh !!
    // s->load_cpu=(float)Proc::dt_used/Proc::dt_total;  //after
    // read_system();

    s->load_cpu = load_cpu; // after read_system();
    s->time = time(NULL);   // save current time in seconds since epoch

    // TODO: clean
    // Procinfo::read_sockets(); // for future, BottleNect 2%
    read_byte = 0;
    write_byte = 0;
    io_byte = 0; //!!!
                 ///	Proc::read_sockets(); // test

    read_proc_all(); // Linux, Solaris...

    // s->load_io=(float)(read_byte + write_byte ); // (50*1024*1024);
    // //dt_total;
    if (io_byte != 0)
    {
        s->load_io = log10(io_byte) * 2; // 9000 -> 3point
        //	printf("DEBUG: rw=[%d,%d] %d, %f\n",read_byte,
        // write_byte,io_byte,s->load_io);
    }
    else
        s->load_io = 0;

    // remove non-existing processes, remove Procinfos of nonexisting
    // processes
    QHash<int, Procinfo *>::iterator i;
    for (i = procs.begin(); i != procs.end();)
    {
        Procinfo *p = i.value();
        if (p->generation != current_gen)
        {
            //			printf("delete %d\n",p->pid);
            i = procs.erase(i);
            delete p;
        }
        else
        {
            //	p->table_children.clear();  // for rebuild()
            //	p->accepted=accept_proc(p);
            ++i;
        }
    }

    //	TESTING
    while (history.size() >= maxSizeHistory)
    {
        // if(history.isEmpty()==false)
        delete history.takeFirst();
    }
}

// COMMON
// read new process info
void Procview::refresh()
{
    /****************************************************************/
    /* Procview.procs has the procinfo
     */
    /****************************************************************/
    if (enable)
    {
        // printf("Procview::refresh()\n");
        Proc::refresh(); // read "/proc/*", then update the process list
    }
}

SysHistory::~SysHistory()
{
    for (const auto *p : qAsConst(procs))
    {
        delete p;
    }
    procs.clear(); // remove all (key,val)
}

/* check
Category *Proc::cat_by_name(const char *s)
{
    if( s )
    {
        for( int i = 0; i < categories.size(); i++ )
            if( strcmp(categories[i]->name, s) == 0 )
                return categories[i];
    }
    return 0;
}


int  Proc::field_id_by_name(const char *s)
{
    if( s )
    {
        for( int i = 0; i < categories.size(); i++ )
            if( strcmp(categories[i]->name, s) == 0 )
                return i;
    }
    return -1;
}
*/
