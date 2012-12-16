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

#include "notify.h"

void Notify::startButtonNotify(PushButtonOrientated* button)
{
    button->setStyleSheet(nofifyStyleSheet);
}

void Notify::setCheckedNotify(PushButtonOrientated* button)
{
    button->setStyleSheet(verticalStyleSheet);
}

void Notify::clearButtonNotify(PushButtonOrientated* button)
{
    button->setStyleSheet("");
}

void Notify::notificationMessage(const QString& sender, const QString& message)
{
#if defined(USE_KDELIBS)
    KNotification::event(KNotification::Notification, "nmapsi4 - " + sender,
        message,
        QPixmap(QString::fromUtf8(":/images/icons/128x128/nmapsi4.png")),
        QApplication::activeWindow(),
        KNotification::CloseWhenWidgetActivated
    );
#endif
}
