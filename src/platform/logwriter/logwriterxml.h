/*
Copyright 2012-2013  Francesco Cecconi <francesco.cecconi@gmail.com>

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

#ifndef LOGWRITERXML_H
#define LOGWRITERXML_H

#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QDebug>
#include <QtCore/QXmlStreamWriter>
#include <QtCore/QXmlStreamReader>
#include <QTreeWidget>

const double minConfigVersion = 0.1;

class LogWriterXml
{

public:
    LogWriterXml();
    ~LogWriterXml();

    bool writeXmlDiscoverLog(const QString& fileName, QTreeWidget* widget);
    QList<QTreeWidgetItem*> readXmlDiscoverLog(const QString& fileName, QTreeWidget* widget);

private:
    bool readHosts(const QString& fileName, QTreeWidget* widget, QList<QTreeWidgetItem*>& treeWidgetItemlist);

};

#endif // LOGWRITERXML_H
