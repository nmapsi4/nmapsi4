/***************************************************************************
 *   Copyright (C) 2011 by Francesco Cecconi                               *
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

#include "discover.h"
#include "memorytools.h"

discover::discover(int uid)
    : ipState(false),
      uid_(uid)
{
    timer = new QTimer(this);
    connectState = false;
    threadLimit = 20;
}

discover::~discover()
{
    delete timer;
    m_ipSospended.clear();
    parameters_.clear();
    memory::freelist<QProcessThread*>::itemDeleteAllWithWait(_threadList);
}

QList<QNetworkInterface> discover::getAllInterfaces(InterfaceOption option) const
{
    /*
     * return all local interfaces
     *
     */

    if (option == AllInterface)
    {
        return QNetworkInterface::allInterfaces();
    }

    QList<QNetworkInterface> interfacesWithAddress;

    foreach (const QNetworkInterface& address, QNetworkInterface::allInterfaces())
    {
        if (address.addressEntries().size())
        {
            interfacesWithAddress.append(address);
        }
    }

    return interfacesWithAddress;
}

QList<QNetworkAddressEntry> discover::getAddressEntries(QNetworkInterface interface) const
{
    /*
     * return all entries for a single QNeworkInterface
     */
    return interface.addressEntries();
}

QList<QNetworkAddressEntry> discover::getAddressEntries(const QString interfaceName) const
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

void discover::isUp(const QStringList networkIpList, QObject *parent, QStringList parameters)
{
    foreach (const QString& host, networkIpList)
    {
        isUp(host, parent, parameters);
    }
}

void discover::isUp(const QString networkIp, QObject *parent, QStringList parameters)
{
    /*
     * start thread for discover ip state
     */
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

        QPointer<QProcessThread> pingTh = new QProcessThread("nping",parameters);
        _threadList.push_back(pingTh);

        connect(pingTh, SIGNAL(threadEnd(QStringList,QByteArray,QByteArray)),
                this, SLOT(threadReturn(QStringList,QByteArray,QByteArray)));

        pingTh->start();
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

void discover::threadReturn(const QStringList ipAddr, QByteArray ipBuffer, QByteArray bufferError)
{
    Q_UNUSED(bufferError);
    /*
     * Signal return, send data to discoverCalls
     */

    // increment thread limit, new ip discover is possible
    threadLimit++;

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

void discover::repeatScanner()
{
    /*
     * Recall discover for ip suspended
     */
    qDebug() << "DEBUG:: scan Counter timer:: " << threadLimit;

    if (!threadLimit)
    {
        return;
    }

    disconnect(this, SLOT(repeatScanner()));
    disconnect(this, SLOT(stopDiscover()));

    connectState = false;
    timer->stop();
    int freeThreadSpace = 1;

    while (freeThreadSpace != threadLimit && freeThreadSpace <= m_ipSospended.size())
    {
        isUp(m_ipSospended.takeFirst(), m_parent, parameters_);
        freeThreadSpace++;
    }
}

void discover::stopDiscover()
{
    /*
     * disconnect timer slot and stop it
     */
    disconnect(this, SLOT(repeatScanner()));
    timer->stop();
}
