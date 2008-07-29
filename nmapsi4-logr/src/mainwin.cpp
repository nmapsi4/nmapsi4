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


mwClass::mwClass() : logF(0), url(0) {
     setupUi(this);

     connect( actionOpen, SIGNAL(triggered()), 
	      this, SLOT(Breader()));

     connect( actionClose, SIGNAL(triggered()), 
	      this, SLOT(exit()));

     connect( actionQuit, SIGNAL(triggered()), 
	      this, SLOT(exit()));

     connect( logTree, SIGNAL(itemSelectionChanged()), 
           this, SLOT(logFromHistory()));

     // Take nmapsi4 geometry info
     QSettings settings("nmapsi4","nmapsi4");
     QPoint pos = settings.value("window/pos", QPoint(200, 200)).toPoint();
     QSize size = settings.value("window/size", QSize(869, 605)).toSize();
     resize(size);
     move(pos);

     // TEST
     //historyReadUrl();
     logHistory *history = new logHistory(logTree,"logReader/urlList");
     history->updateThFile();
     delete history;
}

void mwClass::Breader() {
     url = showBrowser();
     logReader();
}

void mwClass::logReader() {
     
     if(url.isEmpty())
	       return;
     
     qDebug() << "Path Current Item::" << url;

     logHistory *history = new logHistory(logTree,"logReader/urlList");
     history->historyUpdateUrl(url);

     if(!logF) logF = new QFile();
     qDebug() << "nmapsi4-logr:: --> url::" << url;
     logF->setFileName(url);
     if(!logF->open(QIODevice::ReadOnly)) {
	  qDebug() << "Log File open error." << endl;
	  return;
     }

     QTextStream buffer(logF);
     QString tmpLine;

     treeLogView->setIconSize(QSize::QSize (48, 48));

     while( !buffer.atEnd() ) {
	  tmpLine = buffer.readLine();

	  if(tmpLine.contains("Log Start")) {
	       root = new QTreeWidgetItem(treeLogView);
	       ItemList.push_front(root);
	       root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit.png")));
	       tmpLine = buffer.readLine();
	       root->setText(0,tmpLine);
	       while( !tmpLine.contains("Log End") ) {
		    item = new QTreeWidgetItem(root);
		    ItemList.push_front(item);
		    tmpLine = buffer.readLine();
		    
                    // FIXME Scan info
		    if(!tmpLine.contains("Log End"))
			 item->setText(0,tmpLine);

	       }
	  }
     }

     history->updateThFile();
     logF->close();
     delete history;

}

mwClass::~mwClass() 
{
     itemDeleteAll(ItemList);
     //delete logF;
}
