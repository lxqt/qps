// execwindow.cpp
//
// This program is free software. See the file COPYING for details.
// Author: Mattias Engdegård, 1997-1999

#include "execwindow.h"

#include "watchcond.h"

QList<ExecWindow *> execlist;

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

