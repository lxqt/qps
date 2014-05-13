// wchan.h
//
// This program is free software. See the file COPYING for details.
// Author: Mattias Engdegård, 1997-1999

#ifndef WCHAN_H
#define WCHAN_H

#include <QHash>
#include <QString>

class Wchan {
public:
    static QString name(unsigned long addr);

#ifdef LINUX
private:
    static bool open_sysmap();
    static bool try_sysmap(const char *path);
    static char *find_sym(unsigned long addr);
    static inline int beginning_of_line(int ofs);

    static QHash<int,char*> dict;
    static char *sysmap;	// pointer to mmap()ed System.map
    static bool sysmap_inited;	// if an attempt to read sysmap has been made
    static int sysmap_size;
#endif
};

#endif	// WCHAN_H
