// commanddialog.h
//
// This program is free software. See the file COPYING for details.
// Author: Mattias Engdeg?rd, 1997-1999


#ifndef COMMANDDIALOG_H
#define COMMANDDIALOG_H

#include <QDialog>

class QListView;
class QPushButton;
class QLineEdit;
class QCheckBox;
class QModelIndex;

// class CommandDialog : public QWidget
class CommandDialog : public QDialog
{
    Q_OBJECT
  public:
    CommandDialog();

signals:
    void command_change();

  protected slots:
    void new_cmd();
    void add_new();
    void del_current();
    void set_buttons(int);
    void reset();
    void set_select(const QModelIndex &);
    void event_name_midified(const QString &new_name);
    void event_cmd_modified();
    void event_toolbar_checked(bool);

  private:
    QListView *listview;
    QPushButton *new0, *add, *del, *edit, *button_ok;
    QLineEdit *name, *cmdline;
    QCheckBox *qcheck1;
    QCheckBox *qcheck2;
};

#endif // COMMANDDIALOG_H
