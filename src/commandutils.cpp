// commandutils.cpp
//
// This program is free software. See the file COPYING for details.
// Author: Mattias Engdegård, 1997-1999

#include "commandutils.h"

#include <command.h>

extern QList<Command *> commands;

int find_command(QString s)
{
    for (int i = 0; i < commands.size(); i++)
        if (s == commands[i]->name)
            return i;
    return -1;
}

// DEL has "&" end of the string ?
bool hasAmpersand(QString cmdline)
{
    QString str;
    int len;
    str = cmdline.simplified();

    if (str == "%update")
        return true; // internal command

    len = str.length();
    if (str[len - 1] == '&')
        return true;
    else
        return false;
}

void check_command(int idx) {}

//
void check_commandAll()
{
    int i, idx;

    return;
    for (int i = 0; i < commands.size(); i++)
    {
        if (hasAmpersand(commands[i]->cmdline) == false)
            commands[i]->cmdline.append("&");
    }
}

// after read ~/.qpsrc
void add_default_command()
{

    int idx;

    /*
    idx=find_command("Update");
    if (idx>=0)
            commands[idx]->cmdline="%update";
    else
            commands.add(new Command("Update","%update",true));
    */

    /*
     *	PAUSED
    idx=find_command("Quit");
    if (idx>=0)
            commands[idx]->cmdline="killall qps";
    else	commands.add(new Command("Quit","killall qps",false));
    */

    // check_commandAll(); DEL?
}

