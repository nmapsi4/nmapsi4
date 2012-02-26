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

DiscoverManager::DiscoverManager(MainWindow* parent)
: QObject(parent), m_ui(parent), m_ipCounter(0), m_uid(0), m_discoverIsActive(false)
{
#ifndef Q_WS_WIN
    m_uid = getuid();
#endif

    m_discoverHorizontalSplitter = new QSplitter(m_ui);
    m_discoverHorizontalSplitter->setOrientation(Qt::Horizontal);
    m_discoverHorizontalSplitter->addWidget(m_ui->frameDiscoverTree);
    m_discoverHorizontalSplitter->addWidget(m_ui->tabRightDiscover);

    // TODO: insert a vertical splitter and save it

    m_ui->tabUi->widget(m_ui->tabUi->indexOf(m_ui->tabDiscover))->layout()->addWidget(m_discoverHorizontalSplitter);

    QSettings settings("nmapsi4", "nmapsi4");

    if (!settings.value("discoverHorizontalSplitter").toByteArray().isEmpty())
    {
        m_discoverHorizontalSplitter->restoreState(settings.value("discoverHorizontalSplitter").toByteArray());
    }

    m_ui->treeTracePackets->setIconSize(QSize(22,22));
    //m_ui->treeTracePackets->header()->setResizeMode(0, QHeaderView::ResizeToContents);
}

DiscoverManager::~DiscoverManager()
{
    freelist<Discover*>::itemDeleteAll(m_listDiscover);
}

void DiscoverManager::syncSettings()
{
    QSettings settings("nmapsi4", "nmapsi4");
    settings.setValue("discoverHorizontalSplitter", m_discoverHorizontalSplitter->saveState());
}


void DiscoverManager::startDiscover()
{
    // take local interfaces
    m_ui->comboDiscover->clear();
    m_ui->comboDiscover->insertItem(0, "Select Interface");

    Discover *discoverPtr = new Discover(m_uid);
    foreach (const QNetworkInterface &interface, discoverPtr->getAllInterfaces(Discover::AllInterfaceWithAddress))
    {
        m_ui->comboDiscover->insertItem(1, interface.name());
    }

    delete discoverPtr;
}

void DiscoverManager::discoverIp(const QString& interface)
{
    // ip from interface and discover ip range
    Discover *discover_ = new Discover(m_uid);

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

void DiscoverManager::startDiscoverIpsFromRange()
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

    Discover *discoverPtr = new Discover(m_uid);
    m_listDiscover.push_back(discoverPtr);

    connect(discoverPtr, SIGNAL(fromListFinisched(QStringList,bool,QByteArray)),
            this, SLOT(endDiscoverIpsFromRange(QStringList,bool,QByteArray)));

    m_discoverIsActive = true;
    discoverPtr->fromList(ipList,this,parameters);
    m_ipCounter = ipList.size();
    m_ui->nseNumber->display(m_ipCounter);
}

void DiscoverManager::endDiscoverIpsFromRange(const QStringList hostname, bool state, const QByteArray callBuff)
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
        // active scan all action
        m_ui->m_collections->m_collectionsDiscover.value("scan-all")->setEnabled(true);

        QTreeWidgetItem *item = new QTreeWidgetItem(m_ui->treeDiscover);
        item->setIcon(0, QIcon(QString::fromUtf8(":/images/images/flag_green.png")));
        m_listTreeItemDiscover.push_back(item);
        item->setText(0, hostname[hostname.size()-1]);
        item->setText(1, tr("is Up"));

        while (!stream.atEnd())
        {
            QString line = stream.readLine();
            if ((line.startsWith(QLatin1String("RCVD")) || line.startsWith(QLatin1String("RECV")))
                && line.contains(hostname[hostname.size()-1]))
            {
                QTreeWidgetItem *packet = new QTreeWidgetItem(m_ui->treeTracePackets);
                packet->setText(0, line);
                packet->setForeground(0, QBrush(QColor(100, 162, 101)));
                packet->setToolTip(0,line);
                packet->setIcon(0,QIcon(QString::fromUtf8(":/images/images/document-preview-archive.png")));
                m_listTreePackets.push_back(packet);
            }
            else if (line.startsWith(QLatin1String("SENT")) && line.contains(hostname[hostname.size()-1]))
            {
                QTreeWidgetItem *packet = new QTreeWidgetItem(m_ui->treeTracePackets);
                packet->setText(0, line);
                packet->setToolTip(0,line);
                packet->setIcon(0,QIcon(QString::fromUtf8(":/images/images/document-preview-archive.png")));
                m_listTreePackets.push_back(packet);
            }
        }
    } /*else {
        //qDebug() << "DEBUG:: " << hostname[1] << " Ip is Up:: " << state;
    }*/

    if (!m_ipCounter)
    {
        freelist<Discover*>::itemDeleteAll(m_listDiscover);
        m_ui->startDiscoverButt->setEnabled(true);
        m_ui->stopDiscoverButt->setEnabled(false);
    }
}

void DiscoverManager::discoveryClear()
{
    freelist<QTreeWidgetItem*>::itemDeleteAll(m_listTreeItemDiscover);
    freelist<QTreeWidgetItem*>::itemDeleteAll(m_listTreePackets);
    freelist<Discover*>::itemDeleteAll(m_listDiscover);

    m_ui->m_collections->m_collectionsDiscover.value("scan-single")->setEnabled(false);
    m_ui->m_collections->m_collectionsDiscover.value("scan-all")->setEnabled(false);
}

void DiscoverManager::stopDiscover()
{
    m_ui->startDiscoverButt->setEnabled(true);
    m_ui->stopDiscoverButt->setEnabled(false);
    m_ipCounter = 0;
    m_discoverIsActive = false;
    m_ui->nseNumber->display(m_ipCounter);
    //emit signal
    emit killDiscover();
}

void DiscoverManager::scanSingleDiscoveredIp()
{
    if(m_ui->treeDiscover->currentItem())
    {
        m_ui->updateComboHostnameProperties();
        m_ui->hostEdit->insertItem(0, m_ui->treeDiscover->currentItem()->text(0));
        m_ui->startScan();
    }
}

void DiscoverManager::scanAllDiscoveredIps()
{
    if (m_listTreeItemDiscover.size())
    {
        foreach (QTreeWidgetItem *item, m_listTreeItemDiscover)
        {
            m_ui->updateComboHostnameProperties();
            m_ui->hostEdit->insertItem(0, item->text(0));
            m_ui->startScan();
        }
    }
}

void DiscoverManager::runtimeScanDiscover()
{
    if (!m_ui->m_collections->m_collectionsDiscover.value("scan-single")->isEnabled())
    {
        m_ui->m_collections->m_collectionsDiscover.value("scan-single")->setEnabled(true);
    }
}

void DiscoverManager::defaultDiscoverProbes()
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

        m_ui->discoverProbesCombo->insertItem(0, "--arp");
        m_ui->discoverProbesCombo->insertItem(1, "--icmp");
        m_ui->discoverProbesCombo->insertItem(2, "--tcp");
        m_ui->discoverProbesCombo->insertItem(3, "--udp");
        m_ui->discoverProbesCombo->insertItem(4, "--tr");
    }
    else
    {
        m_ui->discoverProbesCombo->setVisible(false);
        m_ui->labelProbesModes->setVisible(false);
    }
}
