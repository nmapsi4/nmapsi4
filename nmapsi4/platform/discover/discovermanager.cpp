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
#include "mainwin.h"

discoverManager::discoverManager(nmapClass* parent): QObject(parent),
    _ui(parent),
    ipCounter(0),
    _discoverIsActive(false),
    _uid(0)
{
#ifndef Q_WS_WIN
    _uid = getuid();
#endif
}

discoverManager::~discoverManager()
{
    discoveryClear();
}

void discoverManager::startDiscover()
{
    // take local interfaces
    _ui->comboDiscover->clear();
    _ui->comboDiscover->insertItem(0, "Select Interface");

    discover *discoverPtr = new discover(_uid);
    foreach (const QNetworkInterface &interface, discoverPtr->getAllInterfaces(discover::AllInterfaceWithAddress))
    {
        _ui->comboDiscover->insertItem(1, interface.name());
    }

    delete discoverPtr;
}

void discoverManager::discoverIp(const QString& interface)
{
    // ip from interface and discover ip range
    discover *discover_ = new discover(_uid);

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
            _ui->startDiscoverButt->setEnabled(true);
            QStringList ipSplit_ = ipString_.split('.');
            int ipStart = ipSplit_[3].toInt();
            ipSplit_.removeLast();
            QString ipClass_ = ipSplit_.join(".");
            ipClass_.append(".");
            _ui->lineIpDiscover->setText(ipClass_);
            _ui->spinBeginDiscover->setValue(ipStart);
            _ui->spinEndDiscover->setValue(ipStart+10);
        }
        else
        {
            // reset discover value
            _ui->lineIpDiscover->clear();
            _ui->startDiscoverButt->setEnabled(false);
            _ui->spinBeginDiscover->setValue(0);
            _ui->spinEndDiscover->setValue(0);
        }
    }
    else
    {
        // reset discover value
        _ui->lineIpDiscover->clear();
        _ui->startDiscoverButt->setEnabled(false);
        _ui->spinBeginDiscover->setValue(0);
        _ui->spinEndDiscover->setValue(0);
    }

    delete discover_;
}

void discoverManager::discoverIpState()
{
    // start ip discover
    // disable start discover button
    _ui->startDiscoverButt->setEnabled(false);
    _ui->stopDiscoverButt->setEnabled(true);
    // clear tree discover
    discoveryClear();

    QStringList ipList_;
    for (int index = _ui->spinBeginDiscover->value(); index <= _ui->spinEndDiscover->value(); ++index)
    {
        QString tmpIp_ = _ui->lineIpDiscover->text().append(QString::number(index));
        ipList_.append(tmpIp_);
    }

    QStringList parameters;
    if (!_uid)
    {
        parameters.append(_ui->discoverProbesCombo->currentText());
    }
    else
    {
        parameters.append("--tcp-connect");
    }

    discover *discoverPtr = new discover(_uid);
    listDiscover.push_back(discoverPtr);
    connect(discoverPtr, SIGNAL(endPing(QStringList,bool,QByteArray)),
            this, SLOT(pingResult(QStringList,bool,QByteArray)));

    _discoverIsActive = true;
    discoverPtr->isUp(ipList_,this,parameters);
    ipCounter = ipList_.size();
    _ui->nseNumber->display(ipCounter);
}

void discoverManager::pingResult(QStringList hostname, bool state, const QByteArray callBuff)
{
    // decrement ping ip counter
    if (_discoverIsActive)
    {
        --ipCounter;
    }
    _ui->nseNumber->display(ipCounter);
    // set values in treeDiscover widget
    _ui->treeDiscover->setIconSize(QSize(24,24));
    QTextStream stream(callBuff);

    if (state)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(_ui->treeDiscover);
        item->setIcon(0, QIcon(QString::fromUtf8(":/images/images/document-preview-archive.png")));
        item->setIcon(1, QIcon(QString::fromUtf8(":/images/images/flag_green.png")));
        listTreeItemDiscover.push_back(item);
        item->setText(0, hostname[hostname.size()-1]);
        item->setText(1, tr("is Up"));

        while (!stream.atEnd())
        {
            QString line = stream.readLine();
            if ((line.startsWith(QLatin1String("RCVD")) || line.startsWith(QLatin1String("RECV")))
                && line.contains(hostname[hostname.size()-1]))
            {
                recvList.push_back(line);
            }
            else if (line.startsWith(QLatin1String("SENT")) && line.contains(hostname[hostname.size()-1]))
            {
                sendList.push_back(line);
            }
        }
    } /*else {
        //qDebug() << "DEBUG:: " << hostname[1] << " Ip is Up:: " << state;
    }*/

    if (!ipCounter)
    {
        freelist<discover*>::itemDeleteAll(listDiscover);
        _ui->startDiscoverButt->setEnabled(true);
        _ui->stopDiscoverButt->setEnabled(false);
    }
}

void discoverManager::discoveryClear()
{
    freelist<QTreeWidgetItem*>::itemDeleteAll(listTreeItemDiscover);
    freelist<discover*>::itemDeleteAll(listDiscover);
    recvList.clear();
    sendList.clear();
    _ui->discoverScanButt->setEnabled(false);
}

void discoverManager::stopDiscover()
{
    _ui->startDiscoverButt->setEnabled(true);
    _ui->stopDiscoverButt->setEnabled(false);
    ipCounter = 0;
    _discoverIsActive = false;
    _ui->nseNumber->display(ipCounter);
    //emit signal
    emit killDiscover();
}

void discoverManager::updateSRdata()
{
    int index = _ui->treeDiscover->indexOfTopLevelItem(_ui->treeDiscover->currentItem());
    _ui->textDiscoverRec->setText(recvList[index]);
    _ui->textDiscoverSend->setText(sendList[index]);
}

void discoverManager::callScanDiscover()
{
    if(_ui->treeDiscover->currentItem())
    {
        _ui->updateFontHost();
        // clear history setItemText fails
        _ui->hostEdit->insertItem(0, _ui->treeDiscover->currentItem()->text(0));
        _ui->SWscan->setCurrentIndex(0);
        _ui->startScan();
    }
}

void discoverManager::runtimeScanDiscover()
{
    // show discover send/recv data
    updateSRdata();

    if (!_ui->discoverScanButt->isEnabled())
    {
        _ui->discoverScanButt->setEnabled(true);
    }
}

void discoverManager::defaultDiscoverProbes()
{
    /* Modes Probe
    * --tcp,--udp,--arp,--tr
    */
    if (!_uid)
    {
        if (!_ui->discoverProbesCombo->isVisible())
        {
            _ui->discoverProbesCombo->setVisible(true);
            _ui->labelProbesModes->setVisible(true);
        }
        _ui->discoverProbesCombo->insertItem(0, "--icmp");
        _ui->discoverProbesCombo->insertItem(1, "--tcp");
        _ui->discoverProbesCombo->insertItem(2, "--udp");
        _ui->discoverProbesCombo->insertItem(3, "--arp");
        _ui->discoverProbesCombo->insertItem(4, "--tr");
    }
    else
    {
        _ui->discoverProbesCombo->setVisible(false);
        _ui->labelProbesModes->setVisible(false);
    }
}
