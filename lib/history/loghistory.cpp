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

#include "loghistory.h"

logHistory::logHistory(QTreeWidget* treeLog,const QString ConfigTag,const QString ConfigTagTime,int cacheSize)
{
    Q_ASSERT(treeLog->columnCount() == 2 || treeLog->columnCount() == 3);

    logTree = treeLog;
    configTag = ConfigTag;
    configTagTime = ConfigTagTime;
    m_cacheSize = cacheSize;
}

// cache host contructor
logHistory::logHistory(const QString ConfigTag, const QString ConfigTagTime, int cacheSize)
    : logTree(0)
{
    configTag = ConfigTag;
    configTagTime = ConfigTagTime;
    m_cacheSize = cacheSize;
}

// cache host contructor
logHistory::logHistory(const QString ConfigTag, int cacheSize)
    : logTree(0)
{
    configTag = ConfigTag;
    m_cacheSize = cacheSize;
}

logHistory::~logHistory()
{
}

const QStringList logHistory::historyReadUrl()
{
    QSettings settings("nmapsi4", "nmapsi4_bookmark");

    return settings.value(configTag, "NULL").toStringList();
}

const QList<QString> logHistory::historyReadUrlTime()
{
    QSettings settings("nmapsi4", "nmapsi4_bookmark");

    return settings.value(configTagTime, "NULL").toStringList();
}

void logHistory::addItemHistory(const QString name)
{
    coreItemHistory(name, NULL);
}

void logHistory::addItemHistory(const QString name, const QString value)
{
    coreItemHistory(name, value);
}

bool logHistory::isProfileInHistory(const QString profileName)
{
    return historyReadUrlTime().contains(profileName);
}

void logHistory::updateProfile(const QString parameters, const QString profileName)
{
    QSettings settings("nmapsi4", "nmapsi4_bookmark");

    QList<QString> urlList = historyReadUrl();
    QList<QString> urlListTime = historyReadUrlTime();

    if (urlListTime.contains(profileName))
    {
        int index = urlListTime.indexOf(profileName);
        urlList.removeAt(index);
        urlListTime.removeAt(index);

        if (urlList.size())
        {
            settings.setValue(configTag, QVariant(urlList));
            settings.setValue(configTagTime, QVariant(urlListTime));
        }
        else
        {
            settings.setValue(configTag, "NULL");
            settings.setValue(configTagTime, "NULL");
        }
    }

    addItemHistory(parameters,profileName);
}

void logHistory::deleteItemBookmark(const QString item)
{
    QSettings settings("nmapsi4", "nmapsi4_bookmark");
    QList<QString> urlList = historyReadUrl();
    QList<QString> urlListTime = historyReadUrlTime();

    for(int index=0; index < urlList.size(); index++)
    {
        if (urlList[index].contains(item))
        {
            int index = urlList.indexOf(item);
            urlList.removeAt(index);
            urlListTime.removeAt(index);
            if(urlList.size())
            {
                settings.setValue(configTag, QVariant(urlList));
                settings.setValue(configTagTime, QVariant(urlListTime));
            }
            else
            {
                settings.setValue(configTag, "NULL");
                settings.setValue(configTagTime, "NULL");
            }
            break;
        }
    }
}

void logHistory::updateBookMarks()
{
    QList<QString> urlList = historyReadUrl();
    QList<QString> urlListTime = historyReadUrlTime();

    memory::freelist<QTreeWidgetItem*>::itemDeleteAll(ItemListHistory);

    logTree->clear();
    logTree->setIconSize(QSize(22, 22));

    if (!urlList.isEmpty() && !urlList.first().contains("NULL")
        && !urlListTime.first().contains("NULL"))
    {
        short index = 0;
        foreach(const QString& item, urlList)
        {
            historyItem = new QTreeWidgetItem(logTree);
            historyItem->setIcon(0, QIcon(QString::fromUtf8(":/images/images/bookmark.png")));
            ItemListHistory.push_front(historyItem);
            historyItem->setText(0, item);
            historyItem->setText(1, urlListTime[index]);
            index++;
        }
    }
}

QStringList logHistory::getHostCache()
{
    return historyReadUrl();
}
