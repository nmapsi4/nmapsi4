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

void nmapClass::startDiscover() {
    // TODO test for discover
    mainDiscover *discover = new mainDiscover();
    foreach (QNetworkInterface interface, discover->getAllInterfaces()) {
	qDebug() << "DEBUG:: Discover Interfaces name:: " << interface.humanReadableName();
	discoverIp(discover,interface);
    }
    delete discover;
}

void nmapClass::discoverIp(mainDiscover *discover, QNetworkInterface interface) { // SLOT
    // TODO remove input parameters, read and create combo Interfaces
    foreach (QNetworkAddressEntry entry, discover->getAddressEntries(interface)) {
	qDebug() << "DEBUG:: Discover Interfaces addr:: " << entry.ip();
	// TODO write data in lineEdit ip
    }
}