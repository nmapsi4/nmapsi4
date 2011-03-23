/***************************************************************************
 *   Copyright (C) 2009-2011 by Francesco Cecconi                          *
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

void nmapClass::scanLookup(QHostInfo info, int state, const QString hostname) {

#ifndef MAIN_NO_DEBUG
    qDebug() << "scanLookup::flag:: " << state;
#endif

    if(state == -1) {
        QMessageBox::warning(this, "NmapSI4", tr("Wrong Address\n"), tr("Close"));
        this->delMonitorHost(scanMonitor,hostname);
        return;
    }
    
    parserObjUtil* elemObjUtil = new parserObjUtil();

#ifndef MAIN_NO_DEBUG
    qDebug() << "scanLookup::Found address DNS:: " << info.hostName();
#endif

    elemObjUtil->setHostName(hostname);
    const int infoSize_ = info.addresses().size();
    for(int index=0; index < infoSize_; index++) {
#ifndef MAIN_NO_DEBUG
        qDebug() << "scanLookup::Found address:: " << info.addresses()[index].toString();
#endif
	elemObjUtil->setInfoLookup(info.addresses()[index].toString());
    }

    parserObjUtilList.append(elemObjUtil);
    this->scan(hostname);
}

bool nmapClass::isDns(QString hostname) {
    bool state_;
    QRegExp rx_("*.*.*.*");
    rx_.setPatternSyntax(QRegExp::Wildcard);
    (rx_.exactMatch(hostname) == true) ? state_=false : state_=true;

#ifndef MAIN_NO_DEBUG
    qDebug() << "nmapsi4/isDns():: " << hostname << state_;
#endif
    return state_;
}

QString nmapClass::clearHost(const QString hostname) {
    // check for wrong dns address
    // (http:// ftp:// sftp:// https://)[dns/ip]
    QString hostNew_(hostname);
    if(hostname.startsWith("http://")) {
        hostNew_.remove("http://");
        return hostNew_;
    }

    if(hostname.startsWith("https://")) {
        hostNew_.remove("https://");
        return hostNew_;
    }

    if(hostname.startsWith("ftp://")) {
        hostNew_.remove("ftp://");
        return hostNew_;
    }

    if(hostname.startsWith("sftp://")) {
        hostNew_.remove("sftp://");
        return hostNew_;
    }

    return hostname;
}
