// This program is free software. See the file COPYING for details.
// Author:  Mattias Engdeg?rd, 1997-1999
//          fasthyun@magicn.com 2005-2012
//          daehyun.yang@gmail.com 2015-

#include "qpsapp.h"

void QpsApp::saveState(QSessionManager &manager)
{
    //	printf("saveState()\n");
    // manager.setRestartHint(QSessionManager::RestartIfRunning);
    // manager.release();
}

// this is called  when X Logout
// closeEvent() never called !!
void QpsApp::commitData(QSessionManager &manager)
{
    /*
    printf("commitData()\n");
    manager.setRestartHint(QSessionManager::RestartIfRunning);
    qps->flag_exit=true;  // ready to Logout
    qps->save_settings() ;
    manager.release();
    sleep(2);
    return;
     if (manager.allowsInteraction()) {
     int ret = QMessageBox::warning(
                 qps,
                 tr("My Application"),
                 tr("Save changes to document?"),
                 QMessageBox::Save | QMessageBox::Discard |
  QMessageBox::Cancel);

     switch (ret) {
     case QMessageBox::Save:
         manager.release();
  //          if (!saveDocument())    manager.cancel();
         break;
     case QMessageBox::Discard:
         break;
     case QMessageBox::Cancel:
     default:
         manager.cancel();
     }
  } else {

                    manager.release();

     // we did not get permission to interact, then
     // do something reasonable instead
  }
  */
    /*
  //DEL sm.release();
    qDebug("Qps: Session saved\n");
  //	sm.cancel();
    //sm.setRestartHint (QSessionManager::RestartIfRunning);
    QApplication::commitData(sm);
  */
}
