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

#include "mainwin.h"

nmapClass::nmapClass()
     : PFile(NULL),
       labelVersion(NULL),
       userMode(NULL),
       dialog(0),
       th(NULL)
{
    int uid = 0;

#ifndef Q_WS_WIN
    uid = getuid();
#endif

    init();
    this->setNmapsiSlot();
    this->rootMode(uid); // send uid value
    this->checkProfile();
    this->optionListCreate();

    logHistory *historyScan_ = new logHistory(treeLogH, "nmapsi4/urlList", "nmapsi4/urlListTime", 10);
    historyScan_->updateBookMarks();
    delete historyScan_;
    logHistory *historyVuln_ = new logHistory(treeBookVuln, "nmapsi4/urlListVuln", "nmapsi4/urlListTimeVuln", 10);
    historyVuln_->updateBookMarks();
    delete historyVuln_;
}

void nmapClass::init()
{
    setupUi(this);
    // Disable scan action (nmap check)
    action_Scan_menu->setEnabled(false);
    action_Scan_2->setEnabled(false);
    hostEdit->setEnabled(false);
    actionAdd_Bookmark->setEnabled(false);


    hostEdit->setStyleSheet(QString::fromUtf8("color: rgb(153, 153, 153);"));
    hostEdit->insertItem(0, tr("Insert HostName to scan"));

    checkNmapVersion();
    listWscan->setColumnWidth(0, 400);
    treeLogH->setColumnWidth(0, 400);
    scanMonitor->setColumnWidth(0, 400);
    treeBookVuln->setColumnWidth(0, 400);
    QSettings settings("nmapsi4", "nmapsi4");
    QPoint pos = settings.value("window/pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("window/size", QSize(869, 605)).toSize();
    resize(size);
    move(pos);
}

void nmapClass::scan()
{
    if (hostEdit->currentText().isEmpty() && lineInputFile->text().isEmpty()) {
        QMessageBox::warning(this, "NmapSI4", tr("No Host Target\n"), tr("Close"));
        return;
    }

    if (checkLog) { // create a file log
        this->fileSession();
    } else {
        if (!logPath.contains(QDir::tempPath())) {
            QSettings ptr("nmapsi4", "nmapsi4");
            ptr.setValue("confPath", QDir::tempPath());
            logPath = QDir::tempPath();
            this->checkProfile();
        }

        this->fileSession();
    }


    QStringList parametri; //parameters list declaration
    QString hostname = hostEdit->currentText();
    QString title;

    history = new logHistory("nmapsi4/cacheHost", 10);
    history->addItemHistory(hostname);

    title.append("NmapSI4 ");

    //action_Scan_menu->setEnabled(false);
    //action_Scan_2->setEnabled(false);
    //hostEdit->setEnabled(false);
    actionStop_Scan->setEnabled(true);
    action_Save_As->setEnabled(false);
    actionSave_As_Menu->setEnabled(false);
    actionSave->setEnabled(false);
    actionSave_Menu->setEnabled(false);

    parametri = this->check_extensions(parametri, title); // extensions.cpp

    QString tmp_token;
    foreach(QString token, parametri) { // print scan options
        tmp_token.append(token);
        tmp_token.append(" "); // add simple space in option string
    }

    lineOptions->setText(tmp_token);

    parametri << hostname; // parameters list
    
    QMutex mutex;

    th = new scanThread(&Byte1, &Byte2, parametri, this);

    connect(th, SIGNAL(upgradePR()),
      this, SLOT(setProgress())); // nmapParser.cpp

    addMonitorHost(scanMonitor, hostname);
    
    mutex.lock();
    th->start();
    connect(th, SIGNAL(threadEnd(QString)),
      this, SLOT(nmapParser(QString))); // nmapParser.cpp
    mutex.unlock();
    delete history;
}

nmapClass::~nmapClass()
{
#ifndef MAIN_NO_DEBUG
    qDebug() << "Nmapsi4/~nmapClass() -> Global";
    qDebug() << "Nmapsi4/~nmapClass() -> Size Item List::" << itemList.size();
#endif
    if (dialog) dialog->close();
    itemDeleteAll(itemList);
    itemDeleteAll(monitorElem);
    
    delete progressScan;
    delete PFile;
    delete labelVersion;
    delete userMode;
}

