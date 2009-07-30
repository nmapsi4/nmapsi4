/***************************************************************************
 *   Copyright (C) 2008-2009 by Francesco Cecconi                          *
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

#include <QList>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QSettings>
#include <QFile>
#include <QDebug>
#include <QApplication>
#include <QDateTime>
#include <QFileInfo>
#include <QComboBox>
#include <QString>

//#define HISTORY_NO_DEBUG

class logHistory
{

private:
    const QList<QString> historyReadUrl();
    const QList<QString> historyReadUrlTime();
    QTreeWidgetItem* historyItem;
    QTreeWidgetItem* history;
    QList<QTreeWidgetItem*> ItemListHistory;
    QTreeWidget* logTree;
    void coreItemHistory(const QString url, const QString scanTime);

public:
    logHistory(QTreeWidget* treeLog,
               const QString ConfigTag,
               const QString ConfigTagTime,
               int cacheSize);
    logHistory(const QString ConfigTag, int cacheSize);
    ~logHistory() {};
    void updateLogHistory();
    void updateBookMarks();
    virtual void addItemHistory(const QString url);
    virtual void addItemHistory(const QString url, const QString scanTime);
    void deleteItemBookmark(const QString item);

protected:
    QString configTag;
    QString configTagTime;
    int __CACHE_SIZE__;

public slots:
    void searchHistory(const QString tokenWord,  QComboBox* lineHistory);
};

#endif
