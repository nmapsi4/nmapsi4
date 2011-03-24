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

#include "../mainwin.h"

void nmapClass::startProfile_ui()   // start preference UI
{
    dialog->show();
    connect(dialog, SIGNAL(accepted()),
            this, SLOT(readProfile()));
}


void nmapClass::checkNmapVersion()
{
    versionProc = new QProcess();

    connect(versionProc, SIGNAL(finished(int, QProcess::ExitStatus)),
            this, SLOT(setNmapVersion()));

    QStringList parametri;
    parametri << "--version";
    versionProc->start("nmap", parametri);

}

void nmapClass::setNmapVersion()
{
    QString* output;
    // nmap CHECK
    hostEdit->setEnabled(true);
    output = new QString(versionProc->readAllStandardOutput());

    QTextStream stream(output);
    QString tmp, versionNmap;

    while (!stream.atEnd()) {
        tmp = stream.readLine();

        if (tmp.contains("Nmap version")) {
            versionNmap.append(tmp);
        }
    }

    labelVersion = new QLabel();

    if (!versionNmap.isEmpty()) {

        versionNmap.remove("Nmap version ");
        versionNmap.resize(4);
        versionNmap.prepend(tr("<b>Nmap Version:</b> "));
        versionNmap.append(" - <b>Ui</b>: ");
        versionNmap.append(_VERSION_);
        labelVersion->setText(versionNmap);
        statusBar()->addPermanentWidget(labelVersion, 1);
    } else {
        versionNmap.prepend(tr("<b>Nmap Version:</b> "));
        versionNmap.append(tr("Not Present"));
        action_Scan_menu->setEnabled(false);
        action_Scan_2->setEnabled(false);
        labelVersion->setText(versionNmap);
        statusBar()->addPermanentWidget(labelVersion, 6);
    }

    userMode = new QLabel();
    QString userModeString;
    userModeString.prepend(tr("<b>Mode:</b> "));
    if (!uid)
        userModeString.append(tr("Full"));
    else
        userModeString.append(tr("User"));

    userMode->setText(userModeString);
    statusBar()->addPermanentWidget(userMode, 6);

#ifndef TOOLS_NO_DEBUG
    qDebug() << "Nmapsi4/checkVersion --> nmapVersion::" << versionNmap;
    qDebug() << "Nmapsi4/checkVersion --> nmapsi4Mode::" << userModeString;
#endif
    delete output;
    delete versionProc;
}

void nmapClass::exit()
{
    emit killScan();
    //FIXME wait thread in list, dig List and internal lookup list
    if(th) {
	 th->quit();
         th->wait();
     }

    if (FileName != NULL) {

        QFile *tmpFile = new QFile();
        QString newFile = logPath;
        newFile.append("/");
	newFile = QDir::toNativeSeparators(newFile);
        newFile.append(FileName);
        tmpFile->setFileName(newFile);

        if ((checkLog) && (tmpFile->exists())) {
            tmpFile->close();
            this->isEmptyLog();
        } else
            if ((!checkLog) && (tmpFile->exists())) { // if log check is disable but the file exist
                tmpFile->close();
                tmpFile->remove();
#ifndef TOOLS_NO_DEBUG
                qDebug() << "nmapsi4/core --> deleteLog::true";
#endif
            }

        delete tmpFile;
    }

    // Save window size and position and NSS info
    saveUiSettings();
    this->close();
}

void nmapClass::stop_scan()
{
     emit killScan();
}
