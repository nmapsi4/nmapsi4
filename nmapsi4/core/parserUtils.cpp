/***************************************************************************
 *   Copyright (C) 2009-2010 by Francesco Cecconi                          *
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

#include "../mainwin.h"


void nmapClass::checkViewOS(const QString OSline, QTreeWidgetItem *itemOS) {

    if(OSline.contains("OS")) {
        QString tmp_os = OSline;
        if(OSline.contains("Linux")) {
            itemOS->setIcon(4, QIcon(QString::fromUtf8(":/images/images/os-logo/linux_logo.png")));
            itemOS->setText(4, "Linux OS");
        } else
        if(OSline.contains("Windows")) {
            itemOS->setIcon(4, QIcon(QString::fromUtf8(":/images/images/os-logo/windows_logo.png")));
            itemOS->setText(4, "MS Windows OS");
        } else
        if(OSline.contains("FreeBSD")) {
            itemOS->setIcon(4, QIcon(QString::fromUtf8(":/images/images/os-logo/freebsd_logo.png")));
            itemOS->setText(4, "FreeBSD OS");
        } else
        if(OSline.contains("OpenBSD")) {
            itemOS->setIcon(4, QIcon(QString::fromUtf8(":/images/images/os-logo/openbsd_logo.png")));
            itemOS->setText(4, "OpenBSD OS");
        } else
        if(OSline.contains("Solaris")) {
            itemOS->setIcon(4, QIcon(QString::fromUtf8(":/images/images/os-logo/solaris_logo.png")));
            itemOS->setText(4, "Sun Solaris OS");
        } else
        if(OSline.contains("Mac OS X")) {
            itemOS->setIcon(4, QIcon(QString::fromUtf8(":/images/images/os-logo/mac-os-x_logo.png")));
            itemOS->setText(4, "Mac OS X");
        } else {
            if(itemOS->text(4).isEmpty()) { // double check
                itemOS->setIcon(4, QIcon(QString::fromUtf8(":/images/images/no-os.png")));
            }
        }
    }
}

void nmapClass::runtimePars(QTreeWidgetItem *item, int column) { // SLOT
    Q_UNUSED(column);

    QStringList host = item->text(0).split(" ");

#ifndef PARSER_NO_DEBUG
    qDebug() << "DEBUG::Runtime::Split:: " << host.size();
    qDebug() << "DEBUG::Runtime::Host:: " << host[1];
#endif

    if(hostEdit->itemText(0).isEmpty()) {
        hostEdit->addItem(host[1]);
    } else {
        hostEdit->setItemText(0, host[1]);
    }

}
