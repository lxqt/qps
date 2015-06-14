// command.C
//
// This program is free software. See the file COPYING for details.
// Author: Mattias Engdeg?rd, 1997-1999

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <QLabel>
#include <QMessageBox>
#include <QLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QListView>

#include "command.h"
#include "qps.h"
#include "proc.h"
#include "uidstr.h"
#include "dialogs.h"

extern Qps *qps;
extern ControlBar *controlbar;
extern QList<Command *> commands;

int find_command(QString s)
{
    for (int i = 0; i < commands.size(); i++)
        if (s == commands[i]->name)
            return i;
    return -1;
}

// DEL has "&" end of the string ?
bool hasAmpersand(QString cmdline)
{
    QString str;
    int len;
    str = cmdline.simplified();

    if (str == "%update")
        return true; // internal command

    len = str.length();
    if (str[len - 1] == '&')
        return true;
    else
        return false;
}

void check_command(int idx) {}

//
void check_commandAll()
{
    int i, idx;

    return;
    for (int i = 0; i < commands.size(); i++)
    {
        if (hasAmpersand(commands[i]->cmdline) == false)
            commands[i]->cmdline.append("&");
    }
}

// after read ~/.qpsrc
void add_default_command()
{

    int idx;

    /*
    idx=find_command("Update");
    if (idx>=0)
            commands[idx]->cmdline="%update";
    else
            commands.add(new Command("Update","%update",true));
    */

    /*
     *	PAUSED
    idx=find_command("Quit");
    if (idx>=0)
            commands[idx]->cmdline="killall qps";
    else	commands.add(new Command("Quit","killall qps",false));
    */

    // check_commandAll(); DEL?
}

Command::Command(QString n, QString cmd, bool flag)
{
    // printf("Command init\n");
    name = n;
    cmdline = cmd;
    toolbar = false;
    popup = false;

    // toolbutton=new CommandButton(controlbar,name);
    // toolbutton->hide();

    ////toolbutton->setTextLabel (name) ;
    ////toolbutton->setUsesTextLabel ( true );
    /// toolbutton->setAutoRaise(true);
    // QObject::connect(toolbutton, SIGNAL(clicked()),toolbutton,
    // SLOT(exec_command()));
}

QString watchCond::getVal(QString &str, const char *key)
{
    int n; // key.length();
    int idx = str.indexOf(key);
    if (idx < 0)
        return "cant' found";

    idx = str.indexOf("[", idx);
    if (idx < 0)
        return "[ error";
    int idx_end = str.indexOf("]", idx);
    if (idx_end < 0)
        return "] error";
    n = idx_end - idx;
    return str.mid(idx + 1, n - 1);
}
QString watchCond::getstring()
{
    QString string;
    string.clear();
    switch (cond)
    {
    case WATCH_PROCESS_FINISH:
        string.append("if process [" + procname + "] finish, ");
        // string=string.sprintf("if process [%s] finish",procname);
        break;
    case WATCH_PROCESS_START:
        string.append("if process [" + procname + "] start, ");
        break;
    case WATCH_SYS_CPU_OVER:
        string.append("if sys_cpu over [" + QString::number(cpu) + "], ");
        break;
    case WATCH_SYS_CPU_UNDER:
        string.append("if sys_cpu under [" + QString::number(cpu) + "], ");
        break;
    default:
        ;
    }
    if (!command.isEmpty())
        string.append("exec [" + command + "] ");
    if (!message.isEmpty())
        string.append("showmsg [" + message + "] ");
    if (enable)
        string.append("enabled");
    else
        string.append("disabled");

    return string;
}
void watchCond::putstring(QString str)
{
    if (str.contains("if process"))
    {
        if (str.contains("start"))
            cond = WATCH_PROCESS_START;
        if (str.contains("finish"))
            cond = WATCH_PROCESS_FINISH;
        procname = getVal(str, "if process");
    }
    if (str.contains("exec"))
        command = getVal(str, "exec");
    if (str.contains("showmsg"))
        message = getVal(str, "showmsg");
    if (str.contains("enabled"))
        enable = true;
    else
        enable = false;
}
Command::~Command()
{
    // toolbutton->hide();
    // delete toolbutton;
}

QString Command::getString()
{
    QString str;
    str.append(name);
    str.append(",");
    str.append(cmdline);
    return str;
}

void Command::putString(QString str)
{
    int idx = str.indexOf(",");
    // idx=str.indexOf(",",idx);
    name = str.mid(0, idx);
    cmdline = str.mid(idx + 1, str.size());
}

// dirty code...
// Description : this command need "select process"
bool Command::IsNeedProc()
{
    int i, len;
    len = cmdline.length();

    for (i = 0; i < len;)
    {
        int v = cmdline.indexOf('%', i);
        if (v < 0)
            break;
        if (++v >= len)
            break;

        char c = cmdline[v].cell(); //.toLatin1().data();
        switch (c)
        {
        case 'p':
        case 'c':
        case 'C':
        case 'u':
            if (cmdline.indexOf("update", v) == v)
            {
                v += 5;
                break;
            }
            // printf("true\n");
            return true;
        default:
            ;
        }
        i = v + 1;
    }
    return false;
}

QString substString(QString str, Procinfo *p)
{
    QString s;
    int len = str.length();
    for (int i = 0;;)
    {
        int v = str.indexOf('%', i);
        if (v < 0)
        {
            s.append(str.right(len - i));
            break;
        }
        else
        {
            s.append(str.mid(i, v - i));
            if (++v >= len)
                break;
            QString subst;
            // need change to LOCALE(by fasthyun@magicn.com)
            char c = str[v].cell();
            switch (c)
            {
            case 'p':
                subst.setNum(p->pid);
                break;
            case 'c':
                subst = p->command;
                break;
            case 'C':
                subst = p->cmdline;
                break;
            case 'u':
                subst = Uidstr::userName(p->uid);
                break;
            case '%':
                subst = "%";
                break;
            }
            s.append(subst);
            i = v + 1;
        }
    }
    return s;
}

// execute command
void Command::call(Procinfo *p)
{
    QString s;
    QString msg;

    printf("called !\n");
    int len = cmdline.length();

    if (p == NULL)
    {
        if (cmdline == "%update")
        {
            qps->refresh();
            return;
        }

        s = cmdline;
    }
    else
        s = substString(cmdline, p);

    int ret = system(s.toLatin1().data()); ///
                                           /*
                                                   pr=new QProcess;	// leak?
                                                   if(!wc->command.isEmpty())  //conflict pid's command
                                                   {
                                                           pr->start(wc->command);		// thread run, if
                                              null then
                                              segfault occurs. ?
                                                   }
                                       
                                                   connect(pr, SIGNAL(started()), this, SLOT(cmd_started()));
                                                   connect(pr, SIGNAL(finished ( int , QProcess::ExitStatus
                                              )),this,SLOT(cmd_finished ( int , QProcess::ExitStatus )));
                                                   connect(pr, SIGNAL(error ( QProcess::ProcessError )),this,
                                              SLOT(cmd_error(QProcess::ProcessError)));
                                           */

    if (ret)
    {
        msg = "The command:\n\n";
        msg.append(s);
        if (ret == -1)
        {
            msg.append("\n\nfailed with the error:\n\n");
            const char *e =
                (errno == EAGAIN) ? "Too many processes" : strerror(errno);
            msg.append(e ? e : "Unknown error");
        }
        else if (ret & 0xff)
        {
            msg.append("\n\nwas terminated by signal ");
            msg.append(QString().setNum(ret & 0xff));
            msg.append(".");
        }
        else if (ret == 0x7f00)
        {
            msg.append("\n\ncould not be executed because it was not "
                       "found,\nor you did not have execute permission.");
        }
        else
        {
            msg.append("\n\nexited with status ");
            msg.append(QString().setNum(ret >> 8));
            msg.append(".");
        }
        QMessageBox::warning(0, "Command Failed", msg);
    }
}

CommandDialog::CommandDialog()
{
    setWindowTitle("Edit Commands 0.1 alpha");
    // setWindowFlags(Qt::WindowStaysOnTopHint);

    QHBoxLayout *hbox = new QHBoxLayout(this); // TOP
    CommandModel *cmdModel = new CommandModel(this);
    // item list
    listview = new QListView(this);
    listview->setModel(cmdModel);
    listview->setFixedWidth(fontMetrics().width("0") * 16);
    hbox->addWidget(listview);

    QVBoxLayout *vbox = new QVBoxLayout; // TOP-> RIGHT
    hbox->addLayout(vbox);

    QHBoxLayout *h1 = new QHBoxLayout;
    vbox->addLayout(h1);
    QLabel *l1 = new QLabel("Name:", this);
    h1->addWidget(l1);
    name = new QLineEdit(this);
    name->setMinimumWidth(170);
    name->setText("");
    h1->addWidget(name);

    QHBoxLayout *hbox2 = new QHBoxLayout;
    vbox->addLayout(hbox2);
    // qcheck1 = new QCheckBox (this);
    // qcheck1->setText("Toolbar");
    // qcheck1->setEnabled(false);
    // hbox2->addWidget(qcheck1);
    if (0)
    {
        qcheck2 = new QCheckBox(this);
        qcheck2->setText("Popup");
        qcheck2->setEnabled(false);
        hbox2->addWidget(qcheck2);
    }

    QLabel *l2 = new QLabel("Command Line:", this);
    l2->setFixedHeight(l2->sizeHint().height());
    l2->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    vbox->addWidget(l2);

    cmdline = new QLineEdit(this);
    cmdline->setFixedHeight(cmdline->sizeHint().height());
    cmdline->setMinimumWidth(250);
    cmdline->setText("");
    vbox->addWidget(cmdline);

    QLabel *l3 = new QLabel("Substitutions:\n"
                            "%p\tPID\n"
                            "%c\tCOMMAND\n%C\tCMDLINE\n%u\tUSER\n"
                            "%%\t%\n"
                            "\n",
                            this);

    l3->setFrameStyle(QFrame::Panel);
    l3->setFrameShadow(QFrame::Sunken);
    l3->setAlignment(Qt::AlignVCenter | Qt::AlignLeft); // | Qt::ExpandTabs);
    vbox->addWidget(l3);

    QHBoxLayout *hl = new QHBoxLayout;
    vbox->addLayout(hl);
    new0 = new QPushButton("New...", this);
    hl->addWidget(new0);
    add = new QPushButton("Add...", this);
    hl->addWidget(add);
    del = new QPushButton("Delete", this);
    hl->addWidget(del);
    button_ok = new QPushButton("Close", this);
    hl->addWidget(button_ok);

    connect(listview, SIGNAL(clicked(const QModelIndex &)),
            SLOT(set_select(const QModelIndex &)));
    connect(new0, SIGNAL(clicked()), SLOT(new_cmd()));
    connect(add, SIGNAL(clicked()), SLOT(add_new()));
    connect(del, SIGNAL(clicked()), SLOT(del_current()));
    connect(button_ok, SIGNAL(clicked()), SLOT(close()));
    connect(name, SIGNAL(textChanged(const QString &)),
            SLOT(event_name_midified(const QString &)));
    connect(cmdline, SIGNAL(textChanged(const QString &)),
            SLOT(event_cmd_modified()));
    // connect(qcheck1, SIGNAL(toggled ( bool ) ),
    // SLOT(event_toolbar_checked(bool
    // )));

    TBloon *bloon = new TBloon(this);
    /// for(int i = 0; i < commands.size(); i++)
    /// listview->insertItem(commands[i]->name);
    //	listview->addItem(commands[i]->name);
    /// vbox->freeze();
}

// DEL
void CommandDialog::event_toolbar_checked(bool on)
{
    // name->text();
    int idx = find_command(name->text());
    if (idx >= 0)
        commands[idx]->toolbar = on;

    /// controlbar->update_bar();
}

void CommandDialog::event_name_midified(const QString &new_name)
{
    int idx;
    FUNC_START;
    // printf("debug:changed_description() start \n");
    idx = find_command(new_name);
    if (idx == -1)
    {
        add->setEnabled(1);
    }
    else
        add->setEnabled(0);

    // printf("debug:changed_description() end \n");
}

// if modified then call this function
void CommandDialog::event_cmd_modified()
{
    int idx;
    // if(name->text()=="") return;
    if (find_command(name->text()) < 0)
        return;

    idx = find_command(name->text());

    commands[idx]->name = name->text();
    commands[idx]->cmdline = cmdline->text();
    emit command_change();
}

//	set the description,cmdline  from current selected QListBox item
void CommandDialog::set_buttons(int index)
{
    if (index < 0)
    {
        new_cmd();
        return;
    }
    /*
    //bool sel = (lb->currentRow() >= 0);
    Command *c ;
    if(sel)
            //c = commands[find_command(lb->currentText())];
            c = commands[find_command(lb->currentText())];
    else
            c = commands[find_command(lb->text(index))];
    name->setText(c->name);
    cmdline->setText(c->cmdline);
    del->setEnabled(sel);
  */
}

//	called when clicked !
void CommandDialog::set_select(const QModelIndex &index)
{
    Command *c =
        static_cast<Command *>(index.internalPointer()); // never Null ?
                                                         /*
                                                                 if (item==NULL) return; // important
                                                                 Command *c = commands[find_command(item->text())];
                                                         */
    name->setText(c->name);
    cmdline->setText(c->cmdline);
    // DEL	qcheck1->setChecked(c->toolbar);
    //	qcheck2->setChecked(c->popup);

    //	bool sel = (listview->currentItem() >= 0);
    if (c->name == "Update")
        del->setEnabled(false);
    else
        del->setEnabled(true);
}

void CommandDialog::reset()
{
    listview->reset();
    name->setText("");
    cmdline->setText("");
    add->setText("Add...");
    add->setEnabled(0);
    button_ok->setEnabled(1);
    listview->clearSelection();
}

void CommandDialog::new_cmd()
{
    reset();
    add->setEnabled(1);
    name->setFocus();
}

void CommandDialog::add_new()
{
    if (name->text() == "")
        return;

    // commands.add(new Command(name->text(),
    // cmdline->text(),qcheck1->isChecked
    // () ));
    commands.append(new Command(name->text(), cmdline->text(), false));
    check_commandAll(); // TEMP

    listview->reset();
    add->setEnabled(0);
    del->setEnabled(0);
    button_ok->setEnabled(1);

    emit command_change(); // notice to refresh Qps::make_command_menu()
                           //	control_bar->update_bar(); // ** important
}

void CommandDialog::del_current()
{
    int idx = find_command(name->text());
    if (idx >= 0)
    {
        // printf("del\n");
        commands.removeAt(idx);
        listview->reset(); // listview->reset();
        //		control_bar->update_bar();
        emit command_change(); // notice to refresh menu_commands
    }
}

// CommandModel
CommandModel::CommandModel(QObject *parent) {}
CommandModel::~CommandModel() {}

QModelIndex CommandModel::index(int row, int column,
                                const QModelIndex &parent) const
{
    if (row >= 0 and column >= 0 and row < commands.size() and column < 1)
    {
        Command *cmd = commands[row];
        return createIndex(row, column, cmd);
    }
    else
        return QModelIndex();
}
QModelIndex CommandModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}
int CommandModel::rowCount(const QModelIndex &parent) const
{
    return commands.size();
}
// int CommandModel::columnCount(const QModelIndex &parent) const{return 1;};
QVariant CommandModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        Command *cmd = static_cast<Command *>(index.internalPointer());
        return cmd->name;
    }
    if (role == Qt::DecorationRole)
    {
    }
    return QVariant();
}
void CommandModel::update() {} // TEMP

//----------------------------------------------------------------
QList<watchCond *> watchlist;
QList<ExecWindow *> execlist;

extern WatchdogDialog *watchdogDialog;

// except threads, already running process
void watchdog_check_if_start(QString cmd, Procinfo *pi)
{
    /// printf("cmd=%s\n", cmd.toLatin1().data());
    for (int i = 0; i < watchlist.size(); ++i)
    {
        watchCond *wc = watchlist.at(i);
        if (wc->enable == false)
            continue;
        if (wc->cond == WATCH_PROCESS_START)
            if (wc->procname == cmd)
            {
                // printf("Watchdog: start\n");
                if (!pi->isThread())
                    // ExecWindow *mw=new
                    // ExecWindow(wc->message,wc->command,pi->pid,pi->command);
                    // // leak
                    ExecWindow *mw =
                        new ExecWindow(wc, pi->pid, pi->command); // leak

                // note :
                //	1.system("./loop"); //block !!
                //	2.pr.setEnvironment(QProcess::systemEnvironment
                //());
            }
    }
}

void watchdog_check_if_finish(QString cmd, Procinfo *pi)
{
    for (int i = 0; i < watchlist.size(); ++i)
    {
        watchCond *w = watchlist.at(i);
        if (w->enable == false)
            continue;
        if (w->cond == WATCH_PROCESS_FINISH)
        {
            if (w->procname == cmd)
            {
                // printf("Watchdog: finish\n");
                if (!pi->isThread())
                    // if(pi->pid==pi->tgid) // not a thread
                    // !
                    ExecWindow *mw =
                        new ExecWindow(w, pi->pid, pi->command); // leak
                //	ExecWindow *mw=new
                // ExecWindow(w->message,w->command,pi->pid,pi->command);
            }
        }
    }
}

// NOTYET
void watchdog_syscpu(int cpu)
{
    //	printf("Watchdog: watchdog_syscpu\n");
    // if(watchdogDialog->flag_ifsyscpu)
    {
        //	if(cpu> watchdogDialog->syscpu_over)
        //		printf("Watchdog: event %d%\n",cpu);
        // QMessageBox::warning(0, "Watchdog", "aaaaa"); //blocking
    }
}

// ExecWindow
ExecWindow::ExecWindow()
{
    setupUi(this);
    //	connect(okButton, SIGNAL(clicked()), this, SLOT(close()));
    //	show();
}

ExecWindow::~ExecWindow() {}

// eventcat_id;
ExecWindow::ExecWindow(watchCond *wc, int pid, QString cmd)
{
    setupUi(this);
    setWindowTitle("Qps Watchdog");

    wcond = wc;

    QString str;

    if (wc->cond == WATCH_PROCESS_START)
    {
        textEdit->append(cmd + "(" + QString::number(pid) + ")" + " start");
    }
    if (wc->cond == WATCH_PROCESS_FINISH)
        textEdit->append(cmd + "(" + QString::number(pid) + ")" + " finished");

    flag_started = false;

    pr = new QProcess;          // leak?
    if (!wc->command.isEmpty()) // conflict pid's command
    {
        pr->start(wc->command); // thread run, if null then segfault occurs. ?
    }

    connect(okButton, SIGNAL(clicked()), this, SLOT(cmd_ok()));

    connect(pr, SIGNAL(started()), this, SLOT(cmd_started()));
    connect(pr, SIGNAL(finished(int, QProcess::ExitStatus)), this,
            SLOT(cmd_finished(int, QProcess::ExitStatus)));
    connect(pr, SIGNAL(error(QProcess::ProcessError)), this,
            SLOT(cmd_error(QProcess::ProcessError)));

    show();

    execlist.append(this);
}

ExecWindow::ExecWindow(QString str, QString exec_cmd, int pid, QString cmd)
{
    setupUi(this);
    //
}

// QProcess: Destroyed while process is still running.(Segmentation fault)
void ExecWindow::cmd_ok()
{
    if (pr->state() == QProcess::Running)
    {
        //	pr->kill();
        pr->terminate();
        return;
    }
    close(); // Qt::WA_DeleteOnClose
}

// slot : catch terminate signal.
void ExecWindow::cmd_finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    textEdit->append(wcond->command + " exit with code " +
                     QString::number(exitStatus));
    okButton->setText("Close");
    delete pr;
}

void ExecWindow::cmd_started()
{
    textEdit->append(wcond->command + " [running]");
    okButton->setText("terminate command");
    flag_started = true;
}

void ExecWindow::cmd_error(QProcess::ProcessError e)
{
    // not found command
    // Error ? :
    if (e == QProcess::FailedToStart)
        //	textEdit->append("Error :  command not found [" +
        // command + "]" + "(code
        //" + QString::number(e) + ")" );
        textEdit->append("Error " + QString::number(e) + " : [" +
                         wcond->command + "] Maybe command not found");
    delete pr;
}

void ExecWindow::setText(QString str)
{
    textEdit->append(str);
    //	label->setText(str);
}

WatchdogDialog::WatchdogDialog()
{
    setupUi(this);
    listmodel = new ListModel();

    tableView->setModel(listmodel);
    checkBoxDelegate delegate;
    tableView->setEditTriggers(QAbstractItemView::SelectedClicked);
    ///	tableView->setItemDelegate(&delegate);

    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    QHeaderView *h = tableView->verticalHeader();
    h->setVisible(false);

    QHeaderView *v = tableView->horizontalHeader();
#if QT_VERSION >= 0x050000
    v->setSectionResizeMode(0, QHeaderView::Stretch);
    v->setSectionResizeMode(1, QHeaderView::ResizeToContents);
#endif
    //	v->setClickable (false);
    connect(newButton, SIGNAL(clicked()), this, SLOT(_new()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(apply()));
    connect(addButton, SIGNAL(clicked()), this, SLOT(add()));
    connect(delButton, SIGNAL(clicked()), this, SLOT(del()));
    connect(comboBox, SIGNAL(activated(int)), SLOT(comboChanged(int)));
    connect(comboBox, SIGNAL(highlighted(const QString &)),
            SLOT(condChanged(const QString &)));

    connect(tableView, SIGNAL(clicked(const QModelIndex &)),
            SLOT(eventcat_slected(const QModelIndex &)));
    connect(message, SIGNAL(textEdited(const QString &)),
            SLOT(Changed(const QString &)));
    connect(command, SIGNAL(textEdited(const QString &)),
            SLOT(Changed(const QString &)));
    connect(proc_name, SIGNAL(textEdited(const QString &)),
            SLOT(Changed(const QString &)));
    connect(comboBox, SIGNAL(activated(const QString &)),
            SLOT(Changed(const QString &)));

    checkBox_alreadyrun->hide();
    listView->hide();
    spinBox->hide();
    label_cpu->hide();
    ///	printf("close ...\n");
    //	tableView->update();
    //	listmodel->update(); // meaningless..

    TBloon *bloon = new TBloon(this);
    return;
}

void WatchdogDialog::showEvent(QShowEvent *event)
{
    //  Qt 4.4.0 bug?
    //  printf("show!!!!!!!!!\n");
    listmodel->update();
}
void WatchdogDialog::comboChanged(int idx)
{

    // itemText(idx);
    QString str = comboBox->currentText();

    if (str.contains("cpu"))
    {
        label_cpu->show();
        spinBox->show();
    }
    else
    {
        spinBox->hide();
        label_cpu->hide();
    }

    if (str.contains("process"))
    {
        label_procname->show();
        proc_name->show();
    }
    else
    {
        label_procname->hide();
        proc_name->hide();
    }

    if (message->text().isEmpty())
    {
        //	if(str.contains("start")) message->setText("%CMD start
        // with pid %PID");
        //	if(str.contains("finish"))	message->setText("%CMD
        // finish with pid
        //%PID");
    }
}

void WatchdogDialog::eventcat_slected(const QModelIndex &idx)
{

    watchCond *w = watchlist[idx.row()];
    //	printf("row=%d\n",at=idx.row());

    if (idx.column() == 1)
    {
        w->enable = !(w->enable);
        listmodel->update(idx.row());
        return;
    }

    QString str = idx.data().toString(); // Qt::DisplayRol

    if (str.contains("process"))
        proc_name->setText(w->procname);
    else
        proc_name->setText("");
    if (str.contains("cpu"))
        spinBox->setSingleStep(w->cpu);
    else
        spinBox->setSingleStep(50);
    if (str.contains("exec"))
        command->setText(w->command);
    else
        command->setText("");
    if (str.contains("showmsg"))
        message->setText(w->message);
    else
        message->setText("");

    checkCombo();
    comboBox->setCurrentIndex(w->cond);
}

void WatchdogDialog::Changed(const QString &str)
{
    QModelIndex idx = tableView->currentIndex();
    //	QModelIndexList list=tableView->selectedIndexes ();
    bool flag = tableView->selectionModel()->hasSelection();
    // if(list.count() and idx.isValid())
    if (flag and idx.isValid())
    {
        int at = idx.row();
        watchCond *w = watchlist[at];
        w->message = message->text();
        w->command = command->text();
        w->procname = proc_name->text();
        w->cond = comboBox->currentIndex();
        listmodel->update(at);
        // watchlist.removeAt(at);
    }
    // listmodel->update();
}

void WatchdogDialog::checkCombo()
{
    if (comboBox->count() == 1)
    {
        comboBox->clear();
        comboBox->addItem("if process start");
        comboBox->addItem("if process finish");
        return;
    }
}

// comboChanged() -> checkCombo()
void WatchdogDialog::condChanged(const QString &str)
{
    checkCombo();
    // what is this?
    // printf("chagend\n");
    // comboBox->currentText();
    // command->text();
    // message->text();
}

void WatchdogDialog::_new()
{
    tableView->clearSelection();
    proc_name->clear();
    command->clear();
    message->clear();
    comboBox->clear();
    comboBox->addItem("select condition");
}

void WatchdogDialog::add()
{
    watchCond *w = new watchCond;
    w->enable = true;
    w->cond = comboBox->currentIndex();
    w->command = command->text();
    w->message = message->text();
    w->procname = proc_name->text();
    watchlist.append(w);
    //	listView->update(QModelIndex());
    //	listView->reset();
    //	tableView->reset();
    //	listmodel->insertRow(listmodel->rowCount(QModelIndex()));
    // tableView->update(QModelIndex());
    // tableView->dataChanged(QModelIndex(),QModelIndex()); //protected
    listmodel->update();
}

void WatchdogDialog::del()
{
    // QModelIndex idx=listView->currentIndex();
    QModelIndex idx = tableView->currentIndex();
    if (idx.isValid())
    {
        int at = idx.row();
        watchlist.removeAt(at);
    }
    listmodel->update();
    tableView->setCurrentIndex(idx);
}

void WatchdogDialog::apply()
{
    qps->write_settings();
    close();
}

QModelIndex ListModel::index(int row, int column,
                             const QModelIndex &parent) const
{
    // printf("index %d %d\n",row,column);
    if (row >= 0 and column >= 0 and row < watchlist.size())
    {
        if (column < 2)
        {
            watchCond *item = watchlist[row];
            return createIndex(row, column, item);
        }
    }
    return QModelIndex();
}

void ListModel::update(int row)
{
    emit dataChanged(index(row, 0), index(row, 1));
}

void ListModel::update(const QModelIndex &idx) { emit dataChanged(idx, idx); }

// Pure Virtual
QModelIndex ListModel::parent(const QModelIndex &child) const
{
    return QModelIndex(); // no parent!
}
int ListModel::rowCount(const QModelIndex &parent) const
{
    return watchlist.size();
}
QVariant ListModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    //	printf("headerData\n");
    if (role == Qt::DisplayRole)
    {
        if (section == 0)
            return QString("Event Category");
        if (section == 1)
            return QString("Enable");
    }
    /*
    if (role == Qt::FontRole)
    {
            QFont f=QFont();
            f.setBold(false);
            return f;
    } */
    if (role == Qt::SizeHintRole)
    {
        //	return QSize(18,18);
    }
    return QVariant();
}

QVariant ListModel::data(const QModelIndex &index, int role) const
{
    //	printf("data\n");
    watchCond *item = static_cast<watchCond *>(index.internalPointer());
    if (index.column() == 0)
    {
        if (role == Qt::DisplayRole)
        {
            return QString(item->getstring());
        }
        if (role == Qt::DecorationRole)
        {
        }
        if (role == Qt::EditRole)
        {
        }
    }

    if (index.column() == 1)
    {
        if (role == Qt::CheckStateRole)
        {
            if (item->enable)
                return Qt::Checked;
            else
                return Qt::Unchecked;
        }
        if (role == Qt::TextAlignmentRole)
            return Qt::AlignRight;
        if (role == Qt::EditRole)
        {
        }
    }
    if (role == Qt::SizeHintRole)
    {
        //	return QSize(18,18);
    }
    return QVariant();
}
// void CommandModel::update(){} //TEMP

#include <QSpinBox>
checkBoxDelegate::checkBoxDelegate(QObject *parent) : QItemDelegate(parent) {}
//! [0]

//! [1]
QWidget *checkBoxDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const
{
    printf("createEditor\n");
    // return 0;
    if (index.column() == 1)
    {
        QSpinBox *editor = new QSpinBox(parent);
        editor->setMinimum(0);
        editor->setMaximum(100);
        return editor;
    }
    return QItemDelegate::createEditor(parent, option, index);
    return 0;
}
//! [1]

//! [2]
void checkBoxDelegate::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
{
    printf("setEditorData\n");
    return;
    // if(index
    if (index.column() == 1)
    {
        int value = index.model()->data(index, Qt::EditRole).toInt();
        //	int value=0;
        QSpinBox *spinBox = static_cast<QSpinBox *>(editor);
        spinBox->setValue(value);
    }
}
//! [2]

//! [3]
void checkBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
    printf("setModelData\n");
    return;
    if (index.column() == 1)
    {
        QSpinBox *spinBox = static_cast<QSpinBox *>(editor);
        spinBox->interpretText();
        int value = spinBox->value();
        model->setData(index, value, Qt::EditRole);
    }
}
//! [3]

//! [4]
void checkBoxDelegate::updateEditorGeometry(QWidget *editor,
                                            const QStyleOptionViewItem &option,
                                            const QModelIndex &index) const
{
    printf("updateEditorGeometry\n");
    //	if(index.column()==1)
    editor->setGeometry(option.rect);
}
//! [4]

void checkBoxDelegate::paint(QPainter *painter,
                             const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    printf("paint\n");
    if (index.column() == 1)
    {
        QVariant value = 0; // index.model()->data(index, Qt::UserRole);
                            /* if (!isSupportedType(value.type())) {
                                     QStyleOptionViewItem myOption = option;
                                     myOption.state &= ~QStyle::State_Enabled;
                                     QItemDelegate::paint(painter, myOption, index);
                                     return;
                             } */
    }
    QItemDelegate::paint(painter, option, index);
}
// QSize QAbstractItemDelegate::sizeHint ( const QStyleOptionViewItem & option,
// const QModelIndex & index ) const	 [pure virtual]
