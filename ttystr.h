// ttystr.h
//
// This program is free software. See the file COPYING for details.
// Author: Mattias Engdegård, 1997-1999

#ifndef TTYSTR_H
#define TTYSTR_H

#include "config.h"
#include <QString>

#ifdef LINUX
#include <sys/sysmacros.h>
#endif

#ifdef SOLARIS
#include <QHash>
#endif

class Ttystr {
public:
    static QString name(dev_t devnr);

#ifdef SOLARIS
private:
    static void read_devs();
    static void scandevdir(const char *prefix);

    static QHash<int,char*> dict;
    static bool scanned;
#endif
};

#endif	// TTYSTR_H
