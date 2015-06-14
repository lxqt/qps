// lookup.h				emacs, this is written in -*-c++-*-
//
// This program is free software. See the file COPYING for details.
// Author: Mattias Engdegård, 1997-1999

#ifndef LOOKUP_H
#define LOOKUP_H

#include <qstring.h>
#include <qsocketnotifier.h>
#include "svec.h"
#include <QHash>

// queue (fifo) of unsigned ints (inet addresses)
class UintQueue
{
public:
    UintQueue();

    bool isEmpty() { return first == last; };
    void enqueue(unsigned x);
    unsigned dequeue();

private:
    Svec<unsigned> queue;
    int first;			// index of first item in queue
    int last;			// index where next item will be put (< first)
};

// list node for LRU cache of hostnames:
// This list is doubly-linked circular (to reduce the amount of special cases),
// with a head node that carries no data. In addition, each node is
// entered into a hash table for rapid lookup
class Hostnode {
public:
    Hostnode();
    Hostnode(unsigned addr);

    // these must be called on the head of the list
    void moveToFront(Hostnode *node);
    void deleteLast();
    void insertFirst(Hostnode *node);
    Hostnode *last() { return prev; };

    QString name;
    unsigned ipaddr;		// hash key
    Hostnode *next, *prev;
};

class Lookup : public QObject {
    Q_OBJECT
public:
    Lookup();
    ~Lookup();
    QString hostname(unsigned addr);

    static void initproctitle(char **argv, char **envp);

signals:
    void resolved(unsigned addr);

protected slots:
    void receive_result(int);
    void send_request(int);

protected:
    void request(unsigned addr);
    void do_child(int fd);

    static void setproctitle(const char *txt);

    static char *argv0;
    static int maxtitlelen;

    QHash<int,Hostnode*> hostdict;
    Hostnode hostlru;		// head of circular list
    UintQueue addrqueue;	// queue of addresses to lookup
    int outstanding;		// number of outstanding requests
    int sockfd;			// -1 if no helper process is running

    QSocketNotifier *readsn, *writesn;

    static const unsigned hostname_cache_size = 400; // max names to cache
};

#endif	// LOOKUP_H
