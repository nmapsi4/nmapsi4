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
    QString noInfo("not Discovered");

    if(!parserObjList[indexObj]->getUptime().isEmpty()) {
        lineInfouptime->clear();
        lineInfouptime->setText(parserObjList[indexObj]->getUptime());
    } else {
        lineInfouptime->setText(noInfo);
    }

    if(!parserObjList[indexObj]->getTcpSequence().isEmpty()) {
        lineInfotcpsequence->setText(parserObjList[indexObj]->getTcpSequence());
    } else {
        lineInfotcpsequence->setText(noInfo);
    }

    if(!parserObjList[indexObj]->getRunning().isEmpty()) {
        lineInforunning->setText(parserObjList[indexObj]->getRunning());
    } else {
        lineInforunning->setText(noInfo);
    }

    if(!parserObjList[indexObj]->getDeviceType().isEmpty()) {
        lineInfodevice->setText(parserObjList[indexObj]->getDeviceType());
    } else {
        lineInfodevice->setText(noInfo);
    }

    qDeleteAll(objElem);
    GItree->clear();
    objElem.clear();

    for(int index=0; index < parserObjList[indexObj]->getServices().size(); index++) {
        QTreeWidgetItem *objItem = new QTreeWidgetItem(GItree);
        objElem.push_front(objItem);
        objItem->setSizeHint(0, QSize(22, 22));
        objItem->setText(0,parserObjList[indexObj]->getServices()[index]);
    }

    // TODO FIX check for stop scan
    // TODO FIX check for clear History
    qDeleteAll(itemListScan);
    listWscan->clear();
    itemListScan.clear();

    QStringList listOpen_ = parserObjList[indexObj]->getPortOpen();
    QStringList listClose_ = parserObjList[indexObj]->getPortClose();
    QStringList listFilterd_ = parserObjList[indexObj]->getPortFiltered();
    QString noDes = tr("No description");

    // Show open ports
    for(int index=0; index < listOpen_.size(); index++) {
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
    for(int index=0; index < listClose_.size(); index++) {
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
    for(int index=0; index < listFilterd_.size(); index++) {
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
}

