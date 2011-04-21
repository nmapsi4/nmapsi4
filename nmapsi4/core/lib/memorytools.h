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
#include "../discover/maindiscover.h"
#include "../parserObj/parserObj.h"
#include "../parserObj/parserObjUtil.h"
#include "../lookup/lookUpT.h"
#include "../digSupport/digSupport.h"
#include "../scanMT/scanThread.h"

using namespace pingInterface;
using namespace parserObject;
using namespace parserUtilObject;
using namespace internalLookup;
using namespace digInterface;
using namespace scanning;

namespace memory {
    class memoryTools
    {

    public:
	memoryTools();
	virtual ~memoryTools();
	void itemDeleteAll(QList<QTreeWidgetItem*>& items);
	void itemDeleteAll(QList<QListWidgetItem*>& items);
	void itemDeleteAll(QList<parserObj*>& items);
	void itemDeleteAll(QList<parserObjUtil*>& items);
	void itemDeleteAll(QList<QWebView*>& items);
	void itemDeleteAll(QList<lookUpT*>& items);
	void itemDeleteAll(QList<digSupport*>& items);
	void itemDeleteAll(QList<mainDiscover*>& items);
	void itemDeleteAll(QHash<QString, scanThread*>& items);
	/*
	 * Ovverride itemDeleteAll(QHash<QString, scanThread*>& items)
	 * with thread quit() and wait()
	 */
	void itemDeleteAllWithWait(QHash<QString, scanThread*>& items);
	void itemDeleteAllWithWait(QList<lookUpT*>& items);
    };
}

#endif // MEMORYTOOLS_H
