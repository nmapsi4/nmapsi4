/***************************************************************************
 *   Copyright (C) 2009 by Francesco Cecconi                               *
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

void nmapClass::scanLookup(QString hostname) {
    // TODO
    // create a user box for scan (lookup or not) type
    // create a function for check user preference
    //

    QHostInfo info = QHostInfo::fromName(hostname);

    if(info.error() != QHostInfo::NoError && info.error() != QHostInfo::UnknownError) {
#ifndef MAIN_NO_DEBUG
         qDebug() << "Lookup failed:" << info.errorString();
#endif
         QMessageBox::warning(this, "NmapSI4", tr("Wrong Address\n"), tr("Close"));
         this->delMonitorHost(scanMonitor,hostname);
         return;
     }

    // fix a wrong address if possible
    if(info.addresses().size() == 1) {
         QHostAddress address = (info.addresses())[0];
         hostname = address.toString();
    }

    // TODO::[DEBUG List] create a iplist
    foreach (QHostAddress address, info.addresses()) {
        qDebug() << "Found address:" << address.toString();
    }

    this->scan(hostname);
}
