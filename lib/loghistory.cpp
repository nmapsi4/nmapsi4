/***************************************************************************
 *   Copyright (C) 2008 by Francesco Cecconi                               *
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
                       QString ConfigTag,
                       QString ConfigTagTime,
                       int cacheSize)
{
    Q_ASSERT(treeLog->columnCount() == 2 || treeLog->columnCount() == 3);
    logTree = treeLog;
    configTag = ConfigTag;
    configTagTime = ConfigTagTime;
    __CACHE_SIZE__ = cacheSize;
}

logHistory::logHistory(QString ConfigTag, int cacheSize) :  logTree(NULL),
        configTagTime(NULL)
{
    configTag = ConfigTag;
    __CACHE_SIZE__ = cacheSize;
}

/*logHistory::~logHistory()
{
}*/

QList<QString> logHistory::historyReadUrl()
{
    QSettings settings("nmapsi4", "nmapsi4");
    QList<QString> urlList;
    urlList = settings.value(configTag, "NULL").toStringList();
    return urlList;
}

QList<QString> logHistory::historyReadUrlTime()
{

    QSettings settings("nmapsi4", "nmapsi4");
    QList<QString> urlListTime;
    urlListTime = settings.value(configTagTime, "NULL").toStringList();
    return urlListTime;
}

void logHistory::addItemHistory(QString url)
{
    coreItemHistory(url, NULL);
}

void logHistory::addItemHistory(QString url, QString scanTime)
{
    coreItemHistory(url, scanTime);
}

void logHistory::deleteItemBookmark(QString item) 
{
    // TODO
    Q_ASSERT(item != NULL);
    qDebug() << "logHistory::deleteItemBookmark() -- call";
    QSettings settings("nmapsi4", "nmapsi4");
    QList<QString> urlList = historyReadUrl();
    QList<QString> urlListTime = historyReadUrlTime();
    
    foreach(QString itemToken, urlList) {
	if (itemToken.contains(item)) {
	    int index = urlList.indexOf(item);
	    urlList.removeAt(index);
	    urlListTime.removeAt(index);
	    settings.setValue(configTag, QVariant(urlList));
	    settings.setValue(configTagTime, QVariant(urlListTime));
	    break;
	}
    }
    updateBookMarks();
}

void logHistory::updateThFile()
{

    qDebug() << "logHistory::updateTreeHistory() -- call";
    QSettings settings("nmapsi4", "nmapsi4");
    QList<QString> urlList = historyReadUrl();
    QList<QString> urlListTime = historyReadUrlTime();
    logTree->clear();
    /*if(ItemListHistory.size() != 0)
      itemDeleteAll(ItemListHistory);*/
    logTree->setIconSize(QSize::QSize(32, 32));

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
                    qDebug() << "Remove Items...";
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
                /*      if(urlList.isEmpty())
                      history->setText(0, "No Url Cache");*/
            }
        }
    } else {
        history = new QTreeWidgetItem(logTree);
        history->setIcon(0, QIcon(QString::fromUtf8(":/images/images/book.png")));
        ItemListHistory.push_front(historyItem);
        history->setText(0, "No Url Cache");
    }
    delete tmpFile;

}


void logHistory::updateBookMarks()
{
    qDebug() << "logHistory::updateTH() -- call";
    QSettings settings("nmapsi4", "nmapsi4");
    QList<QString> urlList = historyReadUrl();
    QList<QString> urlListTime = historyReadUrlTime();
    short index = 0;
    logTree->clear();
    /*if(ItemListHistory.size() != 0)
      itemDeleteAll(ItemListHistory);*/
    logTree->setIconSize(QSize::QSize(22, 22));

    if (!urlList.isEmpty() && urlList.first().compare("NULL") && urlListTime.first().compare("NULL")) {
        foreach(QString item, urlList) {
            historyItem = new QTreeWidgetItem(logTree);
            historyItem->setIcon(0, QIcon(QString::fromUtf8(":/images/images/bookmark.png")));
            ItemListHistory.push_front(historyItem);
            historyItem->setText(0, item);
            historyItem->setText(1, urlListTime[index]);
            index++;
        }
    } else if (!urlListTime.first().compare("NULL") && urlList.first().compare("NULL")) {
        history = new QTreeWidgetItem(logTree);
        history->setIcon(0, QIcon(QString::fromUtf8(":/images/images/bookmark.png")));
        ItemListHistory.push_front(historyItem);
        history->setText(0, QApplication::translate("logHistory",
                         "Your configuration file is too old, please delete it",
                         0, QApplication::UnicodeUTF8));
    }
}

void logHistory::searchHistory(QString tokenWord, QComboBox* lineHistory)
{
    //TODO works in progress
    qDebug() << "Call --> history::searchHistory() ";
    lineHistory->setItemIcon(0, QIcon(QString::fromUtf8(":/images/images/bookmark_toolbar.png")));
    QList<QString> urlList = historyReadUrl();
    //lineHistory->setCompleter(0);
    foreach(QString item, urlList) {
        if (item.contains(tokenWord)) {
            qDebug() << "History::Item:: " << item << "Token:: " << tokenWord;
            if ((lineHistory->findText(item, Qt::MatchExactly) == -1) && !tokenWord.isEmpty()) {
                lineHistory->insertItem(1, item);
                lineHistory->setItemIcon(1, QIcon(QString::fromUtf8(":/images/images/bookmark.png")));
            } else if (tokenWord.isEmpty())
                lineHistory->clear();
            lineHistory->setItemText(0, tokenWord);
        }
    }
    //TODO show popup
    //lineHistory->setItemText(-1, tokenWord);
    //lineHistory->setCurrentIndex(0);
    //lineHistory->showPopup();
}
