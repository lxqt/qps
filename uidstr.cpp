// uidstr.C
//
// This program is free software. See the file COPYING for details.
// Author: Mattias Engdegård, 1997-1999

#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "uidstr.h"

QHash<int,char*> Uidstr::udict;
QHash<int,char*> Uidstr::gdict;

// return user name (possibly numeric)
QString Uidstr::userName(int uid)
{
	char *p = udict.value(uid,NULL);
	if(!p) {
		struct passwd *pw = getpwuid(uid);
		if(!pw) {
			p = (char *)malloc(11);
			sprintf(p, "%d", uid);
		} else
			p = strdup(pw->pw_name);
		udict.insert(uid, p);
		//if(udict.count() > udict.size() * 3) udict.resize(udict.count());
	}
	QString s(p);
	return s;
}

// return group name (possibly numeric)

QString Uidstr::groupName(int gid)
{
    char *p = gdict[gid];
    if(!p) {
	struct group *gr = getgrgid(gid);
	if(!gr) {
	    p = (char *)malloc(11);
	    sprintf(p, "%d", gid);
	} else
	    p = strdup(gr->gr_name);
	gdict.insert(gid, p);
	//if(gdict.count() > gdict.size() * 3) gdict.resize(gdict.count()); 
	}
    QString s(p);
    return s;
}

