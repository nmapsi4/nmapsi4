/***************************************************************************
 *   Copyright (C) 2011-2012 by Francesco Cecconi                          *
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

#ifndef PARSERMANAGER_H
#define PARSERMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QDir>
#include <QtCore/QDateTime>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QMessageBox>
#include <QtGui/QSplitter>

// local inclusion
#include "pobjects.h"
#include "memorytools.h"
#include "logwriter.h"
#include "regularexpression.h"

class MainWindow;

class ParserManager : public QObject
{
    Q_OBJECT

public:
    ParserManager(MainWindow* parent = 0);
    ~ParserManager();
    /*
     * Clear all Items in parser QList
     */
    void clearParserItems();
    /*
     * Add a parserObjUtil in dedicate parser qlist
     */
    void addUtilObject(PObjectLookup* object);
    void syncSettings();

private:
    void showParserObj(int hostIndex);
    void showParserObjPlugins(int hostIndex);
    PObject* parserCore(const QStringList parList,QString StdoutStr,QString StderrorStr,QTreeWidgetItem* mainTreeE);

    MainWindow* m_ui;
    QList<PObject*> m_parserObjList;
    QList<PObjectLookup*> m_parserObjUtilList;
    QList<QTreeWidgetItem*> m_itemListScan;
    QList<QTreeWidgetItem*> m_objectItems;
    QList<QTreeWidgetItem*> m_treeItems;
    QSplitter *m_rawlogHorizontalSplitter;

public slots:
    /*
     * Start data QByteArray scan result parser.
     */
    void startParser(const QStringList parList, QByteArray dataBuffer,  QByteArray errorBuffer);
    void showParserResult(QTreeWidgetItem *item, int column);
    void showParserTracerouteResult(QTreeWidgetItem *item, int column);
    void callSaveSingleLogWriter();
    void callSaveAllLogWriter();
};

#endif // PARSER_H
