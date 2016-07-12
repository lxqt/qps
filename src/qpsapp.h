// qpsapp.h
//
// This program is free software. See the file COPYING for details.
// Author:  Mattias Engdegård, 1997-1999
//          fasthyun@magicn.com 2005-2012
//          daehyun.yang@gmail.com 2015-

#ifndef QPSAPP_H
#define QPSAPP_H

#include <QApplication>

// MOD!!!: For systray update.
// this trick very suck, but I can't find a better solution.
class QpsApp : public QApplication
{
  public:
    QpsApp(int &argc, char **argv) : QApplication(argc, argv){};
    void commitData(QSessionManager &sm);
    void saveState(QSessionManager &manager);

    /*
        virtual bool x11EventFilter ( XEvent *xev ){
                // catch X11 event for systray_update !! which event?
                ///if(trayicon!=NULL) return
       trayicon->checkNewTrayEvent(xev);
                return false; // events to qt.
        }; */
};

#endif // QPSAPP_H
