/*
Copyright 2011-2012  Francesco Cecconi <francesco.cecconi@gmail.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "discover.h"
#include "memorytools.h"
#include "discovermanager.h"

Discover::Discover(int uid)
    : m_ipState(false),
      m_uid(uid),
      m_parent(0)
{
    m_timer = new QTimer(this);
    m_connectState = false;

    QSettings settings("nmapsi4", "nmapsi4");
    m_threadLimit = settings.value("maxDiscoverProcess", 20).toInt();
}

Discover::~Discover()
{
    delete m_timer;
    memory::freelist<ProcessThread*>::itemDeleteAllWithWait(m_threadList);
}

QList<QNetworkInterface> Discover::getAllInterfaces(InterfaceOption option) const
{
    /*
     * return all local interfaces
     *
     */

    if (option == AllInterface) {
        return QNetworkInterface::allInterfaces();
    }

    QList<QNetworkInterface> interfacesWithAddress;

    foreach(const QNetworkInterface & address, QNetworkInterface::allInterfaces()) {
        if (address.addressEntries().size()) {
            interfacesWithAddress.append(address);
        }
    }

    return interfacesWithAddress;
}

QList<QNetworkAddressEntry> Discover::getAddressEntries(QNetworkInterface interface) const
{
    /*
     * return all entries for a single QNeworkInterface
     */
    return interface.addressEntries();
}

QList<QNetworkAddressEntry> Discover::getAddressEntries(const QString interfaceName) const
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

void Discover::fromList(const QStringList networkIpList, DiscoverManager *parent, QStringList parameters)
{
    foreach(const QString & host, networkIpList) {
        fromList(host, parent, parameters);
    }
}

void Discover::fromList(const QString networkIp, DiscoverManager *parent, QStringList parameters)
{
    /*
     * start thread for discover ip state
     */
    m_parent = parent;
    m_parameters = parameters;
    // Create parameters list for npig
    parameters.append("-c 1");
    parameters.append("-v4");
    parameters.append(networkIp);

    if (m_threadLimit) {
        // acquire one element from thread counter
        m_threadLimit--;

        QPointer<ProcessThread> pingTh = new ProcessThread("nping", parameters);
        m_threadList.push_back(pingTh);

        connect(pingTh, SIGNAL(threadEnd(QStringList,QByteArray,QByteArray)),
                this, SLOT(fromListReturn(QStringList,QByteArray,QByteArray)));

        pingTh->start();
    } else {
        qDebug() << "DEBUG:: thread suspended:: " << networkIp;
        // create a QStringlist with address suspended
        m_ipSospended.append(networkIp);
    }

    if (!m_connectState) {
        connect(parent, SIGNAL(killDiscoverFromIpsRange()), this, SLOT(stopDiscoverFromList()));
    }

    // check suspended discover ip
    if (m_ipSospended.size() && !m_connectState) {
        m_connectState = true;
        connect(m_timer, SIGNAL(timeout()), this, SLOT(repeatScanner()));
        if (!m_timer->isActive()) {
            m_timer->start(5000);
        }
    }
}

void Discover::fromListReturn(const QStringList ipAddr, QByteArray ipBuffer, QByteArray bufferError)
{
    Q_UNUSED(bufferError);
    /*
     * Signal return, send data to discoverCalls
     */

    // increment thread limit, new ip discover is possible
    m_threadLimit++;

    QString buffString(ipBuffer);
    QTextStream buffStream(&buffString);
    QString buffLine;

    while (!buffStream.atEnd()) {
        buffLine = buffStream.readLine();
        if (buffLine.startsWith(QLatin1String("RCVD")) || buffLine.startsWith(QLatin1String("RECV"))) {
            emit fromListFinisched(ipAddr, true, ipBuffer);
            return;
        }
    }
    emit fromListFinisched(ipAddr, false, ipBuffer);
}

void Discover::repeatScanner()
{
    /*
     * Recall discover for ip suspended
     */
    qDebug() << "DEBUG:: scan Counter timer:: " << m_threadLimit;

    if (!m_threadLimit) {
        return;
    }

    disconnect(this, SLOT(repeatScanner()));
    disconnect(this, SLOT(stopDiscoverFromList()));

    m_connectState = false;
    m_timer->stop();
    int freeThreadSpace = 1;

    while (freeThreadSpace <= m_threadLimit && freeThreadSpace <= m_ipSospended.size()) {
        fromList(m_ipSospended.takeFirst(), m_parent, m_parameters);
        freeThreadSpace++;
    }
}

void Discover::stopDiscoverFromList()
{
    /*
     * disconnect timer slot and stop it
     */
    disconnect(this, SLOT(repeatScanner()));
    m_timer->stop();
}

void Discover::fromCIDR(const QString networkCIDR, QStringList parameters, DiscoverManager* parent, IpProtocolType type)
{
    parameters.append("-c 1");
    parameters.append("-v4");

    if (type == IPv6) {
        // active support for ipv6 address
        parameters << "-6";
    }

    parameters.append(networkCIDR);

    QPointer<ProcessThread> thread = new ProcessThread("nping", parameters);

    connect(thread, SIGNAL(flowFromThread(QString,QByteArray)),
            this, SLOT(currentCIDRValue(QString,QByteArray)));
    connect(thread, SIGNAL(threadEnd(QStringList,QByteArray,QByteArray)),
            this, SLOT(endCIDR(QStringList,QByteArray,QByteArray)));
    connect(parent, SIGNAL(killDiscoverFromCIDR()),
            this, SLOT(stopDiscoverFromCIDR()));

    m_threadList.push_back(thread);

    thread->start();
}

void Discover::stopDiscoverFromCIDR()
{
    memory::freelist<ProcessThread*>::itemDeleteAllWithWait(m_threadList);
}

void Discover::currentCIDRValue(const QString parameters, QByteArray data)
{
    QTextStream currentValues(&data);
    QString currentLine;

    while (!currentValues.atEnd()) {
        currentLine = currentValues.readLine();

        if (currentLine.startsWith(QLatin1String("RCVD"))
                || currentLine.startsWith(QLatin1String("RECV"))
                || currentLine.startsWith(QLatin1String("SENT"))) {
            emit cidrCurrentValue(parameters, currentLine);
        }
    }
}

void Discover::endCIDR(const QStringList ipAddr, QByteArray ipBuffer, QByteArray bufferError)
{
    emit cidrFinisced(ipAddr, ipBuffer, bufferError);
}
