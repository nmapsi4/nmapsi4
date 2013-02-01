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

#ifndef HISTORY_H
#define HISTORY_H

#include <QtCore/QList>
#include <QtCore/QSettings>
#include <QtCore/QDateTime>
#include <QtCore/QString>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include "memorytools.h"

#define HISTORY_NO_DEBUG

class History
{
public:
    History(QTreeWidget* treeWidget, const QString firstConfigTag, const QString secondConfigTag, int cacheSize);
    History(const QString firstConfigTag, const QString secondConfigTag, int cacheSize);
    explicit History(const QString configTag, int cacheSize = 0);
    ~History();
    QList<QTreeWidgetItem*> updateBookMarks();
    QStringList getHostCache();
    void addItemHistory(const QString item);
    void addItemHistory(const QString item, const QString value);
    void deleteItemBookmark(const QString item);
    void updateProfile(const QString parameters, const QString profileName);
    bool isProfileInHistory(const QString profileName);

private:
    const QStringList readBaseHistoryList();
    const QStringList readOptionalHistoryList();
    void addItemToHistory(const QString item, const QString timeStamp);

    QTreeWidget* m_treeWidget;
    QString m_firstConfigTag;
    QString m_secondConfigTag;
    int m_cacheSize;

};

#endif
