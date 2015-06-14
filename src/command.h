// command.h				emacs, this is written in -*-c++-*-
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

int find_command(QString s);
void add_default_command();

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

class CommandModel : public QAbstractItemModel
{
    Q_OBJECT
  public:
    CommandModel(QObject *parent = 0);
    ~CommandModel();
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const { return 1; };
    QVariant data(const QModelIndex &index, int role) const;
    //  Qt::ItemFlags flags(const QModelIndex &index) const;
    void update(); // TEMP
};

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

#include <QItemDelegate>
class checkBoxDelegate : public QItemDelegate
// class checkBoxDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
  public:
    checkBoxDelegate(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
};

#include <QStandardItemModel>
// class ListModel : public QAbstractItemModel
class ListModel : public QAbstractTableModel
// class ListModel : public QStandardItemModel
{
    Q_OBJECT
  public:
    ListModel(QObject *parent = 0){};
    ~ListModel(){};
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const; // pure
    QModelIndex parent(const QModelIndex &child) const; // pure virtual
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const { return 2; };
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation o, int role) const;
    //	QMap<int, QVariant> itemData ( const QModelIndex & index ) const
    //;
    void update()
    { // reset();
    }
    void update(const QModelIndex &idx);
    void update(int row);
    //  Qt::ItemFlags flags(const QModelIndex &index) const;
    //	void update(); //TEMP
};

#include "ui_watchdog.h"
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

#define WATCH_PROCESS_START 0
#define WATCH_PROCESS_FINISH 1
#define WATCH_PROCESS_CPU_OVER 2
#define WATCH_SYS_CPU_OVER 3
#define WATCH_SYS_CPU_UNDER 4

// if process [name] start, exec [command], showmsg [xxx]
// if process [name] finish exec [command], showmsg [xxx]
// if system cpu over [90%], exec [command], msg [xxx]
// if system cpu under [10%], exec [command], msg [xxx]
// if process [name] cpu over [90%] exec [command] msg [xxx]
// if process [name] start, kill_it, msg [xxx]
// if process [name] start, soundplay [ ], msg [xxx]

class watchCond
{
  public:
    int cond;
    int enable;
    int cpu;
    QString procname;
    QString command;
    QString message;

    watchCond()
    {
        enable = 0;
        //	procname[0]=0;		command[0]=0;
        // message[0]=0;
    }
    // key [txt]	[a]
    // QString getVal(QString &str, QString &key)
    QString getVal(QString &str, const char *key);
    QString getstring();
    void putstring(QString str);
};

#include <QProcess>
#include "ui_message.h"
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

#endif // COMMAND_H
