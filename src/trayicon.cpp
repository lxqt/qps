/*
        really sucking codes.. sorry..
        from psi ,  modified and cleand by fasthyun@magicn.com
        trayicon_x11.cpp - X11 trayicon (for use with KDE and GNOME)
        Copyright (C) 2003  Justin Karneges
        GNOME2 Notification Area support: Tomasz Sterna

 */

#include <QApplication> // *****should be First!!!
#include "global.h"     // Qps *qps;
#include "trayicon.h"

#include <QMouseEvent>
#include <QDesktopWidget>
#include <QPainter>
#include <QToolTip>

// QIcon icon = iconComboBox->itemIcon(index);
//   trayIcon->setIcon(icon);

extern bool flag_session_start;
//----------------------------------------------------------------------------
// common stuff
//----------------------------------------------------------------------------
// for Gnome2 Notification Area
static int dock_xerror = 0;
extern bool flag_xcompmgr;

#define SYSTEM_TRAY_REQUEST_DOCK 0
#define SYSTEM_TRAY_BEGIN_MESSAGE 1
#define SYSTEM_TRAY_CANCEL_MESSAGE 2

TrayIcon::TrayIcon(const QPixmap &icon, const QString &tooltip, QMenu *popup,
                   QWidget *parent, const char *name)
    : QSystemTrayIcon(0 /* shoud be NULL!! */), pop(popup), pm(icon),
      tip(tooltip)
{
    flag_show_tip = false;
    flag_systray_ready = false;
    inTray = false;
    isWMDock = false;

    QSystemTrayIcon::setIcon(icon);
    QSystemTrayIcon::setContextMenu(popup);

    if (!pm.width() || !pm.height())
        pm = QPixmap(45, 45);
}

/*!
  Removes the icon from the system tray and frees all allocated resources.
*/
TrayIcon::~TrayIcon() { sysRemove(); }

/*!
  Sets the context menu to \a popup. The context menu will pop up when the
  user clicks the system tray entry with the right mouse button.
*/
void TrayIcon::setPopup(QMenu *popup) { pop = popup; }

/*!
  Returns the current popup menu.
*/
QMenu *TrayIcon::popup() const { return pop; }

QPixmap TrayIcon::icon() const { return pm; }

//----------------------------------------------------------------------------
// TrayIcon
//----------------------------------------------------------------------------
// DRAFT Code (by fasthyun@magicn.com)
void TrayIcon::init_TrayIconFreeDesktop() {}

void TrayIcon::init_WindowMakerDock() {}

void TrayIcon::sysInstall()
{
    /// boolSysTray=hasSysTray();

    if (hasSysTray() == true)
    {
        show();
    }
    /*
    if(hasSysTray()==false)
    {
            setFixedSize(46,46); // setIconSize
            init_WindowMakerDock();
            setSysTray(true);
    } */
}

bool TrayIcon::hasSysTray() { return QSystemTrayIcon::isSystemTrayAvailable(); }

void TrayIcon::sysRemove()
{
    // printf("Qps: sysRemove\n");
    inTray = false;
    /// boolSysTray=isWMDock=false;
    hide();
}

/*!
  update systray icon
  called by qps::refresh()
*/
void TrayIcon::setIcon(const QPixmap &pix)
{
    QSystemTrayIcon::setIcon(pix);
    if (isVisible() == false)
        return;
    pm = pix;
}

/*!
  \property TrayIcon::toolTip
  \brief the tooltip for the system tray entry

  On some systems, the tooltip's length is limited and will be truncated as
  necessary.
*/
void TrayIcon::setToolTip(const QString &tooltip)
{
    if (pop->isVisible())
        flag_show_tip = false;
    if (flag_show_tip)
        QToolTip::showText(tip_pos, tooltip);
    else
        QToolTip::hideText();
    tip = tooltip;
}

void TrayIcon::leaveEvent(QEvent *) { flag_show_tip = false; }

void TrayIcon::mouseMoveEvent(QMouseEvent *e)
{
    // printf("move\n");
    // QToolTip::showText(QPoint(0,0),"" );
    // QToolTip::showText(e->globalPos(),tip );
    tip_pos = e->globalPos();
    flag_show_tip = true;
    QToolTip::showText(tip_pos, tip);
    e->accept();
}

void TrayIcon::mousePressEvent(QMouseEvent *e)
{
    // test_xapp();
    // This is for X11, menus appear on mouse press
    // I'm not sure whether Mac should be here or below.. Somebody check?
    switch (e->button())
    {
    case Qt::RightButton:
        if (pop)
        {
            pop->popup(e->globalPos());
            e->accept();
        }
        break;
    case Qt::LeftButton:
    case Qt::MidButton:
        // emit clicked( e->globalPos(), e->button() );
        emit clicked(e->globalPos());
    default:
        break;
    }
    e->ignore();
}

void TrayIcon::mouseReleaseEvent(QMouseEvent *e)
{
    //	printf("mouseReleaseEvent\n");
    switch (e->button())
    {
    case Qt::RightButton:
        if (pop)
        {
            // Necessary to make keyboard focus
            // and menu closing work on Windows.
            //	e->accept();
        }
        break;
    case Qt::LeftButton:
    case Qt::MidButton:
        // emit clicked( e->globalPos(), e->button() );
        // emit clicked( e->globalPos());
        break;
    default:
        break;
    }
    e->ignore();
}

void TrayIcon::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
        emit doubleClicked(e->globalPos());
    e->accept();
}

// this will be called  before shown !!
void TrayIcon::paintEvent(QPaintEvent *)
{
    /*
    //printf("paintEvent()\n");
    QPainter p(this);
    int w,h;
    if(isVisible()==false) // ** important to prevent X11 Error !!
    {
    //	printf("paintEvent(): hidden\n");
            return;
    }
    w=width()/2 ;
    h=height()/2;
    p.drawPixmap(w - pm.width()/2, h - pm.height()/2, pm); */
}

// for session logout ?? never called
void TrayIcon::closeEvent(QCloseEvent *e)
{
    printf("TrayIcon::closeEvent()\n");
    e->accept();
}

// called after size changed
void TrayIcon::resizeEvent(QResizeEvent *e)
{
    int w, h;
    // printf("TrayIcon::resizeEvent(): w=%d,h=%d\n",width(),height());
    // if(isVisible()==false) return;  // X11 error !!
    /// w=width();h=height();
    if (isWMDock)
    {
        /// w=w-14;h=h-14;
    }
    // qps->setIconSize(w,h);
}

void QPS_SHOW();
