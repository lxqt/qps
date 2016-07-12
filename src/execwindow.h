// execwindow.h
//
// This program is free software. See the file COPYING for details.
// Author: Mattias Engdegård, 1997-1999

#ifndef EXECWINDOW_H
#define EXECWINDOW_H 

#include <QProcess>
#include <QString>

#include "ui_message.h"

class watchCond;

class ExecWindow : public QWidget, private Ui_ExecWindow
{
    Q_OBJECT
  public:
    ExecWindow();
    ExecWindow(QString str, QString exec_cmd, int pid = 0, QString cmd = "");
    ExecWindow(watchCond *wc, int pid = 0, QString cmd = "");
    ~ExecWindow();
    void setText(QString str);
    QProcess *pr;
    // QProcess 	proc;
    QString execmd;
    int flag_started;
    watchCond *wcond;
  protected slots:
    void cmd_started();
    void cmd_finished(int exitCode, QProcess::ExitStatus exitStatus);
    void cmd_error(QProcess::ProcessError error);
    void cmd_ok();
};

#endif // EXECWINDOW_H
