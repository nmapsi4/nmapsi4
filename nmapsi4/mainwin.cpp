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

#include "mainwin.h"

nmapClass::nmapClass()
     : PFile(NULL),
       scanCounter(0),
       labelVersion(NULL),
       userMode(NULL),
       th(NULL)
{
    initGUI();
    QTimer::singleShot( 0, this, SLOT(initObject()) );
}

void nmapClass::initGUI()
{
    setupUi(this);
    hostEdit->setStyleSheet(QString::fromUtf8("color: rgb(153, 153, 153);"));
    hostEdit->insertItem(0, tr("Insert [ip] or [dns] or [ip range] or [ip/dns list with space separator] to scan (ip range ex. 192.168.1.10/20)"));

    comboVulnRis->setStyleSheet(QString::fromUtf8("color: rgb(153, 153, 153);"));
    comboVulnRis->insertItem(0, tr("Search Vulnerabilities"));
}

void nmapClass::initObject() {

    int uid = 0;

#ifndef Q_WS_WIN
    uid = getuid();
#endif

#ifdef Q_WS_WIN
    tabWidget->removeTab(3);
#endif

    createBar();
    setNmapsiSlot();

    // Disable scan action (nmap check)
    action_Scan_menu->setEnabled(false);
    action_Scan_2->setEnabled(false);
    hostEdit->setEnabled(false);
    actionAdd_Bookmark->setEnabled(false);
    action_Add_BookmarkToolBar->setEnabled(false);
    toolBarSearch->setVisible(false);

    QString noHost(tr("no Host selected"));
    lineInfouptime->setText(noHost);
    lineInfotcpsequence->setText(noHost);
    lineInforunning->setText(noHost);
    lineInfodevice->setText(noHost);

    QString noService(tr("no Service selected"));
    servDesLabel->setText(noService);
    servPortLabel->setText(noService);

    setTreeWidgetValues();
    checkProfile();
    optionListCreate();

    digC  = new digSupport();
    digC->checkDigSupport();

    checkNmapVersion();

    listWscan->setColumnWidth(0, 300);
    treeLogH->setColumnWidth(0, 400);
    treeBookPar->setColumnWidth(0, 400);
    scanMonitor->setColumnWidth(0, 300);
    scanMonitor->setColumnWidth(1, 200);
    treeTraceroot->setColumnWidth(0, 250);
    treeTraceroot->setColumnWidth(1, 100);
    treeTraceroot->setColumnWidth(2, 200);
    treeTraceroot->setColumnWidth(3, 200);
    treeBookVuln->setColumnWidth(0, 400);
    cW = new QSplitter();
    cW->addWidget(treeMain);
    cW->addWidget(stackedMain);
    this->centralWidget()->layout()->addWidget(cW);

    QSettings settings("nmapsi4", "nmapsi4");
    QPoint pos = settings.value("window/pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("window/size", QSize(910, 672)).toSize();
    resize(size);
    move(pos);
    cW->restoreState(settings.value("splitterSizes").toByteArray());

    logHistory *historyScan_ = new logHistory(treeLogH, "nmapsi4/urlList", "nmapsi4/urlListTime", -1);
    historyScan_->updateBookMarks();
    delete historyScan_;

    logHistory *historyPar_ = new logHistory(treeBookPar, "nmapsi4/urlListPar", "nmapsi4/urlListTimePar", -1);
    historyPar_->updateBookMarks();
    delete historyPar_;

    logHistory *historyVuln_ = new logHistory(treeBookVuln, "nmapsi4/urlListVuln", "nmapsi4/urlListTimeVuln", -1);
    historyVuln_->updateBookMarks();
    delete historyVuln_;

    this->rootMode(uid); // send uid value
    dialog = new mainProfile();

    nssAct->setChecked(NSSsupport); // set NSS support
    parAct->setChecked(ADVSupport); // set ADV support
    parAdv();
    NSSCheck();
    updateComboPar();
}

void nmapClass::startScan() {

    if (hostEdit->currentText().isEmpty() && lineInputFile->text().isEmpty()) {
        QMessageBox::warning(this, "NmapSI4", tr("No Host Target\n"), tr("Close"));
        return;
    }

    QString hostname = hostEdit->currentText();
    // check wrong address
    hostname = clearHost(hostname);

    if(hostname.contains("/") && !hostname.contains("//")) {

        QStringList addrPart_ = hostname.split("/");
        QStringList ipBase_ = addrPart_[0].split(".");
#ifndef MAIN_NO_DEBUG
        qDebug() << "nmapsi4::startScan()::ipBase --> " << ipBase_;
        qDebug() << "nmapsi4::startScan()::addrPart --> " << addrPart_;
#endif

        int ipLeft_ = ipBase_[3].toInt();
        int ipRight_ = addrPart_[1].toInt();
#ifndef MAIN_NO_DEBUG
        qDebug() << "nmapsi4::startScan()::ipRight --> " << ipRight_;
        qDebug() << "nmapsi4::startScan()::ipLeft --> " << ipLeft_;
#endif

        for(int index = ipLeft_; index <= ipRight_; index++) {
            ipBase_[3].setNum(index);
            hostname = ipBase_.join(".");
#ifndef MAIN_NO_DEBUG
            qDebug() << "nmapsi4::startScan()::FullString --> " << hostname;
#endif
            // lookup disabled for group scan
            addMonitorHost(scanMonitor, hostname);
            this->scan(hostname);
         }
         return;
     }

    //scan token DNS/IP parser
    if(hostname.contains(" ")) { // space delimiter
        QStringList addrPart_ = hostname.split(" ");
        addrPart_.removeAll("");
#ifndef MAIN_NO_DEBUG
        qDebug() << "DEBUG::NewToken:: " << addrPart_.size();
        for(int index=0; index < addrPart_.size(); index++) {
            qDebug() << "DEBUG::NewToken::Part:: " << addrPart_[index];
        }
#endif

        if(addrPart_.size() > 1) {// check for only one space in hostname
            for(int index=0; index < addrPart_.size(); index++) {
                addrPart_[index] = clearHost(addrPart_[index]);
                addMonitorHost(scanMonitor, addrPart_[index]);
                this->scan(addrPart_[index]);
            }
            return;
        }
        hostname.remove(" ");
    }

    if(lookupInternal) {
        addMonitorHost(scanMonitor, hostname);
        lth = new lookUpT(hostname,this);
        connect(lth, SIGNAL(threadEnd(QHostInfo,int,const QString)),
                       this, SLOT(scanLookup(QHostInfo,int,const QString)));

        lth->start();
    } else if(lookupDig && digC->getDigSupport()) {
        digC->digProcess(hostname,treeLookup);
        addMonitorHost(scanMonitor, hostname);
        this->scan(hostname);
    } else {
        addMonitorHost(scanMonitor, hostname);
        this->scan(hostname);
    }
}

void nmapClass::scan(const QString hostname)
{

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

    history = new logHistory("nmapsi4/cacheHost", hostCache);
    history->addItemHistory(hostname);

    actionStop_Scan->setEnabled(true);
    action_Save_As->setEnabled(false);
    actionSave_As_Menu->setEnabled(false);
    actionSave->setEnabled(false);
    actionSave_Menu->setEnabled(false);

    if(!frameAdv->isVisible()) {
        parametri = this->check_extensions(); // extensions.cpp
    } else {
        parametri = comboAdv->lineEdit()->text().split(" ");
    }


    parametri << hostname; // add hostname
    
    QByteArray buff1, buff2;
    th = new scanThread(buff1, buff2, parametri, this);
    scanPointerList.push_front(th);

    connect(th, SIGNAL(upgradePR()),
      this, SLOT(setProgress())); // nmapParser.cpp
    
    th->start();
    connect(th, SIGNAL(threadEnd(const QString, QByteArray, QByteArray)),
      this, SLOT(nmapParser(const QString, QByteArray, QByteArray))); // nmapParser.cpp

    delete history;
}

nmapClass::~nmapClass()
{
#ifndef MAIN_NO_DEBUG
    qDebug() << "Nmapsi4/~nmapClass() -> Global";
    qDebug() << "Nmapsi4/~nmapClass() -> Size Item List::" << itemList.size();
#endif
    if(dialog) {
        dialog->close();
    }

    itemDeleteAll(itemList);
    itemDeleteAll(itemListLook);
    itemDeleteAll(monitorElem);
    itemDeleteAll(mainTreeElem);
    qDeleteAll(parserObjList);

    delete progressScan;
    delete PFile;
    delete labelVersion;
    delete userMode;
    delete digC;
    delete scanSez;
    delete logSez;
    delete vulnSez;
    delete nssAct;
    delete parAct;
    delete actBack;
    delete actForward;
    delete actStop;
    delete cW;
}

