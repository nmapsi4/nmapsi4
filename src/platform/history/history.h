/***************************************************************************
 *   Copyright (C) 2008-2011 by Francesco Cecconi                          *
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

#ifndef HISTORY_H
#define HISTORY_H

#include <QtCore/QList>
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>
#include <QtCore/QSettings>
#include <QtCore/QDateTime>
#include <QtCore/QString>

#include "memorytools.h"

#define HISTORY_NO_DEBUG

class History
{
public:
    History(QTreeWidget* treeLog, const QString ConfigTag, const QString ConfigTagTime, int cacheSize);
    History(const QString ConfigTag, const QString ConfigTagTime, int cacheSize);
    explicit History(const QString ConfigTag, int cacheSize = 0);
    ~History();
    QList<QTreeWidgetItem*> updateBookMarks();
    void addItemHistory(const QString name);
    void addItemHistory(const QString name, const QString value);
    void deleteItemBookmark(const QString item);
    QStringList getHostCache();
    bool isProfileInHistory(const QString profileName);
    void updateProfile(const QString parameters, const QString profileName);

private:
    const QStringList historyReadUrl();
    const QList<QString> historyReadUrlTime();
    void addItemToHistory(const QString url, const QString scanTime);

    QTreeWidget* logTree;
    QTreeWidgetItem* historyItem;
    QString configTag;
    QString configTagTime;
    int m_cacheSize;

};

#endif
