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


#include "memorytools.h"

memory::memoryTools::memoryTools()
{

}

memory::memoryTools::~memoryTools()
{

}

void memory::memoryTools::itemDeleteAll(QList<QTreeWidgetItem*>& items)
{
    qDeleteAll(items);
    items.clear();
}

void memory::memoryTools::itemDeleteAll(QList<QListWidgetItem*>& items)
{
    qDeleteAll(items);
    items.clear();
}

void memory::memoryTools::itemDeleteAll(QList<parserObj*>& items)
{
    qDeleteAll(items);
    items.clear();
}

void memory::memoryTools::itemDeleteAll(QList<parserObjUtil*>& items)
{
    qDeleteAll(items);
    items.clear();
}

void memory::memoryTools::itemDeleteAll(QList<QWebView*>& items)
{
    qDeleteAll(items);
    items.clear();
}

void memory::memoryTools::itemDeleteAll(QList<lookUpT*>& items)
{
    qDeleteAll(items);
    items.clear();
}

void memory::memoryTools::itemDeleteAll(QList<digSupport*>& items)
{
    qDeleteAll(items);
    items.clear();
}

void memory::memoryTools::itemDeleteAll(QList<mainDiscover*>& items)
{
    qDeleteAll(items);
    items.clear();
}

void memory::memoryTools::itemDeleteAll(QHash<QString, scanThread*>& items)
{
    /*
     * Clear QHash for scan thread
     */
    foreach (scanThread *ptrTmp, items) {
	delete ptrTmp;
    }
    
    items.clear();
}

void memory::memoryTools::itemDeleteAllWithWait(QHash< QString, scanThread* >& items)
{
    // scan thread quit
    foreach (scanThread *ptrTmp, items) 
    {
        ptrTmp->quit();
        ptrTmp->wait();
    }
    
    memory::memoryTools::itemDeleteAll(items);
}

void memory::memoryTools::itemDeleteAllWithWait(QList<lookUpT*>& items)
{
    foreach (lookUpT *pointer, items) 
    {
        pointer->quit();
        pointer->wait();
    }
    
    memory::memoryTools::itemDeleteAll(items);
}
