// command.h
//
// This program is free software. See the file COPYING for details.
// Author: Mattias Engdegård, 1997-1999

#ifndef COMMAND_H
#define COMMAND_H

#ifndef USING_PCH
#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QToolButton>
#include <QPushButton>
#include <QListView>
#endif

#include "proc.h"
#include "misc.h"

class ListModel;

class Command
{
  public:
    Command(){};
    Command(QString n, QString cmd, bool toolbar = false);
    ~Command();
    void call(Procinfo *p);
    bool IsNeedProc();
    QString getString();
    void putString(QString str);

    // CommandButton *toolbutton;

    QString name;    // appears in the menu
    QString cmdline; // may contain variables (%p etc)
    int menu;        // index in menu
    bool toolbar;
    bool popup;
};

#endif // COMMAND_H
