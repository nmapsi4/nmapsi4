/*
    Copyright (C) 2011  Francesco Cecconi <francesco.cecconi@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "../../mainwin.h"

namespace varDiscover {
    QList<mainDiscover*> listDiscover;
    int ipCounter = 0;
}

void nmapClass::startDiscover() {
    // TODO test for discover, clear pointer
    mainDiscover *discover = new mainDiscover();
    foreach (const QNetworkInterface &interface, discover->getAllInterfaces()) {
	qDebug() << "DEBUG:: Discover Interfaces name:: " << interface.humanReadableName();
	discoverIp(discover,interface);
    }
    delete discover;
    
}

void nmapClass::discoverIp(mainDiscover *discover, QNetworkInterface interface) { // SLOT
    // TODO remove input parameters, read and create combo Interfaces
    foreach (const QNetworkAddressEntry &entry, discover->getAddressEntries(interface)) {
	qDebug() << "DEBUG:: Discover Interfaces addr:: " << entry.ip();
	// TODO write data in lineEdit ip
    }
}

void nmapClass::discoverIpState()
{
    //TODO: take values from QWidget
    QStringList ipList;
    ipList.append("192.168.1.4");
    ipList.append("192.168.1.2");
    ipList.append("192.168.1.3");
    ipList.append("192.168.1.1");
    mainDiscover *discover = new mainDiscover();
    varDiscover::listDiscover.push_back(discover);
    connect(discover, SIGNAL(endPing(QStringList,bool)), this, SLOT(pingResult(QStringList,bool)));
    
    foreach (const QString &token, ipList) {
	discover->isUp(token,this);
	varDiscover::ipCounter++;
    }
}

void nmapClass::pingResult(QStringList hostname, bool state)
{
    // decrement ping ip counter
    --varDiscover::ipCounter;
    
    if (state) {
	qDebug() << "DEBUG:: " << hostname[1] << " Ip is Up:: " << state;
    } else {
	qDebug() << "DEBUG:: " << hostname[1] << " Ip is Up:: " << state;
    }
    
    if (!varDiscover::ipCounter) {
	itemDeleteAll(varDiscover::listDiscover);
    }
}
