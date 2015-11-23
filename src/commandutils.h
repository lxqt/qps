// commandutils.h
//
// This program is free software. See the file COPYING for details.
// Author: Mattias Engdeg?rd, 1997-1999

#ifndef COMMANDUTILS_H
#define COMMANDUTILS_H

#include <QString>

int find_command(QString s);
bool hasAmpersand(QString cmdline);
void check_command(int idx);
void check_commandAll();
void add_default_command();

#endif // COMMANDUTILS_H
