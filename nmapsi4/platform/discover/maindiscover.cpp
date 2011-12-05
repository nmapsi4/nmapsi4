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
#include "memorytools.h"

pingInterface::mainDiscover::mainDiscover(int uid) 
    : ipState(false),
      uid_(uid)
{
    timer = new QTimer(this);
    connectState = false;
    ScanCounter = 0;
    threadLimit = 20;
}

pingInterface::mainDiscover::~mainDiscover()
{
    delete timer;
    m_ipSospended.clear();
    parameters_.clear();
    memory::freelist<QProcessThread*>::itemDeleteAllWithWait(_threadList);
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
    
    if (interface_.isValid()) 
    {
        return interface_.addressEntries();
    } 
    else 
    {
        return entryList_;
    }
}

void pingInterface::mainDiscover::isUp(const QStringList networkIpList, QObject *parent, QStringList parameters) 
{
    foreach (const QString& host, networkIpList)
    {
        isUp(host, parent, parameters);
    }
}

void pingInterface::mainDiscover::isUp(const QString networkIp, QObject *parent, QStringList parameters) 
{
    /*
     * start thread for discover ip state
     */
    //QByteArray pingBuffer;
    //QByteArray bufferError;
    m_parent = parent;
    parameters_ = parameters;
    // Create parameters list for npig
    parameters.append("-c 1");
    parameters.append("-v4");
    parameters.append(networkIp);
    
    if (threadLimit)
    {
        // acquire one element from thread counter
        threadLimit--;
        ScanCounter++;
        QPointer<QProcessThread> pingTh = new QProcessThread("nping",parameters);
        _threadList.push_back(pingTh);
        pingTh->start();
        connect(pingTh, SIGNAL(threadEnd(QStringList,QByteArray,QByteArray)),
            this, SLOT(threadReturn(QStringList,QByteArray,QByteArray)));
    } 
    else 
    {
        qDebug() << "DEBUG:: thread suspended:: " << networkIp;
        // create a QStringlist with address suspended
        m_ipSospended.append(networkIp);
    }
    
    if (!connectState)
    {
        connect(parent, SIGNAL(killDiscover()), this, SLOT(stopDiscover()));
    }
    
    // check suspended discover ip
    if (m_ipSospended.size() && !connectState)
    {
        connectState = true;
        connect(timer, SIGNAL(timeout()), this, SLOT(repeatScanner()));
        if (!timer->isActive())
        {
            timer->start(5000);
        }
    }
}

void pingInterface::mainDiscover::threadReturn(QStringList ipAddr, QByteArray ipBuffer, QByteArray bufferError)
{
    Q_UNUSED(bufferError);
    /*
     * Signal return, send data to discoverCalls
     */
    // increment thread limit, new ip discover is possible
    threadLimit++;
    // remove ip from counter
    ScanCounter--;
    QString buffString(ipBuffer);
    QTextStream buffStream(&buffString);
    QString buffLine;
    
    while(!buffStream.atEnd()) 
    {
        buffLine = buffStream.readLine();
        if (buffLine.startsWith(QLatin1String("RCVD")) || buffLine.startsWith(QLatin1String("RECV"))) 
        {
            emit endPing(ipAddr, true, ipBuffer);
            return;
        }
    }
    emit endPing(ipAddr, false, ipBuffer);
}

void pingInterface::mainDiscover::repeatScanner()
{
    /*
     * Recall discover for ip suspended 
     */
    qDebug() << "DEBUG:: scan Counter timer:: " << ScanCounter;
    if (!ScanCounter) 
    {
        disconnect(this, SLOT(repeatScanner()));
        disconnect(this, SLOT(stopDiscover()));

        connectState = false;
        timer->stop();
        int lengthMin_ = threadLimit;
    
        if (lengthMin_ > m_ipSospended.size())
        {
            lengthMin_ = m_ipSospended.size();
            for (int index = 0; index < lengthMin_; index++) 
            {
                isUp(m_ipSospended.takeFirst(), m_parent, parameters_);
            }
        } 
        else 
        {
            for (int index = 0; index < lengthMin_; index++) 
            {
                isUp(m_ipSospended.takeFirst(), m_parent, parameters_);
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
    timer->stop();
}
