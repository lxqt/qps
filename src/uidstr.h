// uidstr.h
//
// This program is free software. See the file COPYING for details.
// Author: Mattias Engdegård, 1997-1999

#ifndef UIDSTR_H
#define UIDSTR_H

#include <QHash>
#include <QString>

class Uidstr {
public:
    static QString userName(int uid);
    static QString groupName(int gid);

private:
    static QHash<int,char*> udict;
    static QHash<int,char*> gdict;
};

#endif	// UIDSTR_H
