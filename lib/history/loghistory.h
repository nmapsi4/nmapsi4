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

#ifndef LOGHISTORY_H
#define LOGHISTORY_H

#include <QtCore/QList>
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>
#include <QtCore/QSettings>
#include <QtCore/QFile>
#include <QtGui/QApplication>
#include <QtCore/QDateTime>
#include <QtCore/QFileInfo>
#include <QtGui/QComboBox>
#include <QtCore/QString>

#include "memorytools.h"

#define HISTORY_NO_DEBUG

class logHistory
{
    /**
     * TODO:: const to const &
     **/

public:
    logHistory(QTreeWidget* treeLog,const QString ConfigTag,const QString ConfigTagTime,int cacheSize);
    logHistory(const QString ConfigTag, const QString ConfigTagTime, int cacheSize);
    logHistory(const QString ConfigTag, int cacheSize);
    ~logHistory();
    void updateLogHistory();
    void updateBookMarks();
    void addItemHistory(const QString name);
    void addItemHistory(const QString name, const QString value);
    void deleteItemBookmark(const QString item);
    QStringList getHostCache();
    bool isProfileInHistory(const QString profileName);
    void updateProfile(const QString parameters, const QString profileName);

private:
    const QStringList historyReadUrl();
    const QList<QString> historyReadUrlTime();
    void coreItemHistory(const QString url, const QString scanTime);

    QTreeWidget* logTree;
    QTreeWidgetItem* historyItem;
    QTreeWidgetItem* history;
    QList<QTreeWidgetItem*> ItemListHistory;
    QString configTag;
    QString configTagTime;
    int m_cacheSize;

};

#endif
