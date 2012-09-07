/***************************************************************************
 *   Copyright (C) 2012 by Francesco Cecconi                               *
 *   francesco.cecconi@gmail.com                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License.        *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "notify.h"

void Notify::startButtonNotify(PushButtonOrientated* button)
{
    if (!button->isChecked()) {
        button->setStyleSheet(nofifyStyleSheet);
    }
}

void Notify::setCheckedNotify(PushButtonOrientated* button)
{
    button->setStyleSheet(verticalStyleSheet);
}

void Notify::clearButtonNotify(PushButtonOrientated* button)
{
    button->setStyleSheet("");
}

void Notify::sendQtNotification(const QString& address)
{
#if defined(Q_WS_X11)
    org::freedesktop::Notifications notification("org.freedesktop.Notifications",
            "/org/freedesktop/Notifications",
            QDBusConnection::sessionBus());

    if (notification.isValid()) {
        QVariantMap hints;
        hints["desktop-entry"] = "nmapsi4";

        QDBusPendingReply<uint> dbusReply = notification.Notify("nmapsi4", 0, "nmapsi4",
                                        QString("Scan Completed"),
                                        QString("Ip: ") + address, QStringList(), hints, -1);
        dbusReply.waitForFinished();

        if(!dbusReply.isError()) {
            return;
        }
    }
#endif
}
