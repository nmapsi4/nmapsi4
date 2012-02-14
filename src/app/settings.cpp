/***************************************************************************
 *   Copyright (C) 2010-2012 by Francesco Cecconi                          *
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

#include "mainwindow.h"

void MainWindow::syncSettings()
{

    QSettings settings("nmapsi4", "nmapsi4");

    // TODO: load saved profile or simple default

    bool position = settings.value("savePos",true).toBool();
    m_savePos = position;

    bool size = settings.value("saveSize",true).toBool();
    m_saveSize = size;

    m_hostCache = settings.value("hostCache",10).toInt();

#ifdef Q_WS_WIN
    // disable lookup in MS windows
    m_lookupType = 0;
#else
    m_lookupType = settings.value("lookupType", 1).toInt();
#endif

    // restore actionMenuBar
    bool actionMB = settings.value("showMenuBar", false).toBool();
    actionMenuBar->setChecked(actionMB);

    // update max parallel scan option
    m_monitor->updateMaxParallelScan();
}

void MainWindow::saveSettings()
{
    QSettings settings("nmapsi4", "nmapsi4");

    if(m_savePos)
    {
        settings.setValue("window/pos", pos());
    }

    if(m_saveSize)
    {
        settings.setValue("window/size", size());
    }

    settings.setValue("savePos", m_savePos);
    settings.setValue("saveSize", m_saveSize);
    settings.setValue("hostCache",10);
    settings.setValue("splitterSizes", m_mainHorizontalSplitter->saveState());
    settings.setValue("splitterSizesRight", m_mainVerticalSplitter->saveState());
    settings.setValue("showMenuBar", actionMenuBar->isChecked());

// check and reset for settings file permission
#ifndef Q_WS_WIN
    if (!m_userId)
    {
        QString settingsFile_ = settings.fileName();
        QFileInfo fiS_(settingsFile_);
        if ((!fiS_.permissions().testFlag(QFile::WriteOther) && !fiS_.ownerId()))
        {
             QFile::setPermissions(settingsFile_, QFile::ReadOwner | QFile::ReadUser | QFile::ReadOther |
                QFile::WriteOwner | QFile::WriteUser | QFile::WriteOther);
        }
    }
#endif

#ifndef Q_WS_WIN
    if (!m_userId)
    {
        QSettings settings2("nmapsi4", "nmapsi4_bookmark");
        QString settingsFile_ = settings2.fileName();
        QFileInfo fiS_(settingsFile_);

        if ((!fiS_.permissions().testFlag(QFile::WriteOther) && !fiS_.ownerId()))
        {
            QFile::setPermissions(settingsFile_, QFile::ReadOwner | QFile::ReadUser | QFile::ReadOther |
                QFile::WriteOwner | QFile::WriteUser | QFile::WriteOther);
        }
    }

    if (!m_userId)
    {
        QSettings settings3("nmapsi4", "nmapsi4_gprofile");
        QString settingsFile_ = settings3.fileName();
        QFileInfo fiS_(settingsFile_);

        if ((!fiS_.permissions().testFlag(QFile::WriteOther) && !fiS_.ownerId()))
        {
           QFile::setPermissions(settingsFile_, QFile::ReadOwner | QFile::ReadUser | QFile::ReadOther |
               QFile::WriteOwner | QFile::WriteUser | QFile::WriteOther);
        }
    }
#endif
}
