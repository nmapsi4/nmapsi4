/*
Copyright 2008-2012  Francesco Cecconi <francesco.cecconi@gmail.com>

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

#include "history.h"

History::History(QTreeWidget* treeLog, const QString ConfigTag, const QString ConfigTagTime, int cacheSize)
{
    Q_ASSERT(treeLog->columnCount() == 2 || treeLog->columnCount() == 3);

    logTree = treeLog;
    configTag = ConfigTag;
    configTagTime = ConfigTagTime;
    m_cacheSize = cacheSize;
}

// cache host contructor
History::History(const QString ConfigTag, const QString ConfigTagTime, int cacheSize)
    : logTree(0)
{
    configTag = ConfigTag;
    configTagTime = ConfigTagTime;
    m_cacheSize = cacheSize;
}

// cache host contructor
History::History(const QString ConfigTag, int cacheSize)
    : logTree(0)
{
    configTag = ConfigTag;
    m_cacheSize = cacheSize;
}

History::~History()
{
}

const QStringList History::historyReadUrl()
{
    QSettings settings("nmapsi4", "nmapsi4_bookmark");

    return settings.value(configTag, "NULL").toStringList();
}

const QList<QString> History::historyReadUrlTime()
{
    QSettings settings("nmapsi4", "nmapsi4_bookmark");

    return settings.value(configTagTime, "NULL").toStringList();
}

void History::addItemHistory(const QString name)
{
    addItemToHistory(name, NULL);
}

void History::addItemHistory(const QString name, const QString value)
{
    addItemToHistory(name, value);
}

bool History::isProfileInHistory(const QString profileName)
{
    return historyReadUrlTime().contains(profileName);
}

void History::updateProfile(const QString parameters, const QString profileName)
{
    QSettings settings("nmapsi4", "nmapsi4_bookmark");

    QList<QString> urlList = historyReadUrl();
    QList<QString> urlListTime = historyReadUrlTime();

    if (urlListTime.contains(profileName)) {
        int index = urlListTime.indexOf(profileName);
        urlList.removeAt(index);
        urlListTime.removeAt(index);

        if (urlList.size()) {
            settings.setValue(configTag, QVariant(urlList));
            settings.setValue(configTagTime, QVariant(urlListTime));
        } else {
            settings.setValue(configTag, "NULL");
            settings.setValue(configTagTime, "NULL");
        }
    }

    addItemHistory(parameters, profileName);
}

void History::deleteItemBookmark(const QString item)
{
    QSettings settings("nmapsi4", "nmapsi4_bookmark");
    QList<QString> urlList = historyReadUrl();
    QList<QString> urlListTime = historyReadUrlTime();

    for (int index = 0; index < urlList.size(); index++) {
        if (urlList[index].contains(item)) {
            int index = urlList.indexOf(item);
            urlList.removeAt(index);
            urlListTime.removeAt(index);
            if (urlList.size()) {
                settings.setValue(configTag, QVariant(urlList));
                settings.setValue(configTagTime, QVariant(urlListTime));
            } else {
                settings.setValue(configTag, "NULL");
                settings.setValue(configTagTime, "NULL");
            }
            break;
        }
    }
}

QList<QTreeWidgetItem*> History::updateBookMarks()
{
    QList<QString> urlList = historyReadUrl();
    QList<QString> urlListTime = historyReadUrlTime();

    QList<QTreeWidgetItem*> ItemListHistory;

    logTree->clear();
    logTree->setIconSize(QSize(22, 22));

    if (!urlList.isEmpty() && !urlList.first().contains("NULL")
            && !urlListTime.first().contains("NULL")) {
        short index = 0;
        foreach(const QString & item, urlList) {
            QTreeWidgetItem* historyItem = new QTreeWidgetItem(logTree);
            historyItem->setIcon(0, QIcon(QString::fromUtf8(":/images/images/bookmark.png")));
            ItemListHistory.push_front(historyItem);
            historyItem->setText(0, item);
            historyItem->setToolTip(0, item);
            historyItem->setText(1, urlListTime[index]);
            historyItem->setToolTip(1, urlListTime[index]);
            index++;
        }
    }

    return ItemListHistory;
}

QStringList History::getHostCache()
{
    return historyReadUrl();
}

void History::addItemToHistory(const QString url, const QString scanTime)
{
    Q_ASSERT(!url.isEmpty());

    QSettings settings("nmapsi4", "nmapsi4_bookmark");

    QList<QString> urlList = historyReadUrl();
    QList<QString> urlListTime;

    if (!scanTime.isNull()) {
        urlListTime = historyReadUrlTime();
    }

    if (urlList.contains("NULL")) {
        urlList.removeFirst();
        urlList.append(url);
        settings.setValue(configTag, QVariant(urlList));
        if (!scanTime.isNull()) {
            urlListTime.removeFirst();
            urlListTime.append(scanTime);
            settings.setValue(configTagTime, QVariant(urlListTime));
        }
    } else if ((urlList.size() == m_cacheSize) && (m_cacheSize != -1) && (!urlList.contains(url))) {
        // new value with m_cacheSize limit
        urlList.removeLast();
        urlList.push_front(url);
        settings.setValue(configTag, QVariant(urlList));

        if (!scanTime.isNull()) {
            urlListTime.removeLast();
            urlListTime.push_front(scanTime);
            settings.setValue(configTagTime, QVariant(urlListTime));
        }
    } else if (!urlList.contains(url)) {
        // new value with not m_cacheSize limit
        urlList.push_front(url);
        settings.setValue(configTag, QVariant(urlList));

        if (!scanTime.isNull()) {
            urlListTime.push_front(scanTime);
            settings.setValue(configTagTime, QVariant(urlListTime));
        }
    } else if (!scanTime.isNull()) {
        int index = urlList.indexOf(url);
        urlListTime[index].clear();
        urlListTime[index].append(QDateTime::currentDateTime().toString("MMMM d yyyy - hh:mm:ss"));
        settings.setValue(configTagTime, QVariant(urlListTime));
    }
}
