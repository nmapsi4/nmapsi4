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

#include "mainwin.h"

// TODO
#define __CACHE_SIZE__ 10

QList<QString> mwClass::historyReadUrl() {
     QSettings settings("nmapsi4","nmapsi4");
     QList<QString> urlList;
     urlList = settings.value("logReader/urlList", "NULL").toStringList();
     return urlList;
}

void mwClass::historyUpdateUrl(QString url) {
     assert(!url.isEmpty());

     QSettings settings("nmapsi4","nmapsi4");
     QList<QString> urlList;
     urlList = historyReadUrl();

     if(urlList.contains("NULL")) {
	  urlList.removeFirst();
	  urlList.append(url);
	  settings.setValue("logReader/urlList", QVariant(urlList));
     } else {
	  if(urlList.size() == __CACHE_SIZE__) {
	       urlList.removeLast();
	       urlList.push_front(url);
	       settings.setValue("logReader/urlList", QVariant(urlList));
	  } else {
	       urlList.push_front(url);
	       settings.setValue("logReader/urlList", QVariant(urlList));
	  }
     }
}

void mwClass::updateTreeHistory() {
     
     QList<QString> urlList = historyReadUrl();
     /*if(ItemListHistory.size() != 0)
       itemDeleteAll(ItemListHistory);*/
     logTree->setIconSize(QSize::QSize (32, 32));
     logTree->clear();
     QFile *tmpFile = new QFile();
     
     if(!urlList.contains("NULL")) {
	  foreach(QString item, urlList) {
	       tmpFile->setFileName(item);
	       if(tmpFile->exists()) {
		    historyItem = new QTreeWidgetItem(logTree);
		    historyItem->setIcon(0, QIcon(QString::fromUtf8(":/images/images/book.png")));
		    ItemListHistory.push_front(historyItem);
		    // FIXME insert file exists check
		    historyItem->setText(0,item);
	       } else {
		    qDebug() << "Remove Items...";
		    // FIXME create a freelist function for the configuration file
		    QList<QString> urlListD = urlList;
		    urlListD.removeAll(item);
	       }
	  }
     } else {
	  history = new QTreeWidgetItem(logTree);
	  historyItem->setIcon(0, QIcon(QString::fromUtf8(":/images/images/book.png")));
	  ItemListHistory.push_front(historyItem);
	  history->setText(0, "No Url Cache");
     }
     delete tmpFile;

}

void mwClass::logFromHistory() {
     QString url2;
     url2 = logTree->currentItem()->text(0);
     qDebug() << "Path Call Item::" << logTree->currentItem()->text(0);
     if(url2.compare(url)) {
	  url = url2;
	  logReader();
     } else {
	  qDebug() << "Is equal...";
     }
}
