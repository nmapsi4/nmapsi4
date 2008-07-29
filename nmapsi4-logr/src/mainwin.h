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

#ifndef MAINWIN_H
#define MAINWIN_H

#include <QString>
#include <QTreeWidgetItem>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QSettings>
#include <QDir>
#include <QFileDialog>
#include <assert.h>
#include "ui_mainwin.h"

class mwClass : public QMainWindow , private Ui::mWindow
{
     Q_OBJECT

private:
     QString showBrowser();
     void itemDeleteAll(QList<QTreeWidgetItem*> itemsList);

     QTreeWidgetItem *root;
     QTreeWidgetItem *item;
     QTreeWidgetItem *history;
     QTreeWidgetItem *historyItem;
     QFile *logF;
     QList<QTreeWidgetItem*> ItemList;
     QList<QTreeWidgetItem*> ItemListHistory;
     
     // Hostory (FIXME create a history class)
     QList<QString> historyReadUrl();
     void historyUpdateUrl(QString url);
     void updateThFile();

public:
     mwClass();
     ~mwClass();

protected:
     QString url;

private slots:
     void logReader();
     void logFromHistory();
     void Breader();
     void exit();


};

#endif
