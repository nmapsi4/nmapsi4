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

#include "mainwin.h"

mwClass::mwClass() : logF(0)
{
    initGUI();
    QTimer::singleShot(0, this, SLOT(initObject()));
}

void mwClass::initGUI() 
{
    setupUi(this);
    connect(actionOpen, SIGNAL(triggered()),
            this, SLOT(Breader()));
    connect(actionClose, SIGNAL(triggered()),
            this, SLOT(exit()));
    connect(actionQuit, SIGNAL(triggered()),
            this, SLOT(exit()));
    connect(action_About, SIGNAL(triggered()),
            this, SLOT(about()));
    connect(actionAbout_Qt, SIGNAL(triggered()),
            this, SLOT(about_qt()));
    connect(logTree, SIGNAL(itemSelectionChanged()),
            this, SLOT(logFromHistory()));
}

void mwClass::initObject() 
{
    // Take nmapsi4 geometry info
    QSettings settings("nmapsi4", "nmapsi4");
    QPoint pos = settings.value("window-logr/pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("window-logr/size", QSize(869, 605)).toSize();
    nHost = settings.value("hostCache").toInt();
    resize(size);
    move(pos);
    logTree->setColumnWidth(0, 200);
    logTree->setColumnWidth(1, 150);
    logHistory *history = new logHistory(logTree, "logReader/urlList", "logReader/urlListTime", nHost);
    history->updateLogHistory();
    delete history;
    
    cW = new QSplitter();
    cW->setOrientation(Qt::Horizontal);
    cW->addWidget(treeLogView);
    cW->addWidget(logTree);
    centralWidget()->layout()->addWidget(cW);
    cW->restoreState(settings.value("splitterSizesLogr").toByteArray());
}

void mwClass::Breader()
{
    url = showBrowser();
    logReader();
}

void mwClass::logReader()
{

    if (url.isEmpty())
    {
        return;
    }

    logHistory *history = new logHistory(logTree, "logReader/urlList", "logReader/urlListTime", nHost);
    history->addItemHistory(url, QDateTime::currentDateTime().toString("ddd MMMM d yy - hh:mm:ss.zzz"));

    if (!logF) 
    { 
        logF = new QFile();
    }
    
    logF->setFileName(url);
    if (!logF->open(QIODevice::ReadOnly)) 
    {
        return;
    }

    QTextStream buffer(logF);
    QString tmpLine;

    treeLogView->setIconSize(QSize(32, 32));

    while (!buffer.atEnd()) 
    {
        tmpLine = buffer.readLine();

        if (tmpLine.contains("==LogStart"))
        {
            tmpLine = buffer.readLine();
            if (!(treeLogView->findItems(tmpLine, Qt::MatchFixedString, 0)).size())
            {
                root = new QTreeWidgetItem(treeLogView);
                ItemList.push_front(root);
                root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit.png")));
                root->setText(0, tmpLine);
                while (!tmpLine.contains("==LogEnd"))
                {
                    tmpLine = buffer.readLine();

                    if (!tmpLine.contains("==LogEnd") && !tmpLine.isEmpty())
                    {
                        item = new QTreeWidgetItem(root);
                        ItemList.push_front(item);
                        item->setText(0, tmpLine);
                    }

                }
            }
        }
    }

    history->updateLogHistory();
    logF->close();
    delete history;
}

mwClass::~mwClass()
{
    itemDeleteAll(ItemList);
    delete cW;
}
