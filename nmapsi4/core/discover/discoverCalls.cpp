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

namespace varDiscover {
    QList<mainDiscover*> listDiscover;
    QList<QTreeWidgetItem*> listTreeItemDiscover;
    int ipCounter = 0;
}

void nmapClass::startDiscover() 
{
    // take local interfaces
    comboDiscover->clear();
    comboDiscover->insertItem(0, "Select Interface");
    
    mainDiscover *discover = new mainDiscover(uid);
    foreach (const QNetworkInterface &interface, discover->getAllInterfaces()) {
	comboDiscover->insertItem(1, interface.name());
    }

    delete discover;
    
}

void nmapClass::discoverIp(const QString& interface) 
{
    // ip from interface and discover ip range (FIXME unify the if)
    mainDiscover *discover_ = new mainDiscover(uid);
    
    QList<QNetworkAddressEntry> entryList_ = discover_->getAddressEntries(interface);
    
    if (!entryList_.isEmpty()) {
	QNetworkAddressEntry entry_ = discover_->getAddressEntries(interface).first();
	QString ipString_ = entry_.ip().toString();
	
	if (!ipString_.contains("127.0.0.1")) {
	    // active discover buttton
	    startDiscoverButt->setEnabled(true);
	    QStringList ipSplit_ = ipString_.split('.');
	    int ipStart = ipSplit_[3].toInt();
	    ipSplit_.removeLast();
	    QString ipClass_ = ipSplit_.join(".");
	    ipClass_.append(".");
	    lineIpDiscover->setText(ipClass_);
	    spinBeginDiscover->setValue(ipStart);
	    spinEndDiscover->setValue(ipStart+10);
	    //qDebug() << "DEBUG:: Discover Interfaces addr:: " << entry_.ip();
	} else {
 	    // reset discover value
	    lineIpDiscover->clear();
	    startDiscoverButt->setEnabled(false);
	    spinBeginDiscover->setValue(0);
	    spinEndDiscover->setValue(0);
	}
    } else {
	// reset discover value
	lineIpDiscover->clear();
	startDiscoverButt->setEnabled(false);
	spinBeginDiscover->setValue(0);
	spinEndDiscover->setValue(0);
    }
    
    delete discover_;
}

void nmapClass::discoverIpState()
{
    // start ip discover
    // disable start discover button
    startDiscoverButt->setEnabled(false);
    stopDiscoverButt->setEnabled(true);
    // clear tree discover
    cleanDiscovery();
    memoryTools *memTools = new memoryTools();
    memTools->itemDeleteAll(varDiscover::listDiscover);
    
    QStringList ipList_;
    for (int index = spinBeginDiscover->value(); index <= spinEndDiscover->value(); ++index) {
	QString tmpIp_ = lineIpDiscover->text().append(QString::number(index));
	ipList_.append(tmpIp_);
    }
    
    mainDiscover *discover = new mainDiscover(uid);
    varDiscover::listDiscover.push_back(discover);
    connect(discover, SIGNAL(endPing(QStringList,bool)), this, SLOT(pingResult(QStringList,bool)));
    
    foreach (const QString &token, ipList_) {
	discover->isUp(token,this);
	varDiscover::ipCounter++;
	nseNumber->display(varDiscover::ipCounter);
    }
    
    delete memTools;
}

void nmapClass::pingResult(QStringList hostname, bool state)
{
    // decrement ping ip counter
    --varDiscover::ipCounter;
    nseNumber->display(varDiscover::ipCounter);
    // set values in treeDiscover widget
    treeDiscover->setIconSize(QSize(24,24));
    if (state) {
	QTreeWidgetItem *item = new QTreeWidgetItem(treeDiscover);
	item->setIcon(0, QIcon(QString::fromUtf8(":/images/images/document-preview-archive.png")));
	item->setIcon(1, QIcon(QString::fromUtf8(":/images/images/flag_green.png")));
	varDiscover::listTreeItemDiscover.push_back(item);
	item->setText(0, hostname[hostname.size()-1]);
	item->setText(1, tr("is Up"));
    } else {
	//qDebug() << "DEBUG:: " << hostname[1] << " Ip is Up:: " << state;
    }
    
    if (!varDiscover::ipCounter) {
	memoryTools *memTools = new memoryTools();
	memTools->itemDeleteAll(varDiscover::listDiscover);
	delete memTools;
	startDiscoverButt->setEnabled(true);
	stopDiscoverButt->setEnabled(false);
    }
}

void nmapClass::cleanDiscovery()
{
    memoryTools *memTools = new memoryTools();
    memTools->itemDeleteAll(varDiscover::listTreeItemDiscover);
    delete memTools;
}

void nmapClass::stopDiscover()
{
    startDiscoverButt->setEnabled(true);
    stopDiscoverButt->setEnabled(false);
    varDiscover::ipCounter = 0;
    nseNumber->display(varDiscover::ipCounter);
    // don't kill running QProcess is no necessary
    //emit killPingScan();
    emit killDiscover();
}
