// watchcond.h
//
// This program is free software. See the file COPYING for details.
// Author: Mattias Engdegård, 1997-1999

#ifndef WATCHCOND_H
#define WATCHCOND_H

#include <QString>

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

#endif // WATCHCOND_H

