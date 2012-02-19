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

#ifndef LOGWRITER_H
#define LOGWRITER_H

#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtCore/QString>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>
#include <QtCore/QList>

#include "parserObjects.h"

class LogWriter
{

public:
    LogWriter();
    ~LogWriter() {};

    /**
     * Save log for a single selected host
     **/
    void writeSingleLogFile(parserObj* pObject, const QString& path);
    /**
     * Save all not empty log for scanned hosts
     **/
    void writeAllLogFile(QList<parserObj*> pObjectList, const QString& path);

private:
    enum LogType {
        FancyLog,
        RawLog,
        HtmlLog
    };

    parserObj* m_pObject;

    void writeRawLogFormat(const QString& path);
    void writeFancyLogFormat(const QString& path);
    void writeHtmlLogFormat(const QString& path);
};

#endif // LOGWRITER_H
