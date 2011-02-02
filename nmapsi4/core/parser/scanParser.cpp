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

#include "../../mainwin.h"

void nmapClass::showParserObj(int indexObj) {
    // Clear widget
    qDeleteAll(itemListScan);
    listWscan->clear();
    itemListScan.clear();

    QString noInfo("not Discovered");

    if (!parserObjList[indexObj]->getUptime().isEmpty()) {
        QTreeWidgetItem *root = new QTreeWidgetItem(treeHostDet);
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/messagebox_info.png")));
        root->setSizeHint(0, QSize(22, 22));
        itemListScan.push_front(root);
        QString tmpElem_ = parserObjList[indexObj]->getUptime();
        tmpElem_.prepend("Uptime: ");
        root->setText(0, tmpElem_);
        root->setToolTip(0, tmpElem_);
    } else {
        QTreeWidgetItem *root = new QTreeWidgetItem(treeHostDet);
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/messagebox_info.png")));
        root->setSizeHint(0, QSize(22, 22));
        itemListScan.push_front(root);
        QString tmpElem_("Uptime: ");
        tmpElem_.append(noInfo);
        root->setText(0, tmpElem_);
    }

    if (!parserObjList[indexObj]->getTcpSequence().isEmpty()) {
        QTreeWidgetItem *root = new QTreeWidgetItem(treeHostDet);
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/messagebox_info.png")));
        root->setSizeHint(0, QSize(22, 22));
        itemListScan.push_front(root);
        QString tmpElem_ = parserObjList[indexObj]->getTcpSequence();
        tmpElem_.prepend("TcpSequence: ");
        root->setText(0, tmpElem_);
        root->setToolTip(0, tmpElem_);
    } else {
        QTreeWidgetItem *root = new QTreeWidgetItem(treeHostDet);
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/messagebox_info.png")));
        root->setSizeHint(0, QSize(22, 22));
        itemListScan.push_front(root);
        QString tmpElem_("TcpSequence: ");
        tmpElem_.append(noInfo);
        root->setText(0, tmpElem_);
    }

    if (!parserObjList[indexObj]->getRunning().isEmpty()) {
        QTreeWidgetItem *root = new QTreeWidgetItem(treeHostDet);
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/messagebox_info.png")));
        root->setSizeHint(0, QSize(22, 22));
        itemListScan.push_front(root);
        QString tmpElem_ = parserObjList[indexObj]->getRunning();
        tmpElem_.prepend("Running: ");
        root->setText(0, tmpElem_);
        root->setToolTip(0, tmpElem_);
    } else {
        QTreeWidgetItem *root = new QTreeWidgetItem(treeHostDet);
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/messagebox_info.png")));
        root->setSizeHint(0, QSize(22, 22));
        itemListScan.push_front(root);
        QString tmpElem_("Running: ");
        tmpElem_.append(noInfo);
        root->setText(0, tmpElem_);
    }

    if (!parserObjList[indexObj]->getDeviceType().isEmpty()) {
        QTreeWidgetItem *root = new QTreeWidgetItem(treeHostDet);
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/messagebox_info.png")));
        root->setSizeHint(0, QSize(22, 22));
        itemListScan.push_front(root);
        QString tmpElem_ = parserObjList[indexObj]->getDeviceType();
        tmpElem_.prepend("DeviceType: ");
        root->setText(0, tmpElem_);
        root->setToolTip(0, tmpElem_);
    } else {
        QTreeWidgetItem *root = new QTreeWidgetItem(treeHostDet);
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/messagebox_info.png")));
        root->setSizeHint(0, QSize(22, 22));
        itemListScan.push_front(root);
        QString tmpElem_("DeviceType: ");
        tmpElem_.append(noInfo);
        root->setText(0, tmpElem_);
    }

    qDeleteAll(objElem);
    GItree->clear();
    objElem.clear();

    QStringList listServices_ = parserObjList[indexObj]->getServices();
    const int listServicesSize_ = listServices_.size();

    for(int index=0; index < listServicesSize_; index++) {
        QTreeWidgetItem *objItem = new QTreeWidgetItem(GItree);
        objElem.push_front(objItem);
        objItem->setSizeHint(0, QSize(22, 22));
        objItem->setText(0,listServices_[index]);
    }

    // TODO FIX check for stop scan
    // TODO FIX check for clear History

    QStringList listOpen_ = parserObjList[indexObj]->getPortOpen();
    QStringList listClose_ = parserObjList[indexObj]->getPortClose();
    QStringList listFilterd_ = parserObjList[indexObj]->getPortFiltered();
    QStringList listScanInfo_ = parserObjList[indexObj]->getScanInfo();
    QStringList listNssInfo_ = parserObjList[indexObj]->getNssInfo();
    QString noDes = tr("No description");

    // Show open ports
    const int listOpenSize = listOpen_.size();
    for(int index=0; index < listOpenSize; index++) {
        QTreeWidgetItem *root = new QTreeWidgetItem(listWscan);
        itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/flag_green.png")));
        root->setForeground(0, QBrush(QColor(0, 0, 255, 127)));
        QStringList split = listOpen_[index].split(" ", QString::SkipEmptyParts);
        root->setText(0, split[0]);
        root->setText(1, split[1]);
        root->setText(2, split[2]);
        if(split.size() == 4) {
            root->setText(3, split[3]);
            root->setToolTip(3, split[3]);
        } else if (split.size() > 4) {
            QString lineDescription_("");
            for(int index=3; index < split.size(); index++) {
                lineDescription_.append(split[index]);
                lineDescription_.append(" ");
            }
            root->setText(3, lineDescription_);
            root->setToolTip(3, lineDescription_);
        } else {
            root->setText(3, noDes);
        }
    }

    // Show Close ports
    const int listCloseSize = listClose_.size();
    for(int index=0; index < listCloseSize; index++) {
        QTreeWidgetItem *root = new QTreeWidgetItem(listWscan);
        itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/flag_red.png")));
        QStringList split = listClose_[index].split(" ", QString::SkipEmptyParts);
        root->setText(0, split[0]);
        root->setText(1, split[1]);
        root->setText(2, split[2]);
        if(split.size() == 4) {
            root->setText(3, split[3]);
            root->setToolTip(3, split[3]);
        } else if (split.size() > 4) {
            QString lineDescription_("");
            for(int index=3; index < split.size(); index++) {
                lineDescription_.append(split[index]);
                lineDescription_.append(" ");
            }
            root->setText(3, lineDescription_);
            root->setToolTip(3, lineDescription_);
        }
    }

    // Show Filtered ports
    const int listFilteredSize = listFilterd_.size();
    for(int index=0; index < listFilteredSize; index++) {
        QTreeWidgetItem *root = new QTreeWidgetItem(listWscan);
        itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/flag_yellow.png")));
        root->setForeground(0, QBrush(QColor(255, 0, 0, 127)));
        QStringList split = listFilterd_[index].split(" ", QString::SkipEmptyParts);
        root->setText(0, split[0]);
        root->setText(1, split[1]);
        root->setText(2, split[2]);
        if(split.size() == 4) {
            root->setText(3, split[3]);
            root->setToolTip(3, split[3]);
        } else if (split.size() > 4) {
            QString lineDescription_("");
            for(int index=3; index < split.size(); index++) {
                lineDescription_.append(split[index]);
                lineDescription_.append(" ");
            }
            root->setText(3, lineDescription_);
            root->setToolTip(3, lineDescription_);
        }
    }

    // Show scan Info
    const int listScanInfoSize = listScanInfo_.size();
    for(int index=0; index < listScanInfoSize; index++) {
        QTreeWidgetItem *root = new QTreeWidgetItem(treeWinfo);
        itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/messagebox_info.png")));
        root->setText(0, listScanInfo_[index]);
        root->setToolTip(0, listScanInfo_[index]);
    }

    // Show Nss Info
    const int listNssSize = listNssInfo_.size();
    for(int index=0; index < listNssSize; index++) {
        QTreeWidgetItem *root = new QTreeWidgetItem(treeNSS);
        itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/traceroute.png")));
        root->setText(0, listNssInfo_[index]);
        root->setToolTip(0, listNssInfo_[index]);
    }

    // call plugins parser
    showParserObjPlugins(indexObj);
}

