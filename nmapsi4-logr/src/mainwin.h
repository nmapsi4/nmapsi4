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

#ifndef MAINWIN_H
#define MAINWIN_H

#include <QtCore/QString>
#include <QtGui/QTreeWidgetItem>
#include <QtCore/QList>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QtCore/QDir>
#include <QtGui/QFileDialog>
#include <QtCore/QDateTime>
#include <QtCore/QTimer>
#include <QtGui/QSplitter>
#include <QtGui/QLayout>
#include <assert.h>
#include <ui_mainwin.h>
#include "../../lib/history/loghistory.h"
#include "../../lib/about/about.h"
#include "logr-debug.h"


class mwClass : public QMainWindow , private Ui::mWindow
{
    Q_OBJECT

private:
    void initGUI();
    QString showBrowser();
    void itemDeleteAll(QList<QTreeWidgetItem*> itemsList);

    QTreeWidgetItem *root;
    QTreeWidgetItem *item;
    QFile *logF;
    QList<QTreeWidgetItem*> ItemList;

public:
    mwClass();
    ~mwClass();

protected:
    QString url;
    int nHost;
    QSplitter *cW;

private slots:
    void initObject();
    void logReader();
    void logFromHistory();
    void Breader();
    void exit();
    void about();
    void about_qt();
};
#endif
