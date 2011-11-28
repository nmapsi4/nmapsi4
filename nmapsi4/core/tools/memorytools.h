/*
    <one line to give the program's name and a brief idea of what it does.>
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


#ifndef MEMORYTOOLS_H
#define MEMORYTOOLS_H

#include <QtCore/QList>
#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QListWidgetItem>
#include <QtWebKit/QWebView>

// local include
#include "maindiscover.h"
#include "parserObj.h"
#include "parserObjUtil.h"
#include "lookUpT.h"
#include "digSupport.h"
#include "scanThread.h"

using namespace pingInterface;
using namespace internalLookup;
using namespace digInterface;
using namespace scanning;

namespace memory
{

template <class T>
class freelist
{
public:
    static void itemDeleteAll(QList<T>& items);
    /*
     * list itemDeleteAll(QHash<QString, scanThread*>& items)
     * with thread quit() and wait()
     */
    static void itemDeleteAllWithWait(QList<T>& items);
};

template <class T, class U>
class freemap
{
public:
    static void itemDeleteAll(QHash<T,U>& items);
    /*
     * map itemDeleteAll(QHash<QString, scanThread*>& items)
     * with thread quit() and wait()
     */
    static void itemDeleteAllWithWait(QHash<T,U>& items);
};

template <class T>
inline void memory::freelist<T>::itemDeleteAll(QList<T>& items)
{
    qDeleteAll(items);
    items.clear();
}

template <class T>
inline void memory::freelist<T>::itemDeleteAllWithWait(QList<T>& items)
{
    foreach (lookUpT *pointer, items)
    {
        pointer->quit();
        pointer->wait();
    }

    memory::freelist<T>::itemDeleteAll(items);
}

template <class T, class U>
inline void memory::freemap<T,U>::itemDeleteAll(QHash<T,U>& items)
{
    /*
     * Clear QHash
     */
    foreach (U ptrTmp, items) {
        delete ptrTmp;
    }

    items.clear();
}

template <class T, class U>
inline void memory::freemap<T,U>::itemDeleteAllWithWait(QHash<T,U>& items)
{
    // scan thread quit
    foreach (U ptrTmp, items)
    {
        ptrTmp->quit();
        ptrTmp->wait();
    }

    memory::freemap<T,U>::itemDeleteAll(items);
}

} // end namespace
#endif // MEMORYTOOLS_H
