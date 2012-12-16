/*
Copyright 2011-2012  Francesco Cecconi <francesco.cecconi@gmail.com>

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

#ifndef LOGWRITER_H
#define LOGWRITER_H

#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtCore/QString>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>
#include <QtCore/QList>

#include "pobjects.h"

class LogWriter
{

public:
    enum LogType {
        FancyLog,
        RawLog,
        HtmlLog
    };

    LogWriter();
    ~LogWriter() {};

    /**
     * Save log for a single selected host
     **/
    void writeSingleLogFile(PObject* pObject, const QString& path);
    /**
     * Save all not empty log for scanned hosts
     **/
    void writeAllLogFile(QList<PObject*> pObjectList, const QString& path);

private:
    PObject* m_pObject;

    void writeRawLogFormat(const QString& path);
    void writeFancyLogFormat(const QString& path);
    void writeHtmlLogFormat(const QString& path);
};

#endif // LOGWRITER_H
