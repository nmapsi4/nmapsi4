/***************************************************************************
 *   Copyright (C) 2009-2011 by Francesco Cecconi                          *
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


void nmapClass::checkViewOS(const QString OSline, QTreeWidgetItem *itemOS) {

    if(OSline.contains("Linux")) {
        itemOS->setIcon(0, QIcon(QString::fromUtf8(":/images/images/os-logo/linux_logo.png")));
        //itemOS->setText(0, "Linux OS");
    } else if(OSline.contains("Windows")) {
        itemOS->setIcon(0, QIcon(QString::fromUtf8(":/images/images/os-logo/windows_logo.png")));
        //itemOS->setText(0, "MS Windows OS");
    } else if(OSline.contains("FreeBSD")) {
        itemOS->setIcon(0, QIcon(QString::fromUtf8(":/images/images/os-logo/freebsd_logo.png")));
        //itemOS->setText(0, "FreeBSD OS");
    } else if(OSline.contains("OpenBSD")) {
        itemOS->setIcon(0, QIcon(QString::fromUtf8(":/images/images/os-logo/openbsd_logo.png")));
        //itemOS->setText(0, "OpenBSD OS");
    } else if(OSline.contains("Solaris")) {
        itemOS->setIcon(0, QIcon(QString::fromUtf8(":/images/images/os-logo/solaris_logo.png")));
        //itemOS->setText(0, "Sun Solaris OS");
    } else if(OSline.contains("Mac OS X")) {
        itemOS->setIcon(0, QIcon(QString::fromUtf8(":/images/images/os-logo/mac-os-x_logo.png")));
        //itemOS->setText(0, "Mac OS X");
    } else if(itemOS->text(0).isEmpty()) { // double check
        itemOS->setIcon(0, QIcon(QString::fromUtf8(":/images/images/no-os.png")));
    }
}

void nmapClass::runtimePars(QTreeWidgetItem *item, int column) { // SLOT
    Q_UNUSED(column);

#ifndef PARSER_NO_DEBUG
    qDebug() << "DEBUG::Runtime::Parent:: " << item->parent();
#endif
    QString hostName_ = item->text(0);
    hostName_  = hostName_.left(hostName_.indexOf("\n"));

    if(hostEdit->itemText(0).isEmpty() && item->parent() == NULL) {
        hostEdit->addItem(hostName_);
    } else
    if(item->parent() == NULL) {
        hostEdit->setItemText(0, hostName_);
    }

    int indexObj = treeMain->indexOfTopLevelItem(item);
#ifndef PARSER_NO_DEBUG
    qDebug() << "DEBUG::ItemIndex:: " << indexObj;
#endif

    if(indexObj != -1) {
        showParserObj(indexObj);
    }
}

void nmapClass::runtimeTraceroutePars(QTreeWidgetItem *item, int column) { // SLOT
    Q_UNUSED(column);

#ifndef PARSER_NO_DEBUG
    qDebug() << "DEBUG::Runtime::Parent:: " << item->parent();
#endif

    if(hostEdit->itemText(0).isEmpty() && item->parent() != NULL && !item->text(2).isEmpty()) {
        if(!item->text(3).contains("DNS")) {
            hostEdit->addItem(item->text(3));
        } else {
            hostEdit->addItem(item->text(2));
        }
    } else
    if(item->parent() != NULL && !item->text(2).isEmpty()) {
        if(!item->text(3).contains("DNS")) {
            hostEdit->setItemText(0, item->text(3));
        } else {
            hostEdit->setItemText(0, item->text(2));
        }
    }
}

void nmapClass::objVulnButton() { // SLOT

    comboVulnRis->clear();
    
    QString tmpServices_ = GItree->currentItem()->text(0);
    QList<QTreeWidgetItem*> itemList = listWscan->findItems(tmpServices_, Qt::MatchExactly, 2);

    if(comboVulnRis->itemText(0).isEmpty()) {
        comboVulnRis->addItem(itemList[0]->text(3));
    } else {
        comboVulnRis->setItemText(0,itemList[0]->text(3));
    }

    stackedMain->setCurrentIndex(2);
    toolBar->setVisible(false);
    toolBarBook->setVisible(true);
    actionMain_Toolbars->setChecked(false);
    toolBar_2->setVisible(false);
    actionActions_Toolbar->setChecked(false);
    toolBarSearch->setVisible(true);

    if(frameScan->isVisible()) {
        frameScan->setVisible(false);
    }
    if(treeMain->isVisible()) {
        treeMain->setVisible(false);
    }

    if(frameAdv->isVisible()) {
        frameAdv->setVisible(false);
    }
    
    if(frameRight->isVisible()) {
        frameRight->setVisible(false);
    }
    
    if(frame_2->isVisible()) {
        frame_2->setVisible(false);
    }
}
