// tablefield.h				emacs, this is written in -*-c++-*-
//
// This program is free software. See the file COPYING for details.
// Author: Mattias Engdegård, 1997-1999

#ifndef TABLEFIELD_H
#define TABLEFIELD_H

struct TableField
{
    const char *name;
    int width;
    int gap;
    int align;
    const char *tooltip;
};

#endif // TABLEFIELD_H
