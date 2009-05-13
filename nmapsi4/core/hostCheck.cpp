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
    // fix lookup scan and create a new free scan
    // create a user box for scan (lookup or not) type
    // create a function for check user preference
    // create a group scan with check address in address the / character
    //

    QHostInfo info = QHostInfo::fromName(hostname);

    if(info.error() != QHostInfo::NoError && info.error() != QHostInfo::UnknownError) {
         qDebug() << "Lookup failed:" << info.errorString();
         QMessageBox::warning(this, "NmapSI4", tr("Wrong Address\n"), tr("Close"));
         return;
     }

    // fix a wrong address if possible
    if(info.addresses().size() == 1) {
         QHostAddress address = (info.addresses())[0];
         hostname = address.toString();
    }

    // DEBUG List
    foreach (QHostAddress address, info.addresses()) {
        qDebug() << "Found address:" << address.toString();
    }

    this->scan(hostname);
}
