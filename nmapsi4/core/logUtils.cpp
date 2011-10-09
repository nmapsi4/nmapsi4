/***************************************************************************
 *   Copyright (C) 2007-2011 by Francesco Cecconi                          *
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

QFile* nmapClass::create_logFile(const QString Path)
{
    QFile *Pfile = new QFile();
    QDir::setCurrent(Path);
    Pfile->setFileName(FileName);
    return Pfile;
}

void nmapClass::fileSession()
{
    int FLAGS = 0;

    if ((!PFile) || (PFile && (firstPath != logPath) && (FLAGS = 1))) 
    {

        if (FLAGS) 
        {
            QDir::setCurrent(firstPath);
#ifndef TOOLS_NO_DEBUG
            qDebug() << "DEBUG::SESSION:: " << PFile << firstPath << logPath;
#endif
            PFile->remove();
        }

        FileName = QDate::currentDate().toString("[dd-MM-yyyy]");
        FileName.append(QTime::currentTime().toString("[h-m-s]"));
        FileName.append("-nmapsi4.log");
        firstPath = logPath;
        PFile = create_logFile(logPath);
        PFile->open(QIODevice::WriteOnly | QIODevice::Text);
    }
}

void nmapClass::isEmptyLog()
{
    qint64 tmp = PFile->size();

    if (!tmp) 
    {
        QDir::setCurrent(logPath);
        PFile->setFileName(FileName);
        PFile->remove();
    } 
    else if (firstPath.compare(logPath)) 
    {
        QString pathTmp = logPath;
        pathTmp.append("/");
        pathTmp = QDir::toNativeSeparators(pathTmp);
        pathTmp.append(FileName);

        if (!PFile->copy(FileName, pathTmp)) 
        {
            QMessageBox::critical(this, "NmapSI4",
                                  tr("Save File permission Error (Log stored in /tmp)\n"), tr("Close"));
        } 
        else 
        {
            PFile->remove();
        }
    }
}

void nmapClass::saveAsLog()
{

    if ((FileName == NULL) || (listClearFlag)) 
    {
        QMessageBox::critical(this, tr("Save Log"), tr("No Scan found\n"), tr("Close"));
        return;
    }

    QFile *tmpFile = new QFile();
    QString newFile = logPath;
    newFile.append("/");
    newFile = QDir::toNativeSeparators(newFile);
    newFile.append(FileName);
    tmpFile->setFileName(newFile);

    if (!tmpFile || !tmpFile->size()) 
    {
        QMessageBox::critical(this, tr("Save Log"),tr("No Scan found\n"), tr("Close"));
        delete tmpFile;
        return;
    }

    QString url;
    url = QDir::homePath();
    url.append("/");
    url = QDir::toNativeSeparators(url);
    url.append("si4");
    url.append(_VERSION_);
    url.append("-");
    url.append(hostEdit->currentText());
    url.append(".log");

    QString FileNameTmp;
    FileNameTmp = QFileDialog::getSaveFileName(this, tr("Save Log"), url, tr("Log (*.log)"));

    if (!FileNameTmp.isEmpty()) 
    {

        QFile *freeFile = new QFile();
        freeFile->setFileName(FileNameTmp);
        if (freeFile->exists())
        {
            freeFile->remove();
        }
        delete freeFile;

        if (!tmpFile->copy(FileNameTmp))
        {
            QMessageBox::critical(this, tr("Information"),
                                  tr("Save File permission Error (Log stored in /tmp)\n"), tr("Close"));
        }

        logSessionFile = FileNameTmp;
    }

    delete tmpFile;
}

void nmapClass::saveLog()
{

    if (logSessionFile.isEmpty()) 
    {
        return;
    }

    QFile *tmpFile = new QFile();
    QString newFile = logPath;
    newFile.append("/");
    newFile = QDir::toNativeSeparators(newFile);
    newFile.append(FileName);
    tmpFile->setFileName(newFile);

    QFile *freeFile = new QFile();
    freeFile->setFileName(logSessionFile);
    
    if (freeFile->exists()) 
    {
        freeFile->remove();
    }
    
    delete freeFile;

    if (!tmpFile->copy(logSessionFile)) 
    {
        QMessageBox::critical(this, tr("Information"),
                              tr("Save File permission Error\n"), tr("Close"));
    }

    delete tmpFile;
}
