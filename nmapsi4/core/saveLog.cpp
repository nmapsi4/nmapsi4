/***************************************************************************
 *   Copyright (C) 2007-2010 by Francesco Cecconi                          *
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

#include "../mainwin.h"

void nmapClass::saveAsLog()
{

    if ((FileName == NULL) || (listClearFlag)) {
        QMessageBox::critical(this, tr("Save Log"),
                              tr("No Scan found\n"), tr("Close"));
        return;
    }

    QFile *tmpFile = new QFile();
    QString newFile = logPath;

#ifndef Q_WS_WIN
    newFile.append("/");
#else
    newFile.append("\\");
#endif

    newFile.append(FileName);
    tmpFile->setFileName(newFile);
#ifndef SAVELOG_NO_DEBUG
    qDebug() << "File::" << newFile << tmpFile->exists();
#endif

    if (!tmpFile || !tmpFile->size()) {
        QMessageBox::critical(this, tr("Save Log"),
                              tr("No Scan found\n"), tr("Close"));
        delete tmpFile;
        return;
    }

    QString url;
    url = QDir::homePath();

#ifndef Q_WS_WIN
    url.append("/");
#else
    url.append("\\");
#endif
    url.append("Si4");
    url.append(_VERSION_);
    url.append("-");
    url.append(hostEdit->currentText());
    url.append(".log");
#ifndef SAVELOG_NO_DEBUG
    qDebug() << "Nmapsi4/saveLog::-->" << url;
#endif

    QString FileNameTmp;
    FileNameTmp = QFileDialog::getSaveFileName(this, tr("Save Log"),
                  url, tr("Log (*.log)"));

#ifndef SAVELOG_NO_DEBUG
    qDebug() << "Nmapsi4/saveLog::-->" << FileName;
#endif

    if (FileNameTmp != "") {

        QFile *freeFile = new QFile();
        freeFile->setFileName(FileNameTmp);
        if (freeFile->exists())
            freeFile->remove();
        delete freeFile;

        if (!tmpFile->copy(FileNameTmp))
            QMessageBox::critical(this, tr("Information"),
                                  tr("Save File permission Error (Log stored in /tmp)\n"), tr("Close"));

        logSessionFile = FileNameTmp;
    }

    delete tmpFile;
}

void nmapClass::saveLog()
{

    if (logSessionFile.isEmpty())
        return;

    QFile *tmpFile = new QFile();
    QString newFile = logPath;

#ifndef Q_WS_WIN
    newFile.append("/");
#else
    newFile.append("\\");
#endif

    newFile.append(FileName);
    tmpFile->setFileName(newFile);

    QFile *freeFile = new QFile();
    freeFile->setFileName(logSessionFile);
    if (freeFile->exists())
        freeFile->remove();
    delete freeFile;

    if (!tmpFile->copy(logSessionFile))
        QMessageBox::critical(this, tr("Information"),
                              tr("Save File permission Error\n"), tr("Close"));

    delete tmpFile;
}
