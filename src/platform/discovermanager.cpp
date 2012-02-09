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

#include "discovermanager.h"
#include "mainwindow.h"

discoverManager::discoverManager(MainWindow* parent)
: QObject(parent), m_ui(parent), m_ipCounter(0), m_uid(0), m_discoverIsActive(false)
{
#ifndef Q_WS_WIN
    m_uid = getuid();
#endif
}

discoverManager::~discoverManager()
{
    discoveryClear();
}

void discoverManager::startDiscover()
{
    // take local interfaces
    m_ui->comboDiscover->clear();
    m_ui->comboDiscover->insertItem(0, "Select Interface");

    discover *discoverPtr = new discover(m_uid);
    foreach (const QNetworkInterface &interface, discoverPtr->getAllInterfaces(discover::AllInterfaceWithAddress))
    {
        m_ui->comboDiscover->insertItem(1, interface.name());
    }

    delete discoverPtr;
}

void discoverManager::discoverIp(const QString& interface)
{
    // ip from interface and discover ip range
    discover *discover_ = new discover(m_uid);

    QList<QNetworkAddressEntry> entryList_ = discover_->getAddressEntries(interface);

    if (!entryList_.isEmpty())
    {
        QNetworkAddressEntry entry_ = discover_->getAddressEntries(interface).first();
        QString ipString_ = entry_.ip().toString();

        QHostAddress address(ipString_);

        // TODO:: ipv6 support
        if (!ipString_.contains("127.0.0.1") && address.protocol() != QAbstractSocket::IPv6Protocol)
        {
            // active discover buttton
            m_ui->startDiscoverButt->setEnabled(true);
            QStringList ipSplit_ = ipString_.split('.');
            int ipStart = ipSplit_[3].toInt();
            ipSplit_.removeLast();
            QString ipClass_ = ipSplit_.join(".");
            ipClass_.append(".");
            m_ui->lineIpDiscover->setText(ipClass_);
            m_ui->spinBeginDiscover->setValue(ipStart);
            m_ui->spinEndDiscover->setValue(ipStart+10);
        }
        else
        {
            // reset discover value
            m_ui->lineIpDiscover->clear();
            m_ui->startDiscoverButt->setEnabled(false);
            m_ui->spinBeginDiscover->setValue(0);
            m_ui->spinEndDiscover->setValue(0);
        }
    }
    else
    {
        // reset discover value
        m_ui->lineIpDiscover->clear();
        m_ui->startDiscoverButt->setEnabled(false);
        m_ui->spinBeginDiscover->setValue(0);
        m_ui->spinEndDiscover->setValue(0);
    }

    delete discover_;
}

void discoverManager::discoverIpState()
{
    // start ip discover
    // disable start discover button
    m_ui->startDiscoverButt->setEnabled(false);
    m_ui->stopDiscoverButt->setEnabled(true);
    // clear tree discover
    discoveryClear();

    QStringList ipList;
    for (int index = m_ui->spinBeginDiscover->value(); index <= m_ui->spinEndDiscover->value(); ++index)
    {
        QString tmpIp_ = m_ui->lineIpDiscover->text().append(QString::number(index));
        ipList.append(tmpIp_);
    }

    QStringList parameters;
    if (!m_uid)
    {
        parameters.append(m_ui->discoverProbesCombo->currentText());
    }
    else
    {
        parameters.append("--tcp-connect");
    }

    discover *discoverPtr = new discover(m_uid);
    m_listDiscover.push_back(discoverPtr);

    connect(discoverPtr, SIGNAL(endPing(QStringList,bool,QByteArray)),
            this, SLOT(pingResult(QStringList,bool,QByteArray)));

    m_discoverIsActive = true;
    discoverPtr->isUp(ipList,this,parameters);
    m_ipCounter = ipList.size();
    m_ui->nseNumber->display(m_ipCounter);
}

void discoverManager::pingResult(const QStringList hostname, bool state, const QByteArray callBuff)
{
    // decrement ping ip counter
    if (m_discoverIsActive)
    {
        --m_ipCounter;
    }

    m_ui->nseNumber->display(m_ipCounter);
    // set values in treeDiscover widget
    m_ui->treeDiscover->setIconSize(QSize(24,24));
    QTextStream stream(callBuff);

    if (state)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(m_ui->treeDiscover);
        item->setIcon(0, QIcon(QString::fromUtf8(":/images/images/document-preview-archive.png")));
        item->setIcon(1, QIcon(QString::fromUtf8(":/images/images/flag_green.png")));
        m_listTreeItemDiscover.push_back(item);
        item->setText(0, hostname[hostname.size()-1]);
        item->setText(1, tr("is Up"));

        while (!stream.atEnd())
        {
            QString line = stream.readLine();
            if ((line.startsWith(QLatin1String("RCVD")) || line.startsWith(QLatin1String("RECV")))
                && line.contains(hostname[hostname.size()-1]))
            {
                m_recvList.push_back(line);
            }
            else if (line.startsWith(QLatin1String("SENT")) && line.contains(hostname[hostname.size()-1]))
            {
                m_sendList.push_back(line);
            }
        }
    } /*else {
        //qDebug() << "DEBUG:: " << hostname[1] << " Ip is Up:: " << state;
    }*/

    if (!m_ipCounter)
    {
        freelist<discover*>::itemDeleteAll(m_listDiscover);
        m_ui->startDiscoverButt->setEnabled(true);
        m_ui->stopDiscoverButt->setEnabled(false);
    }
}

void discoverManager::discoveryClear()
{
    freelist<QTreeWidgetItem*>::itemDeleteAll(m_listTreeItemDiscover);
    freelist<discover*>::itemDeleteAll(m_listDiscover);
    m_recvList.clear();
    m_sendList.clear();
    m_ui->discoverScanButt->setEnabled(false);
}

void discoverManager::stopDiscover()
{
    m_ui->startDiscoverButt->setEnabled(true);
    m_ui->stopDiscoverButt->setEnabled(false);
    m_ipCounter = 0;
    m_discoverIsActive = false;
    m_ui->nseNumber->display(m_ipCounter);
    //emit signal
    emit killDiscover();
}

void discoverManager::updateSRdata()
{
    int index = m_ui->treeDiscover->indexOfTopLevelItem(m_ui->treeDiscover->currentItem());
    m_ui->textDiscoverRec->setText(m_recvList[index]);
    m_ui->textDiscoverSend->setText(m_sendList[index]);
}

void discoverManager::callScanDiscover()
{
    if(m_ui->treeDiscover->currentItem())
    {
        m_ui->updateComboHostnameProperties();
        m_ui->hostEdit->insertItem(0, m_ui->treeDiscover->currentItem()->text(0));
        m_ui->SWscan->setCurrentIndex(0);
        m_ui->startScan();
    }
}

void discoverManager::runtimeScanDiscover()
{
    // show discover send/recv data
    updateSRdata();

    if (!m_ui->discoverScanButt->isEnabled())
    {
        m_ui->discoverScanButt->setEnabled(true);
    }
}

void discoverManager::defaultDiscoverProbes()
{
    /* Modes Probe
    * --tcp,--udp,--arp,--tr
    */
    if (!m_uid)
    {
        if (!m_ui->discoverProbesCombo->isVisible())
        {
            m_ui->discoverProbesCombo->setVisible(true);
            m_ui->labelProbesModes->setVisible(true);
        }
        m_ui->discoverProbesCombo->insertItem(0, "--icmp");
        m_ui->discoverProbesCombo->insertItem(1, "--tcp");
        m_ui->discoverProbesCombo->insertItem(2, "--udp");
        m_ui->discoverProbesCombo->insertItem(3, "--arp");
        m_ui->discoverProbesCombo->insertItem(4, "--tr");
    }
    else
    {
        m_ui->discoverProbesCombo->setVisible(false);
        m_ui->labelProbesModes->setVisible(false);
    }
}
