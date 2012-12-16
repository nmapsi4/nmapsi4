/*
Copyright 2012  Francesco Cecconi <francesco.cecconi@gmail.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef NOTIFY_H
#define NOTIFY_H

#include "style.h"
#include "pushbuttonorientated.h"
#include <config.h>

// qt includes
#include <QtCore/QDebug>
#include <QtGui/QApplication>

// kde includes
#if defined(USE_KDELIBS)
#include <KNotification>
#endif


class Notify
{

public:
    static void startButtonNotify(PushButtonOrientated* button);
    static void setCheckedNotify(PushButtonOrientated* button);
    static void clearButtonNotify(PushButtonOrientated* button);
    static void notificationMessage(const QString& hostName, const QString& message);
};

#endif // NOTIFY_H
