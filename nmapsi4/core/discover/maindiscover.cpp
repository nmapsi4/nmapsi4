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

namespace discoverLayer {
    QStringList m_ipSospended;
    QObject *m_parent;
    int ScanCounter;
    int threadLimit;
    QTimer *timer;
    bool connectState;
}

pingInterface::mainDiscover::mainDiscover() 
    : ipState(false)
{
    discoverLayer::timer = new QTimer(this);
    discoverLayer::connectState = false;
    discoverLayer::ScanCounter = 0;
    discoverLayer::threadLimit = 20;
}

pingInterface::mainDiscover::~mainDiscover()
{
    delete discoverLayer::timer;
    discoverLayer::m_ipSospended.clear();
}

QList<QNetworkInterface> pingInterface::mainDiscover::getAllInterfaces() const
{
    /*
     * return all loca interfaces
     */
    return QNetworkInterface::allInterfaces();
}

QList<QNetworkAddressEntry> pingInterface::mainDiscover::getAddressEntries(QNetworkInterface interface) const
{
    /*
     * return all entries for a single QNeworkInterface
     */
    return interface.addressEntries();
}

QList<QNetworkAddressEntry> pingInterface::mainDiscover::getAddressEntries(const QString interfaceName) const
{
    /*
     * return all entries for a single interface name
     */
    QNetworkInterface interface_ = QNetworkInterface::interfaceFromName(interfaceName);
    
    QList<QNetworkAddressEntry> entryList_;
    
    if (interface_.isValid()) {
	return interface_.addressEntries();
    } else {
	return entryList_;
    }
}

void pingInterface::mainDiscover::isUp(const QString networkIp, QObject *parent) {
    /*
     * start thread for discover ip state
     */
    QByteArray pingBuffer_;
    QStringList listPar_;
    discoverLayer::m_parent = parent;
    // Create parameters list for npig
    listPar_.append("--tcp-connect");
    listPar_.append(networkIp);
    
    if (discoverLayer::threadLimit) {
	// acquire one element from thread counter
	discoverLayer::threadLimit--;
	discoverLayer::ScanCounter++;
	QPointer<pingThread> pingTh = new pingThread(pingBuffer_, listPar_, parent);
	pingTh->start();
	connect(pingTh, SIGNAL(threadEnd(QStringList, QByteArray, pingThread*)),
		this, SLOT(threadReturn(QStringList, QByteArray, pingThread*)));
    } else {
	qDebug() << "DEBUG:: thread suspended:: " << networkIp;
	// create a QStringlist with address suspended
	discoverLayer::m_ipSospended.append(networkIp);
    }
    
    if (!discoverLayer::connectState) {
	connect(parent, SIGNAL(killDiscover()), this, SLOT(stopDiscover()));
    }
    
    // check suspended discover ip
    if (discoverLayer::m_ipSospended.size() && !discoverLayer::connectState) {
	discoverLayer::connectState = true;
	connect(discoverLayer::timer, SIGNAL(timeout()), this, SLOT(repeatScanner()));
	if (!discoverLayer::timer->isActive()) {
	    discoverLayer::timer->start(5000);
	}
    }
}

void pingInterface::mainDiscover::threadReturn(QStringList ipAddr, QByteArray ipBuffer, pingThread *ptrThread)
{
    /*
     * Signal return, send data to discoverCalls
     */
    // clear thread
    ptrThread->quit();
    ptrThread->wait();
    delete ptrThread;
    
    // increment thread limit, new ip discover is possible
    discoverLayer::threadLimit++;
    // remove ip from counter
    discoverLayer::ScanCounter--;
    qDebug() << "DEBUG:: thread end:: " << ipAddr[1];
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

void pingInterface::mainDiscover::repeatScanner()
{
    /*
     * Recall discover for ip suspended 
     */
    qDebug() << "DEBUG:: scan Counter timer:: " << discoverLayer::ScanCounter;
    if (!discoverLayer::ScanCounter) {
	disconnect(this, SLOT(repeatScanner()));
	discoverLayer::connectState = false;
	discoverLayer::timer->stop();
	int lengthMin_ = discoverLayer::threadLimit;
    
	if (lengthMin_ > discoverLayer::m_ipSospended.size()) {
	    lengthMin_ = discoverLayer::m_ipSospended.size();
	    for (int index = 0; index < lengthMin_; index++) {
		isUp(discoverLayer::m_ipSospended.takeFirst(), discoverLayer::m_parent);
	    }
	} else {
	    for (int index = 0; index < lengthMin_; index++) {
		isUp(discoverLayer::m_ipSospended.takeFirst(), discoverLayer::m_parent);
	    }
	}
    }
}

void pingInterface::mainDiscover::stopDiscover()
{
    /*
     * disconnect timer slot and stop it
     */
    disconnect(this, SLOT(repeatScanner()));
    discoverLayer::timer->stop();
}

