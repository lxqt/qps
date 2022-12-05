/*
 * trayicon.h - system-independent trayicon class (adapted from Qt example)
 * This file is part of qps -- Qt-based visual process status monitor
 *
 * Copyright 2003  Justin Karneges
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 */

#include "global.h"     // Qps *qps;
#include "trayicon.h"

TrayIcon::TrayIcon(const QPixmap &icon, const QString &tooltip, QMenu *popup,
                   QWidget *parent)
    : QSystemTrayIcon(parent),
      pm(icon)
{
    QSystemTrayIcon::setIcon(icon);
    QSystemTrayIcon::setContextMenu(popup);
    QSystemTrayIcon::setToolTip(tooltip);

    if (!pm.width() || !pm.height())
        pm = QPixmap(45, 45);
    hasSysTray = QSystemTrayIcon::isSystemTrayAvailable();
}

TrayIcon::~TrayIcon() {}

QPixmap TrayIcon::icon() const { return pm; }

void TrayIcon::sysInstall()
{
    if (hasSysTray == true)
    {
        show();
    }
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
