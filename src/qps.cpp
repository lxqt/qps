/*
 * qps.cpp
 * This file is part of qps -- Qt-based visual process status monitor
 *
 * Copyright 1997-1999 Mattias Engdegård
 * Copyright 2005-2012 fasthyun@magicn.com
 * Copyright 2015-     daehyun.yang@gmail.com
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

// TODO & MEMO
/*
 *
 * For easy maintance
 *   1. F_CMDLINE should be rightmost field
 *   2. F_PROCESSNAME shoud be leftmost field  and  NEVER removed!
 *
 */

// . Memory Hole or Qt's string allocate bug?
// . clipboard copy [CMDLINE,PID,USERNAME...]
// . [klog] table cache.
// . save sort of tree, linear
// . watchdog showmsg substitude
// . history system
// . cmd [w] analsys: [IDLE] [WHAT]
// . ExecWindow - [running] finish.
// . UNIX Domain SOCKET
// . COLOR : orange FF5d00
// . P_MEM  -> P_%MEM or P_PMEM

#include "../icon/icon.xpm"

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/utsname.h> // uname
#include <csignal>
#include <cerrno>
#include <sched.h>
#include <unistd.h> //for sleep

#include "qps.h"
#include "qpsapp.h"
#include "dialogs.h"
#include "watchcond.h"
#include "lookup.h"
#include "misc.h"

#include <QBitmap>
#include <QTimerEvent>
#include <QVBoxLayout>
#include <QShortcut>
#include <QMenuBar>
#include <QFont>
#include <QFontComboBox>
#include <QToolTip>
#include <QTabWidget>
#include <QApplication>
#include <QCheckBox>
#include <QToolButton>
#include <QMessageBox>
#include <QSessionManager>
#include <QTextEdit>
#include <QSystemTrayIcon>
#include <QSplitter>
#include <QDockWidget>
#include <QDialog>
#include <QTextBrowser>
#include <QDesktopServices>
#include <QDialogButtonBox>
#include <QClipboard>
#include <QSettings>
#include <QTextCodec>

#include <iostream>

/* --------------------- Global Variable START ---------------------- */
bool previous_flag_show_thread_prev = false; // previous state
bool flag_show_thread = false; // to see informations at the thread level
bool flag_thread_ok = true;     // we presume a kernel 2.6.x using NPTL
bool flag_start_mini = false; // Need for Xorg.Session
bool flag_refresh = true;     // DEL
bool flag_schedstat = false;
bool flag_firstRun = true; // test

Qps *qps;
// ControlBar 		*controlbar=NULL;
SearchBox *search_box = nullptr;
TFrame *infobox = nullptr; // testing
QFontComboBox *font_cb = nullptr;

QList<Procview *> proclist;

#include "trayicon.h"
TrayIcon *trayicon = nullptr;

// for Non-ASCII Languages (CJK:chinese,japanese,korean, arabic ...)
QTextCodec *codec = nullptr;
#define UniString(str) str // codec->toUnicode(str)
/* ------------------------ END global variables -------------------------- */

// default values of settings, overridden by $HOME/.qpsrc if present
bool Qps::flag_show =
    true; // window state of last run : mini(iconic) or normal window
bool Qps::flag_exit = true;
bool Qps::save_pos = true;
bool Qps::flag_qps_hide = true; // TEST
bool Qps::flag_useTabView = false;
bool Qps::show_file_path = false;
bool Qps::show_cmd_path = true;
bool Qps::show_infobar = true;
bool Qps::show_ctrlbar = true;
bool Qps::show_cpu_bar = true;
bool Qps::show_load_graph = true; // DEL
bool Qps::show_statusbar = true;
bool Qps::load_in_icon = true;
bool Qps::auto_save_options = true;
#ifdef LINUX
bool Qps::hostname_lookup = true;
bool Qps::service_lookup = true;
#endif
bool Qps::pids_to_selection = true;
bool Qps::vertical_cpu_bar = false; // not used
#ifdef SOLARIS
bool Qps::normalize_nice = true;
bool Qps::use_pmap = true;
#endif
bool Qps::tree_gadgets = true;
bool Qps::tree_lines = true;
int Qps::swaplimit = 5; // default: warn when less than 10% swap left
bool Qps::swaplim_percent = true;

QThread *thread_main = nullptr;

Qps::Qps()
{
    // NOTE: Since we set the graph as the icon continuously,
    // we do not need a big pixmap cache. The default cache limit
    // is 10240 KB, which is too much for a reasonable memory usage.
    QPixmapCache::setCacheLimit(1024);

    setObjectName("qps_main_window");
    timer_id = 0;
    field_win = nullptr;
    prefs_win = nullptr;
    default_icon = nullptr;
    default_icon_set = false;
    explicit_quit = false;
    under_wayland = QGuiApplication::platformName() == "wayland";

    setIconSize(24, 24); // Important!!
                         //	setMouseTracking(true); // no need?

    make_signal_popup_menu();

    // MOVETO Pstable !!
    m_headpopup = new QMenu( tr( "header_popup" ), this);
    m_headpopup->addAction( tr( "Remove Field" ), this, &Qps::menu_remove_field);
    m_fields = new QMenu( tr( "Add Field" ), this);
    m_headpopup->addMenu(m_fields);

    m_session = new QMenu( tr( "Session" ), this);

    QAction *act;

    m_view = new QMenu( tr( "View" ), this);
    act = m_view->addAction( tr( "Process" ) ); // act->setData(Procview::CUSTOM);
    act = m_view->addAction( tr( "Log" ) );     // act->setData(Procview::CUSTOM);
    // m_view->hide();

    QActionGroup* group = new QActionGroup (this);

    m_field = new QMenu( tr( "Fields" ), this);
    act = m_field->addAction( tr( "Custom Fields" ) );
    act->setData(Procview::CUSTOM);
    act = m_field->addAction( tr( "Basic Fields " ) );
    act->setData(Procview::USER);
    act = m_field->addAction( tr( "Jobs Fields " ) );
    act->setData(Procview::JOBS);
    act = m_field->addAction( tr( "Memory Fields " ) );
    act->setData(Procview::MEM);
#ifdef LINUX
    act = m_field->addAction( tr( "Scheduling Fields " ) );
    act->setData(Procview::SCHED);
    act->setActionGroup(group);
    act->setCheckable(true);
    act->setChecked(false);
#endif

    QList<QAction *> list = m_field->actions ();
    for (int i = 0; i < list.size () - 1; i++)
    {
        QAction *act = list[i];
        act->setCheckable(true);
        act->setActionGroup(group);
    }

    m_field->addSeparator();
    act = m_field->addAction( QIcon::fromTheme(QStringLiteral("edit-find-replace"))
                            , tr( "Select Custom Fields..." )
                            , this
                            , &Qps::menu_custom);
    act->setData(MENU_CUSTOM);

    connect(m_field, &QMenu::triggered, this,
            &Qps::view_menu);
    connect(m_field, &QMenu::aboutToShow, this, &Qps::update_menu_status);

    m_options = new QMenu( tr( "Options" ), this);
    m_options->addAction( tr( "Update Period..." ), this, &Qps::menu_update);
    m_options->addSeparator();
    act = m_options->addAction("", /* MENU_PATH */ this,
                               &Qps::menu_toggle_path);
    act->setData(QVariant(MENU_PATH));
    act = m_options->addAction("", this, &Qps::menu_toggle_infobar);
    act->setData(QVariant(MENU_INFOBAR));
    act = m_options->addAction("", this, &Qps::menu_toggle_ctrlbar);
    act->setData(QVariant(MENU_CTRLBAR));
    act = m_options->addAction( tr( "Show Status bar" )
                              , this
                              ,&Qps::menu_toggle_statusbar);
    act->setData(QVariant(MENU_STATUS));
    act = m_options->addAction("", this, &Qps::menu_toggle_cumul);
    act->setData(QVariant(MENU_CUMUL));

    m_options->addSeparator();
    m_options->addAction( QIcon::fromTheme(QStringLiteral("preferences-system"))
                        , tr( "Preferences..." )
                        , this
                        , &Qps::menu_prefs); // MENU_PREFS

    connect(m_options, &QMenu::aboutToShow, this, &Qps::update_menu_status);

    QMenu *m_help = new QMenu( tr( "Help" ), this);
    m_help->addAction(QIcon::fromTheme("help-about"),  tr( "About" ), this,
                      &Qps::about);

    menubar = new QMenuBar;
    menubar->addMenu(m_session);
    menubar->addMenu(m_field);
    menubar->addMenu(m_options);
    menubar->addSeparator();
    menubar->addMenu(m_help);

    ctrlbar = new ControlBar(this);

    connect(ctrlbar, &ControlBar::need_refresh, this, &Qps::refresh);
    connect(ctrlbar, &ControlBar::viewChange, this,
            &Qps::view_menu); //?????

    context_col = -1;

    procview = new Procview(); // refresh() only not rebuild()

    pstable = new Pstable(this, procview); // no refresh()
    PDisplay *display = new PDisplay(this);
    infobar = display->addSystem(procview);
    statusBar = new StatusBar(this);

    if (!read_settings())
    {
        procview->setSortColumn(qMax(procview->findCol(F_CPU), 0)); // default sorted column
        set_update_period(1300); // default update interval
        resize(700, 400); // default initial size
    }

    // apply the kind of sorting that is read by "read_settings()"
    pstable->setReveseSort(procview->reversed);

    set_table_mode(procview->treeview); //  Pstable::refresh() occur

    m_session->addAction(QIcon::fromTheme(QStringLiteral("application-exit"))
                        , tr( "Quit" )
                        , this
                        , &Qps::save_quit
                        , Qt::ALT | Qt::Key_Q);

    // misc. accelerators
    (void) new QShortcut(Qt::CTRL + Qt::Key_Q, this, SLOT(save_quit()));
    (void) new QShortcut(Qt::CTRL + Qt::Key_Space,
                                  ctrlbar->pauseButton, SLOT(click()));
    (void) new QShortcut(Qt::CTRL + Qt::Key_L, pstable, SLOT(repaintAll()));

    connect(pstable, &HeadedTable::doubleClicked, this, &Qps::open_details );
    connect(pstable, &HeadedTable::rightClicked, this,
            &Qps::show_popup_menu);
    connect(pstable->header(), &TableHead::rightClicked, this,
            &Qps::context_heading_menu);

    selection_items_enabled = true; // ????
    update_load_time = 0;

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setMargin(0);
    if (flag_useTabView)
        vlayout->setSpacing(0);
    else
        vlayout->setSpacing(1);

    vlayout->setMenuBar(menubar);
    vlayout->addWidget(display);
    vlayout->addWidget(ctrlbar);

    logbox = new QTextEdit(this);
    logbox->setReadOnly(true);

    vlayout->addWidget(pstable);
    vlayout->addWidget(logbox);
    logbox->hide();

    vlayout->addWidget(statusBar);
    setLayout(vlayout);

    infobox = new TFrame(this);
    if (update_period != eternity)
        Qps::update_timer(); // startTimer(update_period);

    bar_visibility(); // need

    // testing
    popupx = new QMenu( tr( "test" ), this);
    popupx->addAction( tr( "Copied to Clipboard" ) );
}

// explicit destructor needed for gcc
Qps::~Qps() {}

// return true if all selected processes are stopped
bool Qps::all_selected_stopped()
{
    for (int i = 0; i < procview->linear_procs.size(); i++)
    {
        Procinfo *p = procview->linear_procs[i];
        if (p->selected && p->state != 'T')
            return false;
    }
    return true;
}

// Adjust menu to contain Stop or Continue
void Qps::adjust_popup_menu()
{
    bool allstop = all_selected_stopped();
    QList<QAction *> list = m_popup->actions();

    if (allstop)
        for (int i = 0; i < list.size() - 1; i++)
        {
            QAction *act = list[i];
            int id = act->data().toInt();
            if (id == MENU_SIGCONT)
                act->setVisible(true);
            if (id == MENU_SIGSTOP)
                act->setVisible(false);
        }
    else
        for (int i = 0; i < list.size() - 1; i++)
        {
            QAction *act = list[i];
            int id = act->data().toInt();
            if (id == MENU_SIGCONT)
                act->setVisible(false);
            if (id == MENU_SIGSTOP)
                act->setVisible(true);
        }
}

// build signal menu (used in two places)
QMenu *Qps::make_signal_popup_menu()
{

    // move to pstable?
    QAction *act; // show_popup_menu() callback
    m_popup = new QMenu( tr( "context popup" ), this);
    m_popup->addAction( tr( "Renice..." ), this, &Qps::menu_renice);
    m_popup->addAction( tr( "Scheduling..." ), this, &Qps::menu_sched);
    m_popup->addSeparator();
    m_popup->addAction( tr( "Terminate" ), this, &Qps::sig_term ,
                       Qt::Key_Delete); // better
    m_popup->addAction( tr( "Hangup" ), this, &Qps::sig_hup, Qt::ALT + Qt::Key_H);
    m_popup->addAction( tr( "Kill" ), this, &Qps::sig_kill, Qt::ALT + Qt::Key_K);
    act = m_popup->addAction( tr( "Stop" ), this, &Qps::sig_stop);
    act->setData(MENU_SIGSTOP);
    act = m_popup->addAction( tr( "Continue" ), this, &Qps::sig_cont);
    act->setData(MENU_SIGCONT);

    QMenu *m = new QMenu( tr( "Other Signals" ) );
    act = m->addAction( tr( "SIGINT (interrupt)" ) );
    act->setData(SIGINT);
    act = m->addAction( tr( "SIGCONT (continue)" ) );
    act->setData(SIGCONT);
    act = m->addAction( tr( "SIGSTOP (stop)" ) );
    act->setData(SIGSTOP);
    act = m->addAction( tr( "SIGQUIT (quit)" ) );
    act->setData(SIGQUIT);
    act = m->addAction( tr( "SIGILL (illegal instruction)" ));
    act->setData(SIGILL);
    act = m->addAction( tr( "SIGABRT (abort)" ) );
    act->setData(SIGABRT);
    act = m->addAction( tr( "SIGFPE (floating point exception)" ) );
    act->setData(SIGFPE);
    act = m->addAction( tr( "SIGSEGV (segmentation violation)" ) );
    act->setData(SIGSEGV);
    act = m->addAction( tr( "SIGPIPE (broken pipe)" ) );
    act->setData(SIGPIPE);
    act = m->addAction( tr( "SIGALRM (timer signal)" ) );
    act->setData(SIGALRM);
    act = m->addAction( tr( "SIGUSR1 (user-defined 1)" ) );
    act->setData(SIGUSR1);
    act = m->addAction( tr( "SIGUSR2 (user-defined 2)" ) );
    act->setData(SIGUSR2);
    act = m->addAction( tr( "SIGCHLD (child death)" ) );
    act->setData(SIGCHLD);
    act = m->addAction( tr( "SIGTSTP (stop from tty)" ) );
    act->setData(SIGTSTP);
    act = m->addAction( tr( "SIGTTIN (tty input)" ) );
    act->setData(SIGTTIN);
    act = m->addAction( tr( "SIGTTOU (tty output)" ) );
    act->setData(SIGTTOU);

    connect(m, &QMenu::triggered, this, &Qps::signal_menu);

    m_popup->addMenu(m);
    m_popup->addSeparator();
    m_popup->addAction( tr( "View Details" ), this, &Qps::Action_Detail);

    return m;
}

void Qps::signal_menu(QAction *act)
{
    int signal = act->data().toInt();
    send_to_selected(signal);
}

#ifdef MOSIX

// build migrate menu
static int intcmp(const int *a, const int *b) { return *a - *b; }

QMenu *Qps::make_migrate_menu()
{
    QString buf;
    QMenu *m = new QMenu;
    Svec<int> lst = Procinfo::mosix_nodes();
    lst.sort(intcmp); /// sort
    m->insertItem("Home", 1);
    m->insertItem("Find Best", 0);
    for (int i = 0; i < lst.size(); i++)
    {
        buf.sprintf("to node %d", lst[i]);
        m->insertItem(buf, lst[i] + 1);
    }
    return m;
}
#endif // MOSIX

// update the visibility of the {info, control, status} bar
void Qps::bar_visibility()
{

    if (show_infobar)
        infobar->show();
    else
        infobar->hide();

    if (show_ctrlbar)
        ctrlbar->show();
    else
        ctrlbar->hide();

    if (show_statusbar)
        statusBar->show();
    else
        statusBar->hide();
}

void Qps::timerEvent(QTimerEvent */*e*/) { Qps::refresh(); }

// dialogs.cpp:    qps->update_timer();
void Qps::update_timer()
{
    if (timer_id)
        killTimer(timer_id);
    timer_id = startTimer(update_period); // *** important !!!
}

// change the update period, recomputing the averaging factor
void Qps::set_update_period(int milliseconds)
{
    if (milliseconds == 0)
    {
        qDebug("DEBUG: set_update_period(): update= %dms \n", milliseconds);
        milliseconds = 1000;
    }
    update_period = milliseconds;
    Proc::update_msec = milliseconds; // testing transfer...
    Procview::avg_factor = exp(-(float)update_period / Procview::cpu_avg_time);
}

void Qps::refresh()
{
    if (flag_refresh == false)
        return;

    procview->refresh(); // important
    infobar->updatePixmap(); // add_point and drawGraphOnPixmap();

    if (isVisible())
    {
        pstable->refresh();
        if (show_infobar)      // if(Qps::show_load_graph)
            infobar->update(); // TODO: thread update or
                               // infobar->repaint();
                               //==> pdisplay->update();
    }

    if (trayicon)
        update_icon(); // make icon for systray
}

void Qps::showWindow()
{
    // NOTE: If Qps is started minimized to the tray, the list of processes won't be refreshed
    // until its window is shown. But the first showing calls HeadedTable::resizeEvent(), which
    // checks the known processes to calculate the width of the last column. If a process was
    // stopped before that moment, a crash will happen because the app tries to get its info.
    // Therefore, the list should be refreshed before the first showing of the main window.
    if (flag_refresh && flag_start_mini)
    {
        procview->refresh();
        pstable->refresh();
    }
    showNormal();
    flag_start_mini = false; // the window has been shown for the first time
}

void Qps::resizeEvent(QResizeEvent *e)
{
    int w;
    return;

    w = ctrlbar->sizeHint().width();
    if (!search_box->isVisible())
        w += search_box->sizeHint().width();

    if (e->size().width() < w)
    {
        search_box->hide();
    }
}

/*
 Description :
        1. called when visible() or hide() state.
        2. called when clicked  WINDOW_EXIT_X_BUTTON
        3. NOT called when user logout (session manager)
*/
void Qps::closeEvent(QCloseEvent *e)
{
    // remember position because the window will hide
    winPos.setX(geometry().x());
    winPos.setY(geometry().y());

    if (!explicit_quit && !Qps::flag_exit && trayicon->hasSysTray)
    {
        e->ignore(); // dont close window!
        hide();
        return;
    }
    e->accept();
    if (!explicit_quit) // call save_quit() only if not called by it
        save_quit();
}

void Qps::save_quit()
{
    explicit_quit = true;
    close();
    save_settings();
    qApp->quit();
}

// NEW Version !
// : write geometry, visible fields and other settings
void Qps::save_settings()
{
    if (Qps::auto_save_options)
        write_settings();
}

void Qps::keyPressEvent(QKeyEvent *event)
{
    if (search_box)
    {
        if (!search_box->hasFocus())
            search_box->setFocus(Qt::OtherFocusReason);
        search_box->keyPressEvent(event);
    }
}

void Qps::update_icon()
{
    if (load_in_icon)
        set_load_icon(); // always true
    else
        set_default_icon();
}
// called by update_icon()
void Qps::set_default_icon()
{
    if (!default_icon_set)
    {
        if (!default_icon)
            default_icon = new QPixmap((const char **)icon_xpm);
        setWindowIcon(*default_icon);
        default_icon_set = true;
    }
}

void Qps::set_load_icon()
{
    QPixmap *pm = infobar->load_icon(icon_width, icon_height);
    if (!pm->mask())
    {
        // avoid a fvwm/Qt 1.30 problem and create a filled mask for the
        // icon
        // (without mask, Qt would attempt to use a heuristically
        // created mask)
        QBitmap bm(pm->size());
        bm.fill(Qt::color1);
        pm->setMask(bm);
    }
    // QApplication::setWindowIcon(*pm);
    if (trayicon->hasSysTray)
    {
        trayicon->setIcon(*pm);
    }
    // else
    setWindowIcon(*pm);
    default_icon_set = false;
}

QPixmap *Qps::get_load_icon()
{
    return infobar->load_icon(icon_width, icon_height);
}

// called by
//	1.void Qps::field_removed(int index)
//  ====> QMenu::aboutToShow ()
void Qps::update_menu_status()
{
    // Field Menu
    QList<QAction *> list = m_field->actions();
    for (int i = 0; i < list.size() - 1; i++)
    {
        QAction *act = list[i];
        int id = act->data().toInt();
        if (id == procview->viewfields)
	{
            act->setChecked(true);
	    break;
	}
    }

    // Option Menu
    list = m_options->actions();
    for (int i = 0; i < list.size() - 1; i++)
    {
        QAction *act = list[i];
        int id = act->data().toInt();

        if (id == MENU_PATH)
        {
            act->setCheckable(true);
            act->setChecked(Procview::flag_show_file_path);
            act->setText( tr( "Show File Path" ) );
        }
        else if (id == MENU_INFOBAR)
        {
            act->setCheckable(true);
            act->setChecked(show_infobar);
            act->setText( tr( "Show Graph" ) );
        }
        else if (id == MENU_CTRLBAR)
        {
            act->setCheckable(true);
            act->setChecked(show_ctrlbar);
            act->setText( tr( "Show Control Bar" ) );
        }
        else if (id == MENU_STATUS)
        {
            act->setCheckable(true);
            act->setChecked(show_statusbar);
        }
        else if (id == MENU_CUMUL)
        {
            act->setCheckable(true);
            act->setChecked(Procview::flag_cumulative);
            act->setText( tr( "Include Child Times" ) );
        }
    }
}

// need to change name , redesign
void Qps::view_menu(QAction *act)
{
    int id = act->data().toInt();
    if (act->text() == "Process")
    {
        // tbar->setCurrentIndex(0);
        return;
    }
    if (act->text() == "Log")
    {
        // tbar->setCurrentIndex(1);
        return;
    }

    int state = id;
    if (id >= Procview::ALL && id <= Procview::HIDDEN)
    {
        if (procview->viewproc != state)
        {
            procview->viewproc = state;
        }
    }

    if (id >= Procview::USER && id <= Procview::CUSTOM)
    {
        if (procview->viewfields != state)
        {
            procview->viewfields = state;
            procview->set_fields();
        }
    }
    pstable->refresh(); // layout
                        ///	update_menu_status();
}

// call by SearchBox::keyPressEvent

// slot
void Qps::sig_term() { send_to_selected(SIGTERM); }

void Qps::sig_hup() { send_to_selected(SIGHUP); }

// need
void Qps::sig_stop()
{
    send_to_selected(SIGSTOP);

    // test
    for (int i = 0; i < procview->linear_procs.size(); i++)
    {
        Procinfo *p = procview->linear_procs[i];
        if (p->selected)
        {
            p->test_stop = 1; // who ??
                              // sendsig(p, sig);
        }
    }
}

void Qps::sig_cont()
{
    send_to_selected(SIGCONT);

    // test
    for (int i = 0; i < procview->linear_procs.size(); i++)
    {
        Procinfo *p = procview->linear_procs[i];
        if (p->selected)
            p->test_stop = 0;
    }
}

void Qps::sig_kill() { send_to_selected(SIGKILL); }

void Qps::menu_custom()
{
    ////	view_menu(Procview::CUSTOM); // should !!
    if (field_win)
    {
        field_win->show();
        if (!under_wayland)
            field_win->raise();
    }
    else
    {
        field_win = new FieldSelect(procview);
        field_win->show();
        connect(field_win, SIGNAL(added_field(int)), this,
                SLOT(field_added(int)));
        connect(field_win, SIGNAL(removed_field(int)), this,
                SLOT(field_removed(int)));
    }
}

// called by
// 	1.click Tree_checkbox
//	2.void Qps::view_menu(int id)
void Qps::set_table_mode(bool treemode)
{
    //	qDebug("set_table_mode()\n");
    ctrlbar->setMode(treemode);     // toggle checkbox
    pstable->setTreeMode(treemode); // first  pstable->refresh();
}

// Slot:
// SEGFAULT CODE:  MOVETO Pstable
// called by
//      1. void Qps::add_fields_menu()
//      2. field_win
void Qps::field_added(int field_id, bool fromContextMenu)
{
    int where = -1;
    if (fromContextMenu)
        where = pstable->rightClickedColumn() + 1;
    where = procview->addField(field_id, where);

    int sc = pstable->sortedCol();
    if (where >= 0 && where <= sc)
    { // the sorted column is moved to the right
        pstable->setSortedCol(sc + 1);
        procview->setSortColumn(sc + 1, true);
    }

    pstable->refresh();
}

// MOVETO Proview
// what this hell? SEGFAULT!
// call by
//	1. FieldSelect
//	2. void Qps::menu_remove_field()
void Qps::field_removed(int index)
{
    int col = procview->findCol(index);
    procview->removeField(index);
    if (procview->treeview && index == F_PID)
        set_table_mode(false);

    int sc = pstable->sortedCol();
    if (sc >= 0)
    {
        if (col == sc)
        { // the sorted column is removed; sort the first column
            pstable->setSortedCol(0);
            procview->setSortColumn(0, true); // don't change sort order
        }
        else if (col < sc)
        {
            pstable->setSortedCol(sc - 1);
            procview->setSortColumn(sc - 1, true);
        }
    }

    pstable->refresh();
    context_col = -1;
}

// detail
void Qps::Action_Detail()
{
    for (int i = 0; i < procview->linear_procs.size(); i++)
    {
        Procinfo *p = procview->linear_procs[i];
        if (p->selected)
            open_details(i);
    }
}

// MOVETO : pstable.cpp
void Qps::open_details(int row)
{
    Procinfo *p = procview->linear_procs[row];
    if (p->detail)
    {
        p->detail->show();
        //printf("show detail \n");
    }
    else
    {
        Details *d = new Details(p, procview);
        d->show();
    }
}

// find parents of selected processes
void Qps::menu_parent() { locate_relatives(&Procinfo::ppid, &Procinfo::pid); }

void Qps::menu_children() { locate_relatives(&Procinfo::pid, &Procinfo::ppid); }

// Find processes whose attribute b is equal to the attribute a of
// selected processes. Center around topmost found.
// This is quadratic in worst case (shouldn't be a problem)
// called by
// 		1.menu_children()
// 		2.menu_parent()
void Qps::locate_relatives(int Procinfo::*a, int Procinfo::*b)
{
    QList<int> relatives;
    const int infinity = 2000000000;
    int topmost = infinity;
    for (int i = 0; i < procview->linear_procs.size(); i++)
    {
        Procinfo *p = procview->linear_procs[i];
        if (p->selected)
        {
            pstable->setSelected(i, false);
            for (int j = 0; j < procview->linear_procs.size(); j++)
            {
                Procinfo *q = procview->linear_procs[j];
                if (p->*a == q->*b)
                {
                    relatives.append(j);
                    if (j < topmost)
                        topmost = j;
                }
            }
        }
    }
    for (int i = 0; i < relatives.size(); i++)
        pstable->setSelected(relatives[i], true);
}

// select all (direct and indirect) offsprings of currently selected
// processes, without deselecting them
void Qps::menu_dynasty()
{
    QList<int> family;
    for (int i = 0; i < procview->linear_procs.size(); i++)
        if (pstable->isSelected(i))
            family.append(i);
    for (int i = 0, j = family.size(); i < j;)
    {
        for (int k = 0; k < procview->linear_procs.size(); k++)
        {
            Procinfo *p = procview->linear_procs[k];
            for (int m = i; m < j; m++)
            {
                Procinfo *q = procview->linear_procs[family[m]];
                if (q->pid == p->ppid)
                    family.append(k);
            }
        }
        i = j;
        j = family.size();
    }
    const int infinity = 2000000000;
    int topmost = infinity;
    for (int i = 0; i < family.size(); i++)
    {
        pstable->setSelected(family[i], true);
        if (family[i] < topmost)
            topmost = family[i];
    }
}

// CALLBACK: called when right button is clicked in table
void Qps::show_popup_menu(QPoint p)
{
#ifdef MOSIX
    bool may_migrate = false;
    for (int i = 0; i < procview->linear_procs.size(); i++)
    {
        Procinfo *p = procview->linear_procs[i];
        if (p->selected && p->cantmove.isEmpty())
        {
            may_migrate = true;
            break;
        }
    }
    m_popup->setItemEnabled(POPUP_MIGRATE, may_migrate);
#endif
    adjust_popup_menu();
    m_popup->popup(p);
}

// called when right button is clicked in heading
//
void Qps::context_heading_menu(QPoint p, int col)
{
    context_col = col; // ****
    ////printf("context_col=%d\n",col);
    static int init = 0;
    // rebuild the submenu: only include non-displayed fields
    int ncats = procview->categories.size();

    if (init == 0)
    {
	    QList<int> keys = procview->categories.keys();

	    // sort the list alphabetically
        std::sort(keys.begin(), keys.end(), [this](int a, int b) {
            if (procview->categories.contains(a) && procview->categories.contains(b))
                return QString::localeAwareCompare(procview->categories[a]->name, procview->categories[b]->name) < 0;
            return false;
        });

        for (int i = 0; i < ncats; i++)
        {
            Category *cat = procview->categories[keys.takeFirst()];
            QAction *act = m_fields->addAction(cat->name);
            act->setData(cat->id); // index==key
        }
        connect(m_fields, &QMenu::triggered, this,
                &Qps::add_fields_menu);
        init = 1;
    }

    QBitArray displayed(64); // MAX_FIELDS
    displayed.fill(false);

    for (int i = 0; i < procview->cats.size(); i++)
    {
        displayed.setBit(procview->cats[i]->id);
    }

    QList<QAction *> la = m_fields->actions();
    for (int j = 0; j < la.size(); j++)
    {
        QAction *act = la[j];
        int id = act->data().toInt();
        if (!displayed.testBit(id))
            act->setVisible(true);
        else
            act->setVisible(false);
    }

    m_headpopup->popup(p);
}

// called when field is added from heading context menu
// called by 1. context_heading_menu
void Qps::add_fields_menu(QAction *act)
{
    int id = act->data().toInt();
    field_added(id, true);
}

//
void Qps::menu_remove_field()
{
    if (context_col < 0)
        return;
    field_removed(procview->cats[context_col]->index);
}

void Qps::menu_update()
{

    QString txt;
    if (update_period == eternity)
        txt = "1 s";
    else if (update_period % 1000 == 0)
        txt = QString::asprintf("%d s", update_period / 1000);
    else
        txt = QString::asprintf("%d ms", update_period);
    IntervalDialog id(txt.toUtf8().data(), update_period != eternity);
    id.exec();
    return;
}

void Qps::menu_toggle_path()
{
    Procview::flag_show_file_path = !Procview::flag_show_file_path;
    /// table refresh() !
}

void Qps::menu_toggle_infobar()
{
    show_infobar = !show_infobar;
    bar_visibility();
}

void Qps::menu_toggle_ctrlbar()
{
    show_ctrlbar = !show_ctrlbar;
    bar_visibility();
}

void Qps::menu_toggle_statusbar()
{
    show_statusbar = !show_statusbar;
    bar_visibility();
}

void Qps::menu_toggle_cumul()
{
    Procview::flag_cumulative = !Procview::flag_cumulative;
}

void Qps::menu_prefs()
{
    if (prefs_win)
    {
        prefs_win->show();
        if (!under_wayland)
            prefs_win->raise();
    }
    else
    {
        prefs_win = new Preferences(this);
        prefs_win->show();
        if (!under_wayland)
            prefs_win->raise();

        connect(prefs_win, &Preferences::prefs_change, this, &Qps::config_change);
    }
}

// if "Preferences" changed
void Qps::config_change()
{
    write_settings();
    bar_visibility();
}

void Qps::menu_renice()
{
    int defnice = -1000;

    // use nice of first selected process as default, and check permission
    bool possible = true;
    int euid = geteuid();
    Procinfo *p = nullptr;

    for (int i = 0; i < procview->linear_procs.size(); i++)
    {
        p = procview->linear_procs[i];
        if (p->selected)
        {
            if (defnice == -1000)
                defnice = p->nice;
            if (euid != 0 && euid != p->uid && euid != p->euid)
                possible = false;
        }
    }

    if (!possible)
    {
        QMessageBox::warning( this
                            , tr( "Permission denied" )
                            , tr( "You do not have permission to renice the selected processes. Only the process owner and the super-user are allowed to do that." ) );
        return;
    }

    int new_nice;
    for (;;)
    {
        SliderDialog sd(defnice, -20, 19); // Linux kernel : -20 ~ 19
        if (!sd.exec())
            return;
        bool ok;
        new_nice = sd.ed_result.toInt(&ok);
        if (ok && new_nice >= -20 && new_nice <= 19)
            break;
    }

    // do the actual renicing
    for (int i = 0; i < procview->linear_procs.size(); i++)
    {
        Procinfo *p = procview->linear_procs[i];
        if (p->selected)
        {
            if (setpriority(PRIO_PROCESS, p->pid, new_nice) < 0)
            {
                switch (errno)
                {
                case EPERM:
                    // this shouldn't happen, but (e)uid
                    // could be changed...
                    QMessageBox::warning( this
                                        , tr( "Permission denied" )
                                        , tr( "You do not have permission to renice process %1 (%2).Only the process owner and the super-user are allowed to do that." )
                                          .arg( p->pid )
                                          .arg( p->command ) );
                    break;
                case EACCES:
                    QMessageBox::warning( this
                                        , tr( "Permission denied" )
                                        , tr( "Only the super-user may lower the nice value of a process." ) );
                    return;
                }
            }
            else
            {
                p->nice = new_nice; // don't wait for update
#ifdef LINUX
                p->tms = p->get_tms(); // ditto
#endif
            }
        }
    }
}

void Qps::menu_sched()
{
    if (geteuid() != 0)
    {
        QMessageBox::warning( this
                            , tr( "Permission denied" )
                            , tr( "Only the super-user may change the scheduling policy and static priority." ) );
        return;
    }

    // provide reasonable defaults (first selected process)
    Procinfo *p = nullptr;
    for (int i = 0; i < procview->linear_procs.size(); i++)
    {
        p = procview->linear_procs[i];
        if (p->selected)
            break;
    }
    int pol = p->get_policy();
    int pri = p->get_rtprio();
    SchedDialog sd(pol, pri);
    if (!sd.exec())
        return;

    if (sd.out_policy == SCHED_OTHER)
        sd.out_prio = 0; // only allowed value

    for (int i = 0; i < procview->linear_procs.size(); i++)
    {
        Procinfo *p = procview->linear_procs[i];
        if (p->selected)
        {
            struct sched_param sp;
            sp.sched_priority = sd.out_prio;
            if (sched_setscheduler(p->pid, sd.out_policy, &sp) < 0)
            {
                if (errno == EPERM)
                {
                    QMessageBox::warning( this
                                        , tr( "Permission denied" )
                                        , tr( "You do not have permission to change the scheduling and/or priority of process %1 (%2). Only the super-user may do that." )
                                          .arg( p->pid )
                                          .arg( p->command ) );
                    break;
                }
            }
            else
            {
                p->policy = sd.out_policy; // don't wait for update
                p->rtprio = sd.out_prio;
#ifdef LINUX
                p->tms = p->get_tms();
#endif
            }
        }
    }
}

#ifdef MOSIX

void Qps::mig_menu(int id) { migrate_selected(id - 1); }

void Qps::migrate_selected(int migto)
{
    // User wants to migrate a process somewhere
    // Write destination into /proc/XX/goto
    int warnremote = 0;
    for (int i = 0; i < procview->linear_procs.size(); i++)
    {
        Procinfo *p = procview->linear_procs[i];
        if (p->selected)
        {
            if (p->isremote)
                ++warnremote;
            char buf[80];
            sprintf(buf, "/proc/%d/goto", p->pid);
            FILE *f = fopen(buf, "w");
            if (f)
            {
                fprintf(f, "%d", migto);
                fclose(f);
            }
        }
    }
    if (warnremote)
        QMessageBox::warning(this, "Remote migration attempt",
                             "You can only migrate an immigrated process "
                             "using qps on the home node.");
}
#else

// Since this is a slot, at least a stub must be defined even when it isn't
// used (moc ignores preprocessor directives)
void Qps::mig_menu(int) {}

#endif // MOSIX

void Qps::send_to_selected(int sig)
{
    bool allow = (sig != SIGTERM && sig != SIGHUP && sig != SIGKILL);
    QString msg = (sig == SIGTERM ?
                   tr("Do you really want to terminate the selected process(es)?") :
                   sig == SIGHUP ?
                   tr("Do you really want to hang up the selected process(es)?") :
                   sig == SIGKILL ?
                   tr("Do you really want to kill the selected process(es)?") :
                   QString());
    for (int i = 0; i < procview->linear_procs.size(); i++)
    {
        Procinfo *p = procview->linear_procs[i];
        if (p->selected)
        {
            if (!allow)
            {
                allow = (QMessageBox::question(this, tr("Question"), msg,
                                               QMessageBox::Yes | QMessageBox::No,
                                               QMessageBox::No) == QMessageBox::Yes);
                if (!allow)
                    return;
            }
            sendsig(p, sig);
        }
    }
}

void Qps::sendsig(Procinfo *p, int sig)
{
    if (kill(p->pid, sig) < 0)
    {
        // if the process is gone, do nothing - no need to alert the
        // user
        if (errno == EPERM)
        {
            QMessageBox::warning( this
                                , tr( "Permission denied" )
                                , tr( "You do not have permission to send a signal to process %1 (%2). Only the super-user and the owner of the process may send signals to it." )
                                  .arg( p->pid )
                                  .arg( p->command ) );
        }
    }
}

// If the file format is changed in any way (including adding new
// viewable fields), QPS_FILE_VERSION must be incremented to prevent
// version mismatches and core dumps

#ifdef LINUX
#define QPS_FILE_VERSION 40 // version of .qps-linux file format
#endif

#ifdef SOLARIS
#define QPS_FILE_VERSION 24 // version of .qps-solaris file format
#endif

struct Sflagvar
{
    const char *name;
    bool *var;
};
static Sflagvar flagvars[] = {{"ExitOnClose", &Qps::flag_exit},
                              {"SavePos", &Qps::save_pos},
                              {"TabView", &Qps::flag_useTabView},
                              {"SingleCPU", &Procview::flag_pcpu_single},
                              {"cmdpath", &Procview::flag_show_file_path},
                              {"infobar", &Qps::show_infobar},
                              {"ctrlbar", &Qps::show_ctrlbar},
                              {"statbar", &Qps::show_statusbar},
                              //{"autosave", &Qps::auto_save_options}, // Always ave settings
                              {"cpubar", &Qps::show_cpu_bar},
                              {"loadgraph", &Qps::show_load_graph},
                              {"loadicon", &Qps::load_in_icon},
                              {"selectpids", &Qps::pids_to_selection},
                              {"tree", &Procview::treeview},
                              {"cumulative", &Procview::flag_cumulative},
#ifdef LINUX
                              {"hostname", &Qps::hostname_lookup},
                              {"service", &Qps::service_lookup}
#endif
#ifdef SOLARIS
                              {"normalize", &Qps::normalize_nice},
                              {"pmap", &Qps::use_pmap}
#endif
                              ,
                              {nullptr, nullptr}};

extern QList<watchCond *> watchlist;
// 1.Procview should be contstructed !
// 2.
bool Qps::read_settings()
{
    QSettings set("qps", "qps");

    int v = set.value("version", 0).toInt();
    if (v == 0)
        return false;

    // flags (should be read before fields because "tree" decides how fields are added;
    // should be read before geometry too because of "Qps::save_pos")
    QStringList sl = set.value("flags").toStringList();
    for (Sflagvar *fs = flagvars; fs->name; fs++)
    {
        if (sl.contains(fs->name))
            *(fs->var) = true;
        else
            *(fs->var) = false;
    }

    int x, y, w, h;
    x = set.value("geometry/x").toInt();
    y = set.value("geometry/y").toInt();
    w = set.value("geometry/width").toInt();
    h = set.value("geometry/height").toInt();
    Qps::flag_show = true;
    if (Qps::save_pos)
        setGeometry(x, y, w, h);
    else
        resize(w, h);

    QString fontStr = set.value("font").toString();
    if (!fontStr.isEmpty())
    {
        QFont font;
        font.fromString(fontStr);
        QApplication::setFont(font);
    }

    // fields
    procview->cats.clear();

    QList<QVariant> l = set.value("field").toList();
    for (int i = 0; i < l.size(); i++)
    {
        bool ok;
        int id = l[i].toInt(&ok);
        if (ok)
            procview->addField(id);
    }

    if (procview->cats.isEmpty()) // happens without config file or with a corrupt one
    {
        procview->viewfields = Procview::USER;
        procview->set_fields();
    }
    else
    {
        procview->viewfields = Procview::CUSTOM;
    }

    int col = -1;
    bool ok;
    int fid = set.value("sort/field").toInt(&ok);
    if (ok)
        col = procview->findCol(fid);
    if (col < 0) // don't allow an unsorted table; sort the CPU column by default if it exists
        col = qMax(procview->findCol(F_CPU), 0);
    procview->setSortColumn(col); // Pstable::refresh()
    // pstable -> procview
    procview->reversed = set.value("sort/reversed").toBool(); // tmp

    int i = set.value("interval").toInt();
    set_update_period(i);
    i = set.value("viewproc").toInt();
    procview->viewproc = i;
    ctrlbar->view->setCurrentIndex(i); // procview->viewproc=set.value("viewproc").toInt();

    return true;
}

// USING :
// write geometry, visible fields and other settings to $HOME/.qps-settings
void Qps::write_settings() // save setting
{
    QSettings set("qps", "qps");

    set.setValue("version", QPS_FILE_VERSION);

    set.beginGroup("geometry");
    set.setValue("width", geometry().width());
    set.setValue("height", geometry().height());
    if (!under_wayland)
    {
        if (isVisible())
        {
            set.setValue("x", geometry().x());
            set.setValue("y", geometry().y());
        }
        else
        {
            set.setValue("x", winPos.x());
            set.setValue("y", winPos.y());
        }
    }
    set.endGroup();

    set.setValue("viewproc", procview->viewproc);

    procview->update_customfield();
    set.setValue("field", procview->customFieldIDs);

    if (procview->sortcat) // nullptr by default
    {
        set.beginGroup("sort");
        set.setValue("field", procview->sortcat->id);
        set.setValue("reversed", procview->reversed);
        set.endGroup();
    }
    else
        set.remove("sort");

    QStringList sl;
    for (Sflagvar *fs = flagvars; fs->name; fs++)
    {
        if (*(fs->var))
            sl.append(fs->name);
    }
    set.setValue("flags", sl);

    set.setValue("font", QApplication::font().toString());

    set.setValue("interval", update_period);
}

// return host name with domain stripped
QString short_hostname()
{
    struct utsname u;
    uname(&u);
    char *p = strchr(u.nodename, '.');
    if (p)
        *p = '\0';
    QString s(u.nodename);
    return s;
}

bool opt_eq(const char *arg, const char *opt)
{
    if (arg[0] == '-')
        arg++;
    if (arg[0] == '-')
        arg++;

    return strncmp(arg, opt, strlen(opt)) == 0;
}

// print some help to stdout and exit
void print_help(char * /*cmdname*/)
{
    fprintf(stderr, "Usage: qps [options]\n"
                    "Options:\n"
                    "  -version\t\tversion\n"
                    "  -mini     \t\tstart Minimized\n");
}

void Qps::clicked_trayicon(QSystemTrayIcon::ActivationReason r)
{
    if (r == QSystemTrayIcon::Trigger)
    {
        if (!isHidden())
        {
            winPos.setX(geometry().x());
            winPos.setY(geometry().y());
            hide();
        }
        else
        {
            showWindow();
        }
    }
}

void signal_handler(int /*sig*/)
{
    qps->save_quit();
}

int main(int argc, char **argv, char **envp)
{
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);
    signal(SIGKILL, signal_handler);

    Lookup::initproctitle(argv, envp); // will be REMOVEd

    for (int i = 1; i < argc; i++)
    {
        if (opt_eq(argv[i], "version"))
        {
            fprintf(stderr,
                    "qps version " QPS_VERSION ", using Qt library %s\n",
                    qVersion());
            exit(1);
        }
        else if (opt_eq(argv[i], "help") || opt_eq(argv[i], "h"))
        {
            print_help(argv[0]);
            exit(1);
        }
        else if (opt_eq(argv[i], "min")) // mini
        {
            flag_start_mini = true;
        }
    }
    check_system_requirement(); // check kernel version.. etc in proc.cpp

    QpsApp app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);

    init_misc(nullptr); // init misc, some test code runs ...
    qps = new Qps();
    app.setActivationWindow(qps);

    //	sleep(20);

    QString caption( QString( "%1@%2").arg( getenv( "USER" ) )
                                      .arg( short_hostname() ) ); // geteuid()

    qps->setWindowTitle(UniString(caption));
    qps->setWindowIcon(QPixmap((const char **)icon_xpm));

    // MOVETO  Systray
    QMenu *menu = new QMenu(qps);
    menu->addAction( QObject::tr( "Show" )
                   , qps
                   , &Qps::showWindow);
    menu->addAction( QObject::tr( "Hide" )
                   , qps
                   , &Qps::hide);
    menu->addSeparator();
    menu->addAction( QIcon::fromTheme(QStringLiteral("application-exit"))
                   , QObject::tr( "Quit" )
                   , qps
                   , &Qps::save_quit);

    trayicon = new TrayIcon(QPixmap((const char **)icon_xpm /* init icon */),
                            "qps", menu);
    QObject::connect(trayicon,
                     &QSystemTrayIcon::activated, qps,
                     &Qps::clicked_trayicon);

    trayicon->sysInstall();

    if (flag_start_mini)
    {
        if (!trayicon->hasSysTray)
        {
            qps->showMinimized();
        }
    }
    else
    {
        qps->show();
    }

    AddLog( QString( "Qps %1 launched." ).arg( QPS_VERSION ) );

    return app.exec();
}

void Qps::test_popup(const QUrl &link)
{

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(link.toString(), QClipboard::Clipboard);
    popupx->popup(QCursor::pos());
}

void Qps::about()
{
    QDialog *diag = new QDialog(this);
    diag->setWindowTitle( tr( "About" ) );
    diag->setMinimumWidth(400);
    QVBoxLayout *lay = new QVBoxLayout(diag);

    QLabel *label = new QLabel(diag);
    label->setOpenExternalLinks(true);
    QTextBrowser *browser = new QTextBrowser(diag);
    browser->setOpenExternalLinks(false);
    browser->setOpenLinks(false);

    connect(browser, &QTextBrowser::anchorClicked, this,
            &Qps::test_popup);

    lay->addWidget(label);
    lay->addWidget(browser);

    QString title( tr( "<center><h2> Qps %1</center>").arg( QPS_VERSION ) );
    label->setText( title );
    QString str( tr( "<h2> Qps %1 - A Visual Process Manager </h2> %2 using Qt library %3"
                     "<br><br>"
                     "<b>Source: </b><a href=\"https://github.com/lxqt/qps\">https://github.com/lxqt/qps/</a>"
                     "<br>"
                     "<b>Bugtracker: </b><a href=\"https://github.com/lxqt/qps/issues\">https://github.com/lxqt/qps/issues</a>"
                     "<br>")
                 .arg( QPS_VERSION )
                 .arg(
#ifdef SOLARIS
#ifdef _LP64
                      "64-bit "
#else
                      "32-bit "
#endif
                      "Solaris version "
#else
                      ""
#endif // SOLARIS
                     )
                 .arg( qVersion() ) );
    str.append( tr( "<b>Original Qps by</b><br>"
                    "Mattias Engdegård (f91-men@nada.kth.se)<br><br>"
                    "<b>Contributors</b><br>"
                    "Olivier.Daudel@u-paris10.fr<br>"
                    "jsanchez@todounix.homeip.net <br>"
                    "daehyun.yang@gmail.com <br>"
                    "Luís Pereira (luis.artur.pereira@gmail.com)<br>"
                    "Alf Gaida (agaida@siduction.org)<br>"
                    "Paulo Lieuthier (paulolieuthier@gmail.com)<br>"
                    "Jerome Leclanche (jerome@leclan.ch)<br>" ) );
    browser->setText(str);

    QDialogButtonBox *bbox = new QDialogButtonBox( QDialogButtonBox::Ok
                                                 , Qt::Horizontal
                                                 , diag );

    lay->addWidget(bbox);

    connect(bbox, &QDialogButtonBox::accepted, diag, &QDialog::accept);

    diag->exec();
}

void SearchBox::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Delete)
    {
        qps->sig_term();
        return;
    }
    if (e->modifiers() == Qt::AltModifier)
    {
        if (e->key() == Qt::Key_H)
        {
            qps->sig_hup();
            return;
        }
        if (e->key() == Qt::Key_K)
        {
            qps->sig_kill();
            return;
        }
    }

    if (e->key() == Qt::Key_Escape)
    {
        qps->pstable->clearAllSelections();
        clear();
        return; // the rest is done at SearchBox::onTextChanged()
    }

    if (e->modifiers() == Qt::ControlModifier && e->key() == Qt::Key_A)
        qps->pstable->selectAll();
    else
        QLineEdit::keyPressEvent(e);
}

// Covers pasting of selection clipboard too
void SearchBox::onTextChanged(const QString &txt)
{
    qps->procview->filterstr = txt;
    qps->pstable->refresh();
}

void SearchBox::onClearButtonClicked()
{
    qps->pstable->clearAllSelections();
    // the rest is done at SearchBox::onTextChanged()
}

void STATUSBAR_COUNT_UPDATE() {}

void STATUSBAR_SETCOUNT(int n)
{
    if (qps)
        qps->statusBar->update(n);
}

void PSTABLE_SETTREEMODE(bool mode)
{
    if (qps)
        qps->pstable->setTreeMode(mode);
}

// TESTING
int QPS_PROCVIEW_CPU_NUM()
{
    if (qps)
        return qps->procview->num_cpus;
    else
        return 0;
}

void AddLog(QString str)
{
    qDebug() << qps;
    if (qps && qps->logbox)
    {
        qps->logbox->append(str);
    }
}
