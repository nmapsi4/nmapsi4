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


bool nmapClass::checkViewOS(const QString OSline, QTreeWidgetItem *itemOS) const 
{
    // check string OS for Icon selection
    itemOS->setTextAlignment(1, Qt::AlignVCenter | Qt::AlignRight);
    
    if(OSline.contains("Linux")) 
    {
        itemOS->setIcon(0, QIcon(QString::fromUtf8(":/images/images/os-logo/linux_logo.png")));
	itemOS->setText(1, "GNU/Linux");
	return true;
    } 
    else if(OSline.contains("Windows")) 
    {
        itemOS->setIcon(0, QIcon(QString::fromUtf8(":/images/images/os-logo/windows_logo.png")));
	itemOS->setText(1, "MS Windows");
	return true;
    } 
    else if(OSline.contains("FreeBSD")) 
    {
        itemOS->setIcon(0, QIcon(QString::fromUtf8(":/images/images/os-logo/freebsd_logo.png")));
	itemOS->setText(1, "FreeBSD");
	return true;
    } 
    else if(OSline.contains("OpenBSD")) 
    {
        itemOS->setIcon(0, QIcon(QString::fromUtf8(":/images/images/os-logo/openbsd_logo.png")));
	itemOS->setText(1, "OpenBSD");
	return true;
    } 
    else if(OSline.contains("Solaris")) 
    {
        itemOS->setIcon(0, QIcon(QString::fromUtf8(":/images/images/os-logo/solaris_logo.png")));
	itemOS->setText(1, "Solaris");
	return true;
    } 
    else if(OSline.contains("Mac OS X")) 
    {
        itemOS->setIcon(0, QIcon(QString::fromUtf8(":/images/images/os-logo/mac-os-x_logo.png")));
	itemOS->setText(1, "MacOsX");
	return true;
    } 
    else 
    {
        itemOS->setIcon(0, QIcon(QString::fromUtf8(":/images/images/no-os.png")));
	itemOS->setText(1, "UnDiscovered");
	return false;
    }
}

void nmapClass::runtimePars(QTreeWidgetItem *item, int column) 
{ // SLOT
    Q_UNUSED(column);

    QString hostName_ = item->text(0);
    hostName_  = hostName_.left(hostName_.indexOf("\n"));

    if(hostEdit->itemText(0).isEmpty() && item->parent() == NULL) 
    {
        hostEdit->addItem(hostName_);
    } 
    else if(item->parent() == NULL) 
    {
        hostEdit->setItemText(0, hostName_);
    }

    int indexObj = treeMain->indexOfTopLevelItem(item);

    if(indexObj != -1) 
    {
        showParserObj(indexObj);
    }
}

void nmapClass::runtimeTraceroutePars(QTreeWidgetItem *item, int column) 
{ // SLOT
    Q_UNUSED(column);

    if(hostEdit->itemText(0).isEmpty() && !item->parent() && !item->text(2).isEmpty()) 
    {
        if(!item->text(3).contains("DNS")) 
        {
            hostEdit->addItem(item->text(3));
        } 
        else 
        {
            hostEdit->addItem(item->text(2));
        }
    } 
    else if(!item->parent() && !item->text(2).isEmpty()) 
    {
        if(!item->text(3).contains("DNS")) 
        {
            hostEdit->setItemText(0, item->text(3));
        } 
        else 
        {
            hostEdit->setItemText(0, item->text(2));
        }
    }
}

