/***************************************************************************
 *   Copyright (C) 2011 by Francesco Cecconi                               *
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

#include "logwriter.h"

logWriter::logWriter()
{
}

void logWriter::writeAllLogFile(QList<parserObj*> pObjectList, const QString& path)
{
    foreach (parserObj* object, pObjectList)
    {
        if (object->isValidObject())
        {
            QString pathTmp(path);
            if (!pathTmp.endsWith(QDir::toNativeSeparators("/")))
            {
                pathTmp.append(QDir::toNativeSeparators("/"));
            }

            QString fileName(object->getHostName());
            fileName = fileName.replace('.','_');
            fileName.append(".log");

            pathTmp.append(fileName);

            writeSingleLogFile(object, pathTmp);
        }
    }
}

void logWriter::writeSingleLogFile(parserObj* pObject, const QString& path)
{
    m_pObject = pObject;

    // TODO check log type and call the private method
    QSettings settings("nmapsi4", "nmapsi4");

    int logType = settings.value("logType", 0).toInt();

    switch (logType)
    {
    case FancyLog:
        writeFancyLog(path);
        break;
    case RawLog:
        writeRawLog(path);
        break;
    }

}

void logWriter::writeFancyLog(const QString& path)
{
    QFile *filePtr = new QFile(path);

    if (!filePtr->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning() << "DEBUG::File Writer:: Problem with file open";
        return;
    }

    QTextStream fileStream(filePtr);

    // hostname
    fileStream << "\n" << m_pObject->getParameters() << "\n\n";
    fileStream << "|---------- Services" << "\n\n";

    // Open Ports
    foreach (const QString &token, m_pObject->getPortOpen())
    {
        fileStream << token << "\n";
    }

    // close Ports
    foreach (const QString &token, m_pObject->getPortClose())
    {
        fileStream << token << "\n";
    }

    // filtered/unfilteres Ports
    foreach (const QString &token, m_pObject->getPortFiltered())
    {
        fileStream << token << "\n";
    }

    fileStream << "\n|---------- General information" << "\n\n";

    // filtered/unfilteres Ports
    foreach (const QString &token, m_pObject->getMainInfo())
    {
        fileStream << token << "\n";
    }

    fileStream << "\n|---------- Nse result" << "\n";

    // Show Nss Info
    QHash<QString, QStringList> nseResult = m_pObject->getNseResult();
    QHash<QString, QStringList>::const_iterator i;

    for (i = nseResult.constBegin(); i != nseResult.constEnd(); ++i)
    {
        fileStream << "\n--- " << i.key() << "\n\n";

        foreach (const QString& value, i.value())
        {
            fileStream << value << "\n";
        }
    }

    fileStream << "\n|---------- Scan Errors/Warning" << "\n\n";

    // scan errors/Warning
    foreach (const QString &token, m_pObject->getErrorScan())
    {
        fileStream << token << "\n";
    }

    filePtr->close();
    delete filePtr;
}

void logWriter::writeRawLog(const QString& path)
{
    QFile *filePtr = new QFile(path);

    if (!filePtr->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning() << "DEBUG::File Writer:: Problem with file open";
        return;
    }

    QTextStream fileStream(filePtr);

    foreach (const QString& token, m_pObject->getFullScanLog())
    {
        fileStream << token << "\n";
    }

    filePtr->close();
    delete filePtr;
}
