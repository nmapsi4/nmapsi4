/*
Copyright 2008-2013  Francesco Cecconi <francesco.cecconi@gmail.com>

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

History::History(QTreeWidget* treeWidget, const QString firstConfigTag, const QString secondConfigTag, int cacheSize)
{
    Q_ASSERT(treeWidget->columnCount() == 2 || treeWidget->columnCount() == 3);

    m_treeWidget = treeWidget;
    m_firstConfigTag = firstConfigTag;
    m_secondConfigTag = secondConfigTag;
    m_cacheSize = cacheSize;
}

History::History(const QString firstConfigTag, const QString secondConfigTag, int cacheSize)
    : m_treeWidget(0)
{
    m_firstConfigTag = firstConfigTag;
    m_secondConfigTag = secondConfigTag;
    m_cacheSize = cacheSize;
}

History::History(const QString configTag, int cacheSize)
    : m_treeWidget(0)
{
    m_firstConfigTag = configTag;
    m_cacheSize = cacheSize;
}

History::~History()
{
}

const QStringList History::readBaseHistoryList()
{
    QSettings settings("nmapsi4", "nmapsi4_bookmark");

    QVariant value = settings.value(m_firstConfigTag, QStringList());

    if (value.isValid()) {
        QStringList list = value.toStringList();
        // NOTE: backward compatibility (to remove with 0.6.x)
        if (list.size() == 1 && list[0].contains("NULL")) {
            list.clear();
        }
        return list;
    }

    return QStringList();
}

const QStringList History::readOptionalHistoryList()
{
    QSettings settings("nmapsi4", "nmapsi4_bookmark");

    QVariant value = settings.value(m_secondConfigTag, QStringList());

    if (value.isValid()) {
        QStringList list = value.toStringList();
        // NOTE: backward compatibility (to remove with 0.6.x)
        if (list.size() == 1 && list[0].contains("NULL")) {
            list.clear();
        }
        return list;
    }

    return QStringList();
}

void History::addItemHistory(const QString item)
{
    addItemToHistory(item, QString());
}

void History::addItemHistory(const QString item, const QString value)
{
    addItemToHistory(item, value);
}

bool History::isProfileInHistory(const QString profileName)
{
    return readOptionalHistoryList().contains(profileName);
}

void History::updateProfile(const QString parameters, const QString profileName)
{
    QSettings settings("nmapsi4", "nmapsi4_bookmark");

    QStringList baseList = readBaseHistoryList();
    QStringList optionalList = readOptionalHistoryList();

    if (optionalList.contains(profileName)) {
        int index = optionalList.indexOf(profileName);
        baseList.removeAt(index);
        optionalList.removeAt(index);
        settings.setValue(m_firstConfigTag, baseList);
        settings.setValue(m_secondConfigTag, optionalList);
    }

    addItemHistory(parameters, profileName);
}

void History::deleteItemBookmark(const QString item)
{
    QSettings settings("nmapsi4", "nmapsi4_bookmark");
    QStringList baseList = readBaseHistoryList();
    QStringList optionalList = readOptionalHistoryList();

    for (int index = 0; index < baseList.size(); index++) {
        if (baseList[index].contains(item)) {
            int index = baseList.indexOf(item);
            baseList.removeAt(index);
            optionalList.removeAt(index);
            settings.setValue(m_firstConfigTag, baseList);
            settings.setValue(m_secondConfigTag, optionalList);
            break;
        }
    }
}

QList<QTreeWidgetItem*> History::updateBookMarks()
{
    QStringList baseList = readBaseHistoryList();
    QStringList optionalList = readOptionalHistoryList();

    QList<QTreeWidgetItem*> ItemListHistory;

    m_treeWidget->clear();
    m_treeWidget->setIconSize(QSize(22, 22));

    if (!baseList.isEmpty() && !optionalList.isEmpty()) {
        short index = 0;
        for (const QString & item : baseList) {
            QTreeWidgetItem* historyItem = new QTreeWidgetItem(m_treeWidget);
            historyItem->setIcon(0, QIcon(QString::fromUtf8(":/images/images/bookmark.png")));
            ItemListHistory.push_front(historyItem);
            historyItem->setText(0, item);
            historyItem->setToolTip(0, item);
            historyItem->setText(1, optionalList[index]);
            historyItem->setToolTip(1, optionalList[index]);
            index++;
        }
    }

    return ItemListHistory;
}

QStringList History::getHostCache()
{
    return readBaseHistoryList();
}

void History::addItemToHistory(const QString item, const QString timeStamp)
{
    Q_ASSERT(!item.isEmpty());

    QSettings settings("nmapsi4", "nmapsi4_bookmark");

    QStringList baseList = readBaseHistoryList();
    QStringList optionalList;

    if (!timeStamp.isEmpty()) {
        optionalList = readOptionalHistoryList();
    }

    if (baseList.isEmpty()) {
        baseList.append(item);
        settings.setValue(m_firstConfigTag, baseList);
        if (!timeStamp.isEmpty()) {
            optionalList.append(timeStamp);
            settings.setValue(m_secondConfigTag, optionalList);
        }
    } else if ((baseList.size() == m_cacheSize) && (m_cacheSize != -1) && (!baseList.contains(item))) {
        // new value with m_cacheSize limit
        baseList.removeLast();
        baseList.push_front(item);
        settings.setValue(m_firstConfigTag, baseList);

        if (!timeStamp.isEmpty()) {
            optionalList.removeLast();
            optionalList.push_front(timeStamp);
            settings.setValue(m_secondConfigTag, optionalList);
        }
    } else if (!baseList.contains(item)) {
        // new value with not m_cacheSize limit
        baseList.push_front(item);
        settings.setValue(m_firstConfigTag, baseList);

        if (!timeStamp.isEmpty()) {
            optionalList.push_front(timeStamp);
            settings.setValue(m_secondConfigTag, optionalList);
        }
    } else if (!timeStamp.isEmpty()) {
        // append time
        int index = baseList.indexOf(item);
        optionalList[index].clear();
        optionalList[index].append(timeStamp);
        settings.setValue(m_secondConfigTag, optionalList);
    }
}
