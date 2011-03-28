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

#include "../mainwin.h"

void nmapClass::itemDeleteAll(QList<QTreeWidgetItem*>& items)
{
#ifndef TOOLS_NO_DEBUG
    qDebug() << "Nmapsi4/itemDeleteAll() -> Free List";
#endif
    qDeleteAll(items);
    items.clear();
}

void nmapClass::itemDeleteAll(QList<parserObj*>& items)
{
#ifndef TOOLS_NO_DEBUG
    qDebug() << "Nmapsi4/itemDeleteAll() -> Free List";
#endif
    qDeleteAll(items);
    items.clear();
}

void nmapClass::itemDeleteAll(QList<parserObjUtil*>& items)
{
#ifndef TOOLS_NO_DEBUG
    qDebug() << "Nmapsi4/itemDeleteAll() -> Free List";
#endif
    qDeleteAll(items);
    items.clear();
}

void nmapClass::itemDeleteAll(QList<QWebView*>& items)
{
#ifndef TOOLS_NO_DEBUG
    qDebug() << "Nmapsi4/itemDeleteAll() -> Free List";
#endif
    qDeleteAll(items);
    items.clear();
}

void nmapClass::itemDeleteAll(QList<scanThread*>& items)
{
#ifndef TOOLS_NO_DEBUG
    qDebug() << "Nmapsi4/itemDeleteAll() -> Free List";
#endif
    qDeleteAll(items);
    items.clear();
}

void nmapClass::itemDeleteAll(QList<lookUpT*>& items)
{
#ifndef TOOLS_NO_DEBUG
    qDebug() << "Nmapsi4/itemDeleteAll() -> Free List";
#endif
    qDeleteAll(items);
    items.clear();
}

void nmapClass::itemDeleteAll(QList<digSupport*>& items)
{
#ifndef TOOLS_NO_DEBUG
    qDebug() << "Nmapsi4/itemDeleteAll() -> Free List";
#endif
    qDeleteAll(items);
    items.clear();
}

void nmapClass::listClear() 
{
    listClearFlag = true;
    itemDeleteAll(itemListScan); //clear items list
    // parserObj list
    itemDeleteAll(parserObjList);
    itemDeleteAll(parserObjUtilList);
    // Host list
    itemDeleteAll(mainTreeElem);
    itemDeleteAll(objElem);
    listScanError->clear();
    treeMain->clear();
    treeLookup->clear();
    treeTraceroot->clear();
    treeHostDet->clear();
    GItree->clear();
    listWscan->clear();
    treeNSS->clear();
    listScan->clear();
    actionClear_History->setEnabled(false);
    action_Save_As->setEnabled(false);
    comboScanLog->clear();
    comboScanLog->addItem(tr("Scan log parameters"));
}

