/***************************************************************************
 *   Copyright (C) 2012 by Francesco Cecconi                               *
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

#ifndef LOGWRITERXML_H
#define LOGWRITERXML_H

#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QDebug>
#include <QtCore/QXmlStreamWriter>
#include <QtCore/QXmlStreamReader>
#include <QtGui/QTreeWidget>

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
