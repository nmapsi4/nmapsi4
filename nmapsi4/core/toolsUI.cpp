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


void nmapClass::about()
{
    mainAbout about;
    about.exec();
}

void nmapClass::about_qt()
{
    mainAbout about;
    about.qt();
}

void nmapClass::show_browser(QLineEdit *location)
{
    QString FileName;

    FileName = QFileDialog::getOpenFileName(this, QApplication::translate("nmapClass", "Select the file",
                                            0, QApplication::UnicodeUTF8), "/home", "");

    location->setText(FileName);
}


void nmapClass::show_log_browserUrl(const QString url, QLineEdit *location)
{
    QString FileName;

    FileName = QFileDialog::getExistingDirectory(this, "Open Directory",
               url, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(FileName.isEmpty()) {
        FileName.append(QDir::tempPath());
    }
    location->setText(FileName);
}

void nmapClass::input_browser()
{
    this->show_browser(lineInputFile);
}

void nmapClass::exit()
{
    emit killScan();
    if(th) {
	 th->quit();
         th->wait();
     }

    if (FileName != NULL) {

        QFile *tmpFile = new QFile();
        QString newFile = logPath;
#ifndef Q_WS_WIN
        newFile.append("/");
#else
        newFile.append("\\");
#endif
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

QFile* nmapClass::create_logFile(const QString Path)
{

    QFile *Pfile = new QFile();
    QDir::setCurrent(Path);
    Pfile->setFileName(FileName);
    return Pfile;
}

void nmapClass::isEmptyLog()
{

    qint64 tmp = PFile->size();

    if (!tmp) {
        QDir::setCurrent(logPath);
        PFile->setFileName(FileName);
        PFile->remove();
        qDebug() << "nmapsi4/core --> deleteLog::empty";
    } else if (firstPath.compare(logPath)) {
        QString pathTmp = logPath;
#ifndef Q_WS_WIN
        pathTmp.append("/");
#else
        pathTmp.append("\\");
#endif
        pathTmp.append(FileName);

        if (!PFile->copy(FileName, pathTmp))
            QMessageBox::critical(this, "NmapSI4",
                                  tr("Save File permission Error (Log stored in /tmp)\n"), tr("Close"));
        else {
            PFile->remove();
#ifndef TOOLS_NO_DEBUG
            qDebug() << "nmapsi4/core --> deleteLog::newPath";
#endif
        }
    }
}


void nmapClass::fileSession()
{
    int FLAGS = 0;

    if ((!PFile) || (PFile && (firstPath != logPath) && (FLAGS = 1))) {

        if (FLAGS) {
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
    int uid = 0;

    // nmap CHECK
    hostEdit->setEnabled(true);

#ifndef Q_WS_WIN
    uid = getuid();
#endif

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

void nmapClass::itemDeleteAll(QList<QTreeWidgetItem*> items)
{
#ifndef TOOLS_NO_DEBUG
    qDebug() << "Nmapsi4/itemDeleteAll() -> Free List";
#endif
    qDeleteAll(items);
    items.clear();
}

void nmapClass::callScanH()
{
     if(treeLogH->currentItem()) {
#ifndef TOOLS_NO_DEBUG
	qDebug() << "Current Item::" << treeLogH->currentItem();
#endif
        hostEdit->setStyleSheet(QString::fromUtf8(""));
        hostEdit->disconnect(SIGNAL(editTextChanged(QString)));
	// clear history setItemText fails
	hostEdit->clear();
	hostEdit->insertItem(0, treeLogH->currentItem()->text(0));
        SWscan->setCurrentIndex(0);
        startScan();
    }
}

void nmapClass::createBar() {
    // QToolBar asction
    scanSez = new QAction(this);
    logSez = new QAction(this);
    vulnSez = new QAction(this);
    nssAct = new QAction(this);
    parAct = new QAction(this);

    actSearch = new QAction(this);
    actBack = new QAction(this);
    actForward = new QAction(this);
    actStop = new QAction(this);

    actTabLook = new QAction(this);
    actTabTrace = new QAction(this);
    actTabMonitor = new QAction(this);

    // Section QAction
    scanSez->setIcon(QIcon(QString::fromUtf8(":/images/images/network_local.png")));
    scanSez->setIconText(tr("Scan"));
    scanSez->setToolTip(tr("Scan host(s)"));

    logSez->setIcon(QIcon(QString::fromUtf8(":/images/images/book.png")));
    logSez->setIconText(tr("Log"));
    logSez->setToolTip(tr("Scan Log"));

    vulnSez->setIcon(QIcon(QString::fromUtf8(":/images/images/viewmag+.png")));
    vulnSez->setIconText(tr("Services"));
    vulnSez->setToolTip(tr("Check Vulnerabilities"));

    nssAct->setIcon(QIcon(QString::fromUtf8(":/images/images/network_local.png")));
    nssAct->setIconText(tr("Nss Script"));
    nssAct->setToolTip(tr("Enable/Disable NSS script"));
    nssAct->setCheckable(true);

    parAct->setIcon(QIcon(QString::fromUtf8(":/images/images/show-menu.png")));
    parAct->setToolTip(tr("Enable/Disable Manual Parameters"));
    parAct->setCheckable(true);

    sezBar->addAction(scanSez);
    sezBar->addAction(logSez);
    sezBar->addAction(vulnSez);
    sezBar->addSeparator();
    sezBar->addAction(parAct);
    sezBar->addAction(nssAct);
    sezBar->setContextMenuPolicy(Qt::PreventContextMenu);

    actSearch->setIcon(QIcon(QString::fromUtf8(":/images/images/viewmag.png")));
    actSearch->setIconText(tr("Search"));
    actSearch->setEnabled(false);

    actBack->setIcon(QIcon(QString::fromUtf8(":/images/images/left.png")));
    actBack->setIconText(tr("Back"));
    actBack->setEnabled(false);

    actForward->setIcon(QIcon(QString::fromUtf8(":/images/images/right.png")));
    actForward->setIconText(tr("Forward"));
    actForward->setEnabled(false);

    actStop->setIcon(QIcon(QString::fromUtf8(":/images/images/button_cancel.png")));
    actStop->setIconText(tr("Stop"));
    actStop->setEnabled(false);

    toolBarSearch->addAction(actSearch);
    toolBarSearch->addAction(actBack);
    toolBarSearch->addAction(actForward);
    toolBarSearch->addAction(actStop);
    toolBarSearch->setContextMenuPolicy(Qt::PreventContextMenu);

    actTabLook->setIcon(QIcon(QString::fromUtf8(":/images/images/network-workgroup.png")));
    actTabLook->setToolTip(tr("Show/Hide Lookup"));
    actTabLook->setCheckable(true);

    actTabTrace->setIcon(QIcon(QString::fromUtf8(":/images/images/network-wired.png")));
    actTabTrace->setToolTip(tr("Show/Hide Traceroot"));
    actTabTrace->setCheckable(true);

    actTabMonitor->setIcon(QIcon(QString::fromUtf8(":/images/images/utilities-system-monitor.png")));
    actTabMonitor->setToolTip(tr("Show/Hide Scan Monitor"));
    actTabMonitor->setCheckable(true);

    toolBarTab->addAction(actTabLook);
    toolBarTab->addAction(actTabTrace);
    toolBarTab->addAction(actTabMonitor);
    toolBarTab->setContextMenuPolicy(Qt::PreventContextMenu);
}
