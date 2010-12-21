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

logHistory::logHistory(QTreeWidget* treeLog,
                       const QString ConfigTag,
                       const QString ConfigTagTime,
                       int cacheSize)
{
    Q_ASSERT(treeLog->columnCount() == 2 || treeLog->columnCount() == 3);
    logTree = treeLog;
    configTag = ConfigTag;
    configTagTime = ConfigTagTime;
    __CACHE_SIZE__ = cacheSize;
}

// cache host contructor
logHistory::logHistory(const QString ConfigTag, int cacheSize) :  logTree(NULL)
{
    configTag = ConfigTag;
    __CACHE_SIZE__ = cacheSize;
}

const QList<QString> logHistory::historyReadUrl()
{
    QSettings settings("nmapsi4", "nmapsi4_bookmark");
    QList<QString> urlList;
    urlList = settings.value(configTag, "NULL").toStringList();
    return urlList;
}

const QList<QString> logHistory::historyReadUrlTime()
{

    QSettings settings("nmapsi4", "nmapsi4_bookmark");
    QList<QString> urlListTime;
    urlListTime = settings.value(configTagTime, "NULL").toStringList();
    return urlListTime;
}

void logHistory::addItemHistory(const QString url)
{
    coreItemHistory(url, NULL);
}

void logHistory::addItemHistory(const QString url, const QString scanTime)
{
    coreItemHistory(url, scanTime);
}

void logHistory::deleteItemBookmark(const QString item)
{
    Q_ASSERT(item != NULL);
#ifndef HISTORY_NO_DEBUG
    qDebug() << "logHistory::deleteItemBookmark() -- call";
#endif
    QSettings settings("nmapsi4", "nmapsi4_bookmark");
    QList<QString> urlList = historyReadUrl();
    QList<QString> urlListTime = historyReadUrlTime();
    
    for(int index=0; index < urlList.size(); index++) {
        if (urlList[index].contains(item)) {
            int index = urlList.indexOf(item);
            urlList.removeAt(index);
            urlListTime.removeAt(index);
            if(urlList.size()) {
                settings.setValue(configTag, QVariant(urlList));
                settings.setValue(configTagTime, QVariant(urlListTime));
            } else {
                settings.setValue(configTag, "NULL");
                settings.setValue(configTagTime, "NULL");
            }
            break;
        }
    }
    updateBookMarks();
}

void logHistory::updateLogHistory()
{
#ifndef HISTORY_NO_DEBUG
    qDebug() << "logHistory::updateTreeHistory() -- call";
#endif
    QSettings settings("nmapsi4", "nmapsi4_bookmark");
    QList<QString> urlList = historyReadUrl();
    QList<QString> urlListTime = historyReadUrlTime();
    qDeleteAll(ItemListHistory);
    ItemListHistory.clear();
    logTree->clear();
    logTree->setIconSize(QSize(32, 32));

    QFile *tmpFile = new QFile();
    short index = 0;
    if (!urlList.isEmpty() && urlList.first().compare("NULL")) {
        foreach(QString item, urlList) {
            tmpFile->setFileName(item);
            if (tmpFile->exists() && urlListTime.first().compare("NULL")) {
                historyItem = new QTreeWidgetItem(logTree);
                historyItem->setIcon(0, QIcon(QString::fromUtf8(":/images/images/book.png")));
                ItemListHistory.push_front(historyItem);
                QFileInfo fInfo(item);
                historyItem->setText(0, fInfo.fileName());
                historyItem->setText(1, fInfo.filePath().remove(fInfo.fileName()));
                historyItem->setText(2, urlListTime[index]);
                index++;
            } else {
                if (urlList.contains(item) && urlListTime.first().compare("NULL")) {
#ifndef HISTORY_NO_DEBUG
                    qDebug() << "Remove Items...";
#endif
                    int index = urlList.indexOf(item);
                    urlList.removeOne(item);
                    urlListTime.removeAt(index);
                    settings.setValue(configTag, QVariant(urlList));
                    settings.setValue(configTagTime, QVariant(urlListTime));
                } else {
                    history = new QTreeWidgetItem(logTree);
                    history->setIcon(0, QIcon(QString::fromUtf8(":/images/images/book.png")));
                    ItemListHistory.push_front(historyItem);
                    history->setText(0, QApplication::translate("logHistory",
                                     "Your configuration file is too old, please delete it",
                                     0, QApplication::UnicodeUTF8));

                }
            }
        }
    } else {
        history = new QTreeWidgetItem(logTree);
        history->setIcon(0, QIcon(QString::fromUtf8(":/images/images/book.png")));
        ItemListHistory.push_front(historyItem);
        history->setText(0, "No Log Cache");
    }
    delete tmpFile;

}


void logHistory::updateBookMarks()
{

#ifndef HISTORY_NO_DEBUG
    qDebug() << "logHistory::updateTH() -- call";
#endif
    QSettings settings("nmapsi4", "nmapsi4_bookmark");
    QList<QString> urlList = historyReadUrl();
    QList<QString> urlListTime = historyReadUrlTime();
    short index = 0;
    qDeleteAll(ItemListHistory);
    ItemListHistory.clear();
    logTree->clear();
    logTree->setIconSize(QSize(22, 22));

    if (!urlList.isEmpty() && urlList.first().compare("NULL") && urlListTime.first().compare("NULL")) {
#ifndef HISTORY_NO_DEBUG
	qDebug() << "logHistory::updateTH() -- scope 1";
#endif
        foreach(QString item, urlList) {
            historyItem = new QTreeWidgetItem(logTree);
            historyItem->setIcon(0, QIcon(QString::fromUtf8(":/images/images/bookmark.png")));
            ItemListHistory.push_front(historyItem);
            historyItem->setText(0, item);
            historyItem->setText(1, urlListTime[index]);
            index++;
        }
    } else if (!urlListTime.first().compare("NULL") && urlList.first().compare("NULL")) {
#ifndef HISTORY_NO_DEBUG
	qDebug() << "logHistory::updateTH() -- scope 2";
#endif
        history = new QTreeWidgetItem(logTree);
        history->setIcon(0, QIcon(QString::fromUtf8(":/images/images/bookmark.png")));
        ItemListHistory.push_front(historyItem);
        history->setText(0, QApplication::translate("logHistory",
                         "Your configuration file is too old, please delete it",
                         0, QApplication::UnicodeUTF8));
    }
}

void logHistory::searchHistory(const QString tokenWord, QComboBox* lineHistory)
{

#ifndef HISTORY_NO_DEBUG
    qDebug() << "Call --> history::searchHistory() ";
#endif

    lineHistory->setItemIcon(0, QIcon(QString::fromUtf8(":/images/images/bookmark_toolbar.png")));
    QList<QString> urlList = historyReadUrl();
    lineHistory->setCompleter(0);
#ifndef HISTORY_NO_DEBUG
    qDebug() << "logHistory::Pre::cout:: " << lineHistory->count();
#endif
    // FIXME:: check bug on MS windows
#ifdef Q_WS_X11
    for(int index=1; index <= lineHistory->count(); index++) {
                    lineHistory->removeItem(index);
    }
#endif

#ifndef HISTORY_NO_DEBUG
    qDebug() << "logHistory::Post::cout:: " << lineHistory->count();
#endif
    
    foreach(QString item, urlList) {
        if (item.startsWith(tokenWord)) {
#ifndef HISTORY_NO_DEBUG
            qDebug() << "History::Item:: " << item << "Token:: " << tokenWord;
#endif
            if ((lineHistory->findText(item, Qt::MatchExactly) == -1)
                && !tokenWord.isEmpty()) {
                lineHistory->insertItem(1, item);
                lineHistory->setItemIcon(1, QIcon(QString::fromUtf8(":/images/images/bookmark.png")));
            } else {
                if (tokenWord.isEmpty())
                    lineHistory->clear();
            }
	}
        lineHistory->setItemText(0, tokenWord);
    }
    //TODO show popup
    //lineHistory->setItemText(-1, tokenWord);
    //lineHistory->setCurrentIndex(0);
    //lineHistory->showPopup();
}
