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

void nmapClass::showParserObj(int indexObj) 
{
    // Clear widget
    itemDeleteAll(itemListScan);
    itemDeleteAll(objElem);
    listWscan->clear();
    GItree->clear();
    treeNSS->clear();
    int treeMainIndex_ = treeMain->indexOfTopLevelItem(treeMain->currentItem());
    comboScanLog->setCurrentIndex(treeMainIndex_+1);

    QString noInfo("not Discovered");
    QStringList listMainInfo_ = parserObjList[indexObj]->getMainInfo();
    
    foreach (const QString &token, listMainInfo_) {
	QTreeWidgetItem *root = new QTreeWidgetItem(treeHostDet);
        itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/messagebox_info.png")));
        root->setText(0, token);
        root->setToolTip(0, token);
    }
    
    QStringList listOpen_ = parserObjList[indexObj]->getPortOpen();
    QStringList listClose_ = parserObjList[indexObj]->getPortClose();
    QStringList listFilterd_ = parserObjList[indexObj]->getPortFiltered();
    QStringList listNssInfo_ = parserObjList[indexObj]->getNssInfo();
    QStringList listLogInfo_ = parserObjList[indexObj]->getFullScanLog();
    QStringList listErrorInfo_ = parserObjList[indexObj]->getErrorScan();
    QString noDes = tr("No description");
    // clear combo Vulnerabilities
    comboVuln->clear();
    comboVuln->insertItem(0,"Services");

    // Show open ports
    foreach (const QString &token, listOpen_) {
        QTreeWidgetItem *root = new QTreeWidgetItem(listWscan);
        itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/flag_green.png")));
        root->setForeground(0, QBrush(QColor(0, 0, 255, 127)));
        QStringList split = token.split(' ', QString::SkipEmptyParts);
        root->setText(0, split[0]);
        root->setText(1, split[1]);
        root->setText(2, split[2]);
        if (split.size() == 4) {
            root->setText(3, split[3]);
            root->setToolTip(3, split[3]);
	    if (!split[3].isEmpty()) {
		comboVuln->addItem(split[3]);
	    }
        } else if (split.size() > 4) {
            QString lineDescription_("");
            for(int index=3; index < split.size(); index++) {
                lineDescription_.append(split[index]);
                lineDescription_.append(" ");
            }
            root->setText(3, lineDescription_);
            root->setToolTip(3, lineDescription_);
	    //load comboVuln
	    if (!lineDescription_.isEmpty()) {
		comboVuln->addItem(lineDescription_);
	    }
        } else {
            root->setText(3, noDes);
        }
    }

    // Show Close ports
    foreach (const QString &token, listClose_) {
        QTreeWidgetItem *root = new QTreeWidgetItem(listWscan);
        itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/flag_red.png")));
        QStringList split = token.split(' ', QString::SkipEmptyParts);
        root->setText(0, split[0]);
        root->setText(1, split[1]);
        root->setText(2, split[2]);
        if (split.size() == 4) {
            root->setText(3, split[3]);
            root->setToolTip(3, split[3]);
	    if (!split[3].isEmpty()) {
		comboVuln->addItem(split[3]);
	    }
        } else if (split.size() > 4) {
            QString lineDescription_("");
            for(int index=3; index < split.size(); index++) {
                lineDescription_.append(split[index]);
                lineDescription_.append(" ");
            }
            root->setText(3, lineDescription_);
            root->setToolTip(3, lineDescription_);
	    //load comboVuln
	    if (!lineDescription_.isEmpty()) {
		comboVuln->addItem(lineDescription_);
	    }
        } else {
	    root->setText(3, noDes);
	}
    }

    // Show Filtered ports
    foreach (const QString &token, listFilterd_) {
        QTreeWidgetItem *root = new QTreeWidgetItem(listWscan);
        itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/flag_yellow.png")));
        root->setForeground(0, QBrush(QColor(255, 0, 0, 127)));
        QStringList split = token.split(' ', QString::SkipEmptyParts);
        root->setText(0, split[0]);
        root->setText(1, split[1]);
        root->setText(2, split[2]);
        if (split.size() == 4) {
            root->setText(3, split[3]);
            root->setToolTip(3, split[3]);
	    if (!split[3].isEmpty()) {
		comboVuln->addItem(split[3]);
	    }
        } else if (split.size() > 4) {
            QString lineDescription_("");
            for(int index=3; index < split.size(); index++) {
                lineDescription_.append(split[index]);
                lineDescription_.append(" ");
            }
            root->setText(3, lineDescription_);
            root->setToolTip(3, lineDescription_);
	    //load comboVuln
	    if (!lineDescription_.isEmpty()) {
		comboVuln->addItem(lineDescription_);
	    }
        } else {
	    root->setText(3, noDes);
	}
    }
    
    QStringList listServices_ = parserObjList[indexObj]->getServices();
    
    // show services
    foreach (const QString &token, listServices_) {
	if (!listWscan->findItems(token, Qt::MatchExactly, 2)[0]->text(3).contains(noDes)
	) {
	    QTreeWidgetItem *objItem = new QTreeWidgetItem(GItree);
	    objItem->setSizeHint(0, QSize(22, 22));
	    objItem->setIcon(0, QIcon(QString::fromUtf8(":/images/images/network_local.png")));
	    objElem.push_front(objItem);
	    objItem->setText(0,token);
	}
    }

    // Show Nss Info
    foreach (const QString &token, listNssInfo_) {
        QTreeWidgetItem *root = new QTreeWidgetItem(treeNSS);
        itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/traceroute.png")));
	if (token.contains(":") && !token.contains("=") 
		&& !token.contains("//")
		&& !token.contains("ERROR")) {
	    root->setForeground(0, QBrush(QColor(0, 0, 255, 127)));
	}

	if (token.contains("ERROR")) {
	    root->setForeground(0, QBrush(QColor(255, 0, 0, 127)));
	}
        root->setText(0, token);
        root->setToolTip(0, token);
    }

    // Show full scan log
    foreach (const QString &token, listLogInfo_) {
        QTreeWidgetItem *root = new QTreeWidgetItem(listScan);
        itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/book.png")));
        root->setText(0, token);
        root->setToolTip(0, token);
    }

    // Show scan error
    foreach (const QString &token, listErrorInfo_) {
        QTreeWidgetItem *root = new QTreeWidgetItem(listScanError);
        itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/messagebox_critical.png")));
        root->setText(0, token);
        root->setToolTip(0, token);
    }

    // call plugins parser
    showParserObjPlugins(indexObj);
}

