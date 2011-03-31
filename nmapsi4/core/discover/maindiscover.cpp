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


#include "maindiscover.h"

mainDiscover::mainDiscover() 
    : ipState(false)
{

}

mainDiscover::~mainDiscover()
{
    
}

QList<QNetworkInterface> mainDiscover::getAllInterfaces() const
{
    return QNetworkInterface::allInterfaces();
}

QList<QNetworkAddressEntry> mainDiscover::getAddressEntries(QNetworkInterface interface) const
{
    return interface.addressEntries();
}

void mainDiscover::isUp(const QString networkIp, QObject *parent) {
    // start thread for discover ip state
    QByteArray pingBuffer_;
    QStringList listPar_;
    // Create parameters list for npig
    listPar_.append("--tcp-connect");
    listPar_.append(networkIp);
    QPointer<pingThread> pingTh = new pingThread(pingBuffer_, listPar_, parent);
    pingTh->start();
    connect(pingTh, SIGNAL(threadEnd(QStringList, QByteArray, pingThread*)),
            this, SLOT(threadReturn(QStringList, QByteArray, pingThread*)));
}

void mainDiscover::threadReturn(QStringList ipAddr, QByteArray ipBuffer, pingThread *ptrThread)
{
    // clear thread
    ptrThread->quit();
    ptrThread->wait();
    delete ptrThread;
    QString buffString(ipBuffer);
    QTextStream buffStream(&buffString);
    QString buffLine;
    
    while(!buffStream.atEnd()) {
        buffLine = buffStream.readLine();
	if (buffLine.contains("completed") || buffLine.contains("Connection refused")) {
	    emit endPing(ipAddr, true);
	    return;
	}
    }
    emit endPing(ipAddr, false);
}

