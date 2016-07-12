// details.cpp
//
// This program is free software. See the file COPYING for details.
// Author: Mattias Engdegård, 1997-1999

#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>

#include "details.h"
//#include "proc.h" // static flag
#include "qps.h" // static flag
Details::Details(Procinfo *p, Proc *proc) : QWidget(0)
{
    QString cap;
    pi = p;
    pi->detail = this;
    pr = proc;
    //	printf("pi=%x\n",pi);
    cap.sprintf("Process %d ( %s ) - details", pi->pid,
                qPrintable(pi->command));
    setWindowTitle(cap);

    tbar = new QTabWidget(this);
    // tbar->setMargin(5);

    // if(pi->fd_files) //if(pi->fd_files->size())
    if (pi->read_fds())
    {
        tbar->addTab(new Files(this), "&Files");

//	if(pi->read_fds()) // create sock_inodes
#ifdef LINUX
        // printf(" pi->socks.size=%d\n",  p->sock_inodes->size() );
        // this means if a process dont have socket then
        // no socket pane  show in Detail dialog.
        // Procinfo::read_sockets();
        if (pi->sock_inodes.size() != 0)
            tbar->addTab(new Sockets(this), "&Sockets");
#endif
    }

    if (pi->read_maps())
        tbar->addTab(new Maps(this), "&Memory Maps");
    if (pi->read_environ())
        tbar->addTab(new Environ(this), "&Environment");
    tbar->addTab(new AllFields(this), "&All Fields");

    tbar->adjustSize();
    QSize s0 = tbar->sizeHint();
    if (s0.width() > 800)
        s0.setWidth(800);
    resize(s0);
}

Details::~Details()
{
    //	printf("~Details()\n");
    if (pi)
        pi->detail = 0;
    int i;
    /// for(i=0;i<tbar->count();i++)	delete tbar->page(i);
    delete tbar;
}

void Details::set_procinfo(Procinfo *p)
{
    //	printf("p=%x, pi=%x\n",p,pi);
}
void Details::refresh()
{
    printf("Details::refresh()\n");
    QWidget *w = NULL; // tbar->currentPage ();
    // QWidget *w= tbar->currentPage ();
    if (w != NULL)
    {
        /// printf("refresh()\n");
        ((SimpleTable *)w)->refresh();
    }
}

void Details::process_gone()
{
    ///	printf("process_gone() *******************************\n");
    pi = 0;
    // for now, we just close the window. Another possibility would be
    // to leave it with a "process terminated" note.
    close();
}

// slot: react to changes in preferences
void Details::config_change() { return; }

void Details::resizeEvent(QResizeEvent *s)
{
    tbar->resize(s->size()); //**
    QWidget::resizeEvent(s);
}

// user closed the window (via window manager)
void Details::closeEvent(QCloseEvent *) { emit closed(this); }

SimpleTable::SimpleTable(QWidget *parent, int nfields, TableField *f,
                         int options)
    : HeadedTable(parent, options), fields(f)
{
    detail = (Details *)parent;
    setNumCols(nfields);
}

QSize SimpleTable::sizeHint() const
{
    /// return QSize(tableWidth(), 300);
    return QSize(480, 300);
}

QString SimpleTable::title(int col) { return fields[col].name; }

int SimpleTable::colWidth(int col) { return fields[col].width; }

inline int SimpleTable::alignment(int col) { return fields[col].align; }

int SimpleTable::leftGap(int col) { return fields[col].gap; }

QString SimpleTable::tipText(int col) { return fields[col].tooltip; }

#ifdef LINUX

// declaration of static members
bool Sockets::have_services = false;
QHash<int, char *> Sockets::servdict;
Lookup *Sockets::lookup = 0;

TableField Sockets::fields[] = {
    {"Fd", 5, 8, Qt::AlignRight, "File descriptor"},
    {"Proto", 4, 8, Qt::AlignLeft, "Protocol (TCP or UDP)"},
    {"Recv-Q", 9, 8, Qt::AlignRight, "Bytes in receive queue"},
    {"Send-Q", 9, 8, Qt::AlignRight, "Bytes in send queue"},
    {"Local Addr", -1, 8, Qt::AlignLeft, "Local IP address"},
    {"Port", 6, 8, Qt::AlignLeft, "Local port"},
    {"Remote Addr", -1, 8, Qt::AlignLeft, "Remote IP address"},
    {"Port", 6, 8, Qt::AlignLeft, "Remote port"},
    {"State", 18, 8, Qt::AlignLeft, "Connection state"}};

Sockets::Sockets(QWidget *parent) : SimpleTable(parent, SOCKFIELDS, fields)
{
    if (!lookup)
        lookup = new Lookup();
    connect(lookup, SIGNAL(resolved(unsigned)),
            SLOT(update_hostname(unsigned)));
    doing_lookup = Qps::hostname_lookup;
    refresh();
    // compute total width = window width
    int totw = 400;
    // for(int i = 0; i < SOCKFIELDS; i++)
    //	totw += actualColWidth(i);
    resize(totw, 200);
}

Sockets::~Sockets()
{
    // why seg?
    // if(lookup)	delete lookup;
}

static const char *tcp_states[] = {
    "ESTABLISHED", // 1
    "SYN_SENT",    "SYN_RECV",   "FIN_WAIT1", "FIN_WAIT2", "TIME_WAIT",
    "CLOSE",       "CLOSE_WAIT", "LAST_ACK",  "LISTEN",
    "CLOSING" // 11
};

static inline const char *tcp_state_name(int st)
{
    return (st < 1 || st > 11) ? "UNKNOWN" : tcp_states[st - 1];
}

QString Sockets::text(int row, int col)
{
    Procinfo *p = procinfo();
    if (p->sock_inodes.size() == 0)
        refresh_sockets();
    SockInode *sock_ino = p->sock_inodes[row];
    // SockInode *sock_ino = p->sock_inodes[row];
    Sockinfo *si = p->proc->socks.value(sock_ino->inode, NULL);
    if (!si)
        return ""; // process gone, return empty string

    QString s;
    switch (col)
    {
    case FD:
        s.setNum(sock_ino->fd);
        break;

    case PROTO:
        s = (si->proto == Sockinfo::TCP) ? "tcp" : "udp";
        break;

    case RECVQ:
        s.setNum(si->rx_queue);
        break;

    case SENDQ:
        s.setNum(si->tx_queue);
        break;

    case LOCALADDR:
        s = ipAddr(si->local_addr);
        break;

    case LOCALPORT:
        if (Qps::service_lookup)
        {
            const char *serv = servname(si->local_port);
            if (serv)
            {
                s = serv;
                break;
            }
        }
        s.setNum(si->local_port);
        break;

    case REMOTEADDR:
        s = ipAddr(si->rem_addr);
        break;

    case REMOTEPORT:
        if (Qps::service_lookup)
        {
            const char *serv = servname(si->rem_port);
            if (serv)
            {
                s = serv;
                break;
            }
        }
        s.setNum(si->rem_port);
        break;

    case STATE:
        s = tcp_state_name(si->st);
        break;
    }
    return s;
}

QString Sockets::ipAddr(unsigned addr)
{
    unsigned a = htonl(addr);
    QString s;
    if (doing_lookup)
    {
        s = lookup->hostname(addr);
        if (s.isNull())
        {
            s.sprintf("(%d.%d.%d.%d)", (a >> 24) & 0xff, (a >> 16) & 0xff,
                      (a >> 8) & 0xff, a & 0xff);
        }
    }
    else
    {
        if (a == 0)
            s = "*";
        else
            s.sprintf("%d.%d.%d.%d", (a >> 24) & 0xff, (a >> 16) & 0xff,
                      (a >> 8) & 0xff, a & 0xff);
    }
    return s;
}

void Sockets::refresh_window()
{
    Procinfo *p = procinfo();
    if (!p)
        return;
    int rows = p->sock_inodes.size();
    /// resetWidths();
    setNumRows(rows);
    setNumCols(SOCKFIELDS);
    /// repaint_changed();
    repaintAll();
}

void Sockets::refresh()
{
    if (refresh_sockets())
        refresh_window();
}

// return true if sockets could be read successfully, false otherwise
bool Sockets::refresh_sockets()
{
    //	Procinfo::read_sockets();
    return procinfo()->read_fds();
}

// react to changes in preferences
void Sockets::config_change()
{
    if (doing_lookup != Qps::hostname_lookup)
    {
        doing_lookup = Qps::hostname_lookup;
        ////setNumCols(SOCKFIELDS);
        // for(int col = 0; col < SOCKFIELDS; col++)
        //	widthChanged(col);
        ////updateTableSize();
        ////repaintAll();
    }
}

// slot: called when a host name has been looked up
void Sockets::update_hostname(unsigned addr)
{
    // if(widthChanged(REMOTEADDR) || widthChanged(LOCALADDR)) {
    if (1)
    {
        ////	updateTableSize();
        ///	repaintAll();
    }
    else
    {
        // just repaint some rows
        Procinfo *p = procinfo();
        if (!p->sock_inodes.size())
        {
            /// Procinfo::read_sockets();
            p->read_fds();
        }
        int rows = p->sock_inodes.size();
        for (int i = 0; i < rows; i++)
        {
            int inode = p->sock_inodes[i]->inode;
            /// Sockinfo *si = Procinfo::socks[inode];
            /// if(si->local_addr == addr) updateCell(i, LOCALADDR);
            /// if(si->rem_addr == addr)	updateCell(i,
            /// REMOTEADDR);
        }
    }
}

const char *Sockets::servname(unsigned port)
{
    if (!have_services)
    {
        have_services = true;
        // fill servdict from /etc/services (just try once)
        setservent(1);
        struct servent *s;
        while ((s = getservent()) != 0)
        {
            unsigned short hport = ntohs((unsigned short)s->s_port);
            if (!servdict.value(hport, NULL))
            {
                servdict.insert(hport, strdup(s->s_name));
            }
        }
        endservent();
    }
    return servdict.value(port, NULL);
}

#endif // LINUX

#ifdef SOLARIS

// Stupid code to make up for moc:s inability to grok preprocessor conditionals
void Sockets::refresh() {}
QString Sockets::text(int, int) { return 0; }
void Sockets::config_change() {}
Sockets::~Sockets() {}
void Sockets::update_hostname(unsigned int) {}

#endif

TableField Maps::fields[] = {
    {"Address Range", -1, 8, Qt::AlignLeft, "Mapped addresses (hex)"},
    {"Size", 8, 8, Qt::AlignRight, "Kbytes mapped (dec)"},
    {"Perm", 5, 8, Qt::AlignLeft, "Permission flags"},
    {"Offset", -1, 8, Qt::AlignRight, "File offset at start of mapping (hex)"},
    {"Device", 8, 8, Qt::AlignLeft, "Major,Minor device numbers (dec)"},
    {"Inode", 10, 8, Qt::AlignRight, "Inode number (dec)"},
    {"File", -9, 8, Qt::AlignLeft, "File name (if available)"}};

// memory leak
Maps::Maps(QWidget *parent) : SimpleTable(parent, MAPSFIELDS, fields)
{
    // monospaced font looks best in the table body since it contains
    // hex numerals and flag fields. Pick Courier (why not)
    body->setFont(QFont("Courier", font().pointSize()));
    bool mono = true;
    QFont f = font();
    if (f.rawMode())
    {
        /* see if the font is monospaced enough for our needs */
        QFontMetrics fm(f);
        int zw = fm.width('0');
        const char *test = "abcdef";
        for (const char *p = test; *p; p++)
            if (fm.width(*p) != zw)
            {
                mono = false;
                break;
            }
    }
    else
        mono = f.fixedPitch();
    if (mono)
    {
        ////setBodyFont(f);
    }
    else
    {
        int ps = f.pointSize();
        /// setBodyFont(QFont("Courier", ps ? ps : 10));
    }

    refresh();
    // compute total width = window width
    int totw = 300;
    // for(int i = 0; i < MAPSFIELDS; i++)	totw += actualColWidth(i);
    resize(totw + 20, 200);
}

Maps::~Maps()
{
    //	if(maps)
    //		maps->clear();
}

QString Maps::text(int row, int col)
{
    Procinfo *p = procinfo();
    if (p->maps.size() == 0)
    {
        refresh_maps();
        if (p->maps.size() == 0)
            return "";
    }
    Mapsinfo *mi = p->maps[row];

    QString s;
    char buf[80];
    switch (col)
    {
    case ADDRESS:
        sprintf(buf, (sizeof(void *) == 4) ? "%08lx-%08lx" : "%016lx-%016lx",
                mi->from, mi->to);
        s = buf;
        break;
    case SIZE:
        s.setNum((mi->to - mi->from) >> 10);
        s += "kb";
        break;
    case PERM:
        s = "    ";
        for (int i = 0; i < 4; i++)
            s[i] = mi->perm[i];
        break;
    case OFFSET:
        sprintf(buf, (sizeof(void *) == 4) ? "%08lx" : "%016lx", mi->offset);
        s = buf;
        break;
    case DEVICE:
        s.sprintf("%2u,%2u", mi->major, mi->minor);
        break;
    case INODE:
        s.setNum(mi->inode);
        break;
    case FILENAME:
        s = mi->filename;
        if (!Qps::show_file_path)
        {
            int i = s.lastIndexOf('/');
            if (i >= 0)
                s.remove(0, i + 1);
        }
        break;
    }
    return s;
}

void Maps::refresh_window()
{
    if (!procinfo())
        return;
    int rows = procinfo()->maps.size();
    ////resetWidths();
    setNumRows(rows);
    setNumCols(MAPSFIELDS);
    repaintAll();
}

void Maps::refresh()
{
    if (refresh_maps())
        refresh_window();
}

bool Maps::refresh_maps() { return procinfo()->read_maps(); }

TableField Files::fields[] = {
    {"Fd", 5, 8, Qt::AlignRight, "File descriptor"},
#ifdef LINUX
    {"Mode", 3, 8, Qt::AlignLeft, "Open mode"},
#endif
    {"Name", -1, 8, Qt::AlignLeft, "File name (if available)"}};

Files::Files(QWidget *parent) : SimpleTable(parent, FILEFIELDS, fields)
{
    // compute total width = window width
    refresh_window();
}

Files::~Files() {}

void Files::refresh()
{
    printf("Files::refresh()\n");
    // return true if fds could be read successfully, false otherwise
    if (procinfo()->read_fds())
        refresh_window();
}

bool Files::refresh_fds() { return false; }

void Files::refresh_window()
{
    Procinfo *p = procinfo();
    if (!p)
        return;

    // if(p->fd_files==NULL) printf("qps :dddds\n");

    int rows = p->fd_files.size();

    resetWidths();
    // printf("size=%d\n",rows);
    setNumRows(rows);
    setNumCols(FILEFIELDS);
    repaintAll(); // useless ?
}

QString Files::text(int row, int col)
{
    Procinfo *p = procinfo(); // alot!!
    if (p == 0)
        return "zero";
    //	printf("p=%x px=%x\n",p,px);
    if (p->fd_files.size() == 0)
    { //???????????///////
        refresh_fds();
        if (p->fd_files.size() == 0)
            return "";
    }
    if (row >= p->fd_files.size())
        return "";

    Fileinfo *fi = p->fd_files[row];
    QString s;
    switch (col)
    {
    case FILEDESC:
        s.setNum(fi->fd);
        break;

#ifdef LINUX
    case FILEMODE:
        if (fi->mode & OPEN_READ)
            s.append("R");
        if (fi->mode & OPEN_WRITE)
            s.append("W");
        break;
#endif

    case FILENAME:
        s = fi->filename;
        break;
    }
    return s;
}

TableField Environ::fields[] = {
    {"Variable", -1, 8, Qt::AlignLeft, "Variable name"},
    {"Value", -1, 8, Qt::AlignLeft, "Variable value"}};

Environ *Environ::static_env = 0;
Environ::Environ(QWidget *parent)
    : SimpleTable(parent, ENVFIELDS, fields), rev(false)
{
    connect(this, SIGNAL(titleClicked(int)), SLOT(sort_change(int)));
    refresh();
}

Environ::~Environ() {}

QString Environ::text(int row, int col)
{
    Procinfo *p = procinfo(); // if dead process then
    if (row >= p->environ.size())
        printf("size dddd=row=%d\n", row);
    NameValue nv = p->environ[row]; // Segfault !!
    return (col == ENVNAME) ? nv.name : nv.value;
}

void Environ::refresh_window()
{
    if (!procinfo())
        return;
    /// resetWidths();
    int rows = procinfo()->environ.size();
    setNumRows(rows);
    setNumCols(ENVFIELDS);
    repaintAll();
}

void Environ::refresh()
{
    if (procinfo()->read_environ())
    {
        // sort();
        refresh_window();
    }
}

void Environ::sort_change(int col)
{
    Procinfo *p = procinfo();
    /*
    if(!p->environ) {
            refresh_environ();
            if(!p->environ)
                    return;
    }*/
    ////rev = (col == sortedCol()) ? !rev : false;
    setSortedCol(col);
    sort();
    refresh_window();
}

// sort table according to current settings
void Environ::sort()
{
    /////if(sortedCol() >= 0)
    {
        static_env = this;
        //	if(procinfo()->environ==NULL)
        //		printf("qps : Environ::sort() error ???\n");
        //	else
        //	procinfo()->environ.sort(compare);
    }
}

int Environ::compare(const NameValue *a, const NameValue *b)
{
    Environ *e = Environ::static_env;
    int r;
    /*
    if(e->sortedCol() == ENVNAME)
            r = strcmp(a->name, b->name);
    else
            r = strcmp(a->value, b->value);  SimpleTable(QWidget *parent,
    int
    nfields, TableField *f, int options = 0);
  */
    return e->rev ? -r : r;
}

TableField AllFields::fields[] = {
    {"Field", -1, 8, Qt::AlignLeft, "Field name"},
    {"Description", -1, 8, Qt::AlignLeft, "Field description"},
    {"Value", -1, 8, Qt::AlignLeft, "Field value"}};

AllFields::AllFields(QWidget *parent)
    : SimpleTable(parent, FIELDSFIELDS, fields)
{
    refresh();
    // compute total width = window width
    int totw = 0;
    //	for(int i = 0; i < FIELDSFIELDS; i++) totw += actualColWidth(i);
    resize(totw + 20, 200);
}

AllFields::~AllFields() {}

// Proc not changed ?
QString AllFields::text(int row, int col)
{
    QString s;
    // printf("text start r=%d , c=%d\n",row,col);
    // if( ((Details *)parent())->proc()==NULL)
    // printf("size=%d\n", ((Details *)parent())->proc()->allcats.size() );
    // printf("size=%d\n", proc()->allcats.size() );

    Category *cat = proc()->categories.values()[row];
    //	Category *cat = proc()->allcats[row];
    switch (col)
    {
    case FIELDNAME:
        s = cat->name;
        break;
    case FIELDDESC:
        s = cat->help;
        break;
    case FIELDVALUE:
        s = cat->string(procinfo());
        break;
    }
    // printf("text end\n");
    return s;
}

// parent will be  tbar(TabWidget) !!!
void AllFields::refresh_window()
{
    // printf("refresh_window\n");
    if (!procinfo())
        return;
    setNumRows(proc()->categories.size());
    setNumCols(FIELDSFIELDS);
    // DEL resetWidths();
    // repaint_changed();
    repaintAll();
}

void AllFields::refresh() { refresh_window(); }
