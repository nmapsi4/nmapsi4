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

#include "logwriterxml.h"

LogWriterXml::LogWriterXml()
{

}

LogWriterXml::~LogWriterXml()
{

}

bool LogWriterXml::writeXmlDiscoverLog(const QString& fileName, QTreeWidget* widget)
{
    QFile xmlFile(fileName);
    if (!xmlFile.open(QFile::WriteOnly | QFile::Text)) {
        return false;
    }

    QXmlStreamWriter xmlStreamWriter(&xmlFile);
    xmlStreamWriter.setAutoFormatting(true);
    xmlStreamWriter.writeStartDocument();
    xmlStreamWriter.writeStartElement("discoveriplist");
    xmlStreamWriter.writeStartElement("config");
    xmlStreamWriter.writeAttribute("version", QString::number(minConfigVersion));

    for (int i = 0; i < widget->topLevelItemCount(); ++i) {
        xmlStreamWriter.writeStartElement("host");
        xmlStreamWriter.writeAttribute("ip", widget->topLevelItem(i)->text(0));
        xmlStreamWriter.writeAttribute("data", widget->topLevelItem(i)->text(1));
        xmlStreamWriter.writeEndElement();
    }

    xmlStreamWriter.writeEndElement();
    xmlStreamWriter.writeEndDocument();

    xmlFile.close();
    return true;
}

QList<QTreeWidgetItem*> LogWriterXml::readXmlDiscoverLog(const QString& fileName, QTreeWidget* widget)
{
    QList<QTreeWidgetItem*> treeWidgetItemlist;

    QFile xmlFile(fileName);
    if (!xmlFile.open(QFile::ReadOnly | QFile::Text)) {
        return treeWidgetItemlist;
    }

    QXmlStreamReader xmlReader(&xmlFile);
    while (!xmlReader.atEnd()) {
        if (xmlReader.readNextStartElement()) {
            if (xmlReader.name().toString().startsWith(QLatin1String("config"))) {
                QString versionReaded = xmlReader.attributes().value("version").toString();
                if (versionReaded.toDouble() == minConfigVersion) {
                    if (!readHosts(fileName, widget, treeWidgetItemlist)) {
                        qWarning() << "Xml writer:: file not readable";
                    }
                    break;
                }
            }
        }
    }

    return treeWidgetItemlist;
}

bool LogWriterXml::readHosts(const QString& fileName, QTreeWidget* widget, QList<QTreeWidgetItem*>& treeWidgetItemlist)
{
    QFile xmlFile(fileName);
    if (!xmlFile.open(QFile::ReadOnly | QFile::Text)) {
        return false;
    }

    QXmlStreamReader xmlReader(&xmlFile);
    while (!xmlReader.atEnd()) {
        if (xmlReader.readNextStartElement()) {
            if (xmlReader.name().toString().startsWith(QLatin1String("host"))) {
                qDebug() << "READ TAG:: " << xmlReader.name().toString();
                qDebug() << "READ TAG ip:: " << xmlReader.attributes().value("ip").toString();
                qDebug() << "READ TAG data:: " << xmlReader.attributes().value("data").toString();
                QTreeWidgetItem *item = new QTreeWidgetItem(widget);
                treeWidgetItemlist.push_back(item);
                item->setIcon(0, QIcon(QString::fromUtf8(":/images/images/flag_green.png")));
                item->setText(0, xmlReader.attributes().value("ip").toString());
                item->setText(1, xmlReader.attributes().value("data").toString());
            }
        }
    }

    return true;
}
