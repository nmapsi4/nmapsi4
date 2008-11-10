/***************************************************************************
 *   Copyright (C) 2007-2008 by Francesco Cecconi                          *
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
    QMessageBox::aboutQt(this, "Qt Version");
}

void nmapClass::show_browser(QLineEdit *location)
{
    QString FileName;

    FileName = QFileDialog::getOpenFileName(this, QApplication::translate("nmapClass", "Select the file",
                                            0, QApplication::UnicodeUTF8), "/home", "");

    location->setText(FileName);
}


void nmapClass::show_log_browserUrl(QString url, QLineEdit *location)
{
    QString FileName;

    FileName = QFileDialog::getExistingDirectory(this, "Open Directory",
               url, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    location->setText(FileName);
}

void nmapClass::input_browser()
{
    this->show_browser(lineInputFile);
}

void nmapClass::exit()
{
    if (proc)
        proc->terminate();

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
            if ((!checkLog) && (tmpFile->exists())) { // if log check is disable but the file exit
                tmpFile->close();
                tmpFile->remove();
                qDebug() << "nmapsi4/core --> deleteLog::true";
            }

        delete tmpFile;
    }

    // Save window size and position
    QSettings settings("nmapsi4", "nmapsi4");
    if (savePos) settings.setValue("window/pos", pos());
    if (saveSize) settings.setValue("window/size", size());
    this->close();
}

void nmapClass::stop_scan()
{
    if (proc) {

        QString tmp_line = hostEdit->currentText();
        tmp_line.append(" (Stopped)"); // update hostEdit with stopped string (don't traslate)
        hostEdit->setItemText(0, tmp_line);

        proc->terminate();

        progressScan->setValue(0); // clear UI element
        actionStop_Scan->setEnabled(false);
        actionClear_History->setEnabled(true);
    }
}

QFile* nmapClass::create_logFile(QString Path)
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
            qDebug() << "nmapsi4/core --> deleteLog::newPath";
        }
    }
}


void nmapClass::fileSession()
{
    int FLAGS = 0;

    if ((!PFile) || (PFile && (firstPath != logPath) && (FLAGS = 1))) {

        if (FLAGS) {
            QDir::setCurrent(firstPath);
            qDebug() << "DEBUG::SESSION:: " << PFile << firstPath << logPath;
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
    dialog = new mainProfile();
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
    action_Scan_menu->setEnabled(true);
    action_Scan_2->setEnabled(true);
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

    qDebug() << "Nmapsi4/checkVersion --> nmapVersion::" << versionNmap;
    qDebug() << "Nmapsi4/checkVersion --> nmapsi4Mode::" << userModeString;
    delete output;
    delete versionProc;
}

void nmapClass::itemDeleteAll(QList<QTreeWidgetItem*> items)
{
    qDebug() << "Nmapsi4/itemDeleteAll() -> Free List";
    foreach(QTreeWidgetItem *item, items) {
        delete item;
    }
}

void nmapClass::callScanH()
{
    if (treeLogH->currentItem() && !proc) {
	qDebug() << "Current Item::" << treeLogH->currentItem();
        hostEdit->setStyleSheet(QString::fromUtf8(""));
        hostEdit->disconnect(SIGNAL(editTextChanged(QString)));
        hostEdit->setItemText(0, treeLogH->currentItem()->text(0));
        toolBox->setCurrentIndex(0);
        scan();
    }
}

void nmapClass::callSearchHistory()
{
    // TODO create a history global call
    if (!actionAdd_Bookmark->isEnabled())
        actionAdd_Bookmark->setEnabled(true);
    logHistory *history = new logHistory("nmapsi4/cacheHost", 10);
    history->searchHistory(hostEdit->currentText(), hostEdit);
    delete history;
}

void nmapClass::saveBookMarks()
{
    if (hostEdit->currentText().isEmpty())
        return;

    logHistory *history = new logHistory(treeLogH, "nmapsi4/urlList", "nmapsi4/urlListTime", -1);
    history->addItemHistory(hostEdit->currentText(), QDateTime::currentDateTime().toString("ddd MMMM d yy - hh:mm:ss.zzz"));
    history->updateBookMarks();
    delete history;
}

void nmapClass::deleteBookMark()
{
    // TODO error message
    if(!treeLogH->currentItem())
	return;
    logHistory *history = new logHistory(treeLogH, "nmapsi4/urlList", "nmapsi4/urlListTime", -1);
    history->deleteItemBookmark(treeLogH->currentItem()->text(0));
    delete history;
}
