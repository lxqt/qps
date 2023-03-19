/*
 * uidstr.cpp
 * This file is part of qps -- Qt-based visual process status monitor
 *
 * Copyright 1997-1999 Mattias Engdegård
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#include <pwd.h>
#include <grp.h>
#include <sys/types.h>

#include "uidstr.h"

QHash<int, QString> Uidstr::udict;
QHash<int, QString> Uidstr::gdict;

// return user name (possibly numeric)
QString Uidstr::userName(int uid) {
    auto it = udict.find(uid);
    if (it != udict.end()) {
        return it.value();
    }

    QString username;
    struct passwd *pw = getpwuid(uid);
    if (pw) {
        username = QString::fromLocal8Bit(pw->pw_name);
    } else {
        username = QString::number(uid);
    }

    udict.insert(uid, username);
    return username;
}

// return group name (possibly numeric)

QString Uidstr::groupName(int gid) {
    auto it = gdict.find(gid);
    if (it != gdict.end()) {
        return it.value();
    }

    QString groupname;
    struct group *gr = getgrgid(gid);
    if (gr) {
        groupname = QString::fromLocal8Bit(gr->gr_name);
    } else {
        groupname = QString::number(gid);
    }

    gdict.insert(gid, groupname);
    return groupname;
}
