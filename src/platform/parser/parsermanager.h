/*
Copyright 2011-2014  Francesco Cecconi <francesco.cecconi@gmail.com>

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

#ifndef PARSERMANAGER_H
#define PARSERMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QDir>
#include <QtCore/QDateTime>
#include <QTreeWidgetItem>
#include <QMessageBox>
#include <QSplitter>

// local inclusion
#include "pobjects.h"
#include "memorytools.h"
#include "logwriter.h"
#include "regularexpression.h"
#include "notify.h"

class MainWindow;

class ParserManager : public QObject
{
    Q_OBJECT

public:
    explicit ParserManager(MainWindow* parent);
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
    void startParser(const QStringList parList, QByteArray dataBuffer,  QByteArray errorBuffer, int id);

private:
    void showParserObj(int hostIndex);
    void showParserObjPlugins(int hostIndex);
    void setPortItem(QTreeWidgetItem* item, const QStringList& details, bool& isPortDescriptionPresent);
    PObject* parserCore(const QStringList parList, QByteArray StdoutStr, QByteArray StderrorStr, QTreeWidgetItem* mainTreeE);

    MainWindow* m_ui;
    QList<PObject*> m_parserObjList;
    QList<PObjectLookup*> m_parserObjUtilList;
    QList<QTreeWidgetItem*> m_itemListScan;
    QList<QTreeWidgetItem*> m_treeItems;
    QSplitter *m_rawlogHorizontalSplitter;

public slots:
    void callSaveSingleLogWriter();
    void callSaveAllLogWriter();

private slots:
    /*
     * Start data QByteArray scan result parser.
     */
    void showParserResult(QTreeWidgetItem *item, int column);
    void showParserTracerouteResult(QTreeWidgetItem *item, int column);
};

#endif // PARSER_H
