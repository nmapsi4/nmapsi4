/*
Copyright 2011-2012  Francesco Cecconi <francesco.cecconi@gmail.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MEMORYTOOLS_H
#define MEMORYTOOLS_H

#include <QtCore/QList>
#include <QtCore/QHash>

namespace memory
{

template <class T>
class freelist
{
public:
    static void itemDeleteAll(QList<T>& items);
    /*
     * with thread quit() and wait()
     */
    static void itemDeleteAllWithWait(QList<T>& items);
};

template <class T, class U>
class freemap
{
public:
    static void itemDeleteAll(QHash<T, U>& items);
    /*
     * map itemDeleteAll(QHash<QString, scanThread*>& items)
     * with thread quit() and wait()
     */
    static void itemDeleteAllWithWait(QHash<T, U>& items);
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
    foreach(T pointer, items) {
        pointer->quit();
        pointer->wait();
    }

    memory::freelist<T>::itemDeleteAll(items);
}

template <class T, class U>
inline void memory::freemap<T, U>::itemDeleteAll(QHash<T, U>& items)
{
    /*
     * Clear QHash
     */
    foreach(U ptrTmp, items) {
        delete ptrTmp;
    }

    items.clear();
}

template <class T, class U>
inline void memory::freemap<T, U>::itemDeleteAllWithWait(QHash<T, U>& items)
{
    // scan thread quit
    foreach(U ptrTmp, items) {
        ptrTmp->quit();
        ptrTmp->wait();
    }

    memory::freemap<T, U>::itemDeleteAll(items);
}

} // end namespace
#endif // MEMORYTOOLS_H
