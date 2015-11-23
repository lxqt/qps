// watchdogdialog.cpp
//
// This program is free software. See the file COPYING for details.
// Author: Mattias Engdeg?rd, 1997-1999

#ifndef WATCHDOGDIALOG_H
#define WATCHDOGDIALOG_H

#include "ui_watchdog.h"

#include <QDialog>
#include <QModelIndex>
#include <QString>

class ListModel;
class QShowEvent;

class WatchdogDialog : public QDialog, private Ui_EventDialog
{
    Q_OBJECT
  public:
    WatchdogDialog();
    ListModel *listmodel;
    // signals:
    //    void command_change();
    void checkCombo();
  protected slots:
    void _new();
    void apply();
    void add();
    void del();
    void condChanged(const QString &str);
    void Changed(const QString &str);

    void comboChanged(int);
    void eventcat_slected(const QModelIndex &idx);

  protected:
    virtual void showEvent(QShowEvent *event);

    //  void set_select( const QModelIndex & );
    //  void event_name_midified(const QString &new_name);
    //  void event_cmd_modified();
    // void event_toolbar_checked(bool);
  private:
};

#endif // WATCHDOGDIALOG_H
