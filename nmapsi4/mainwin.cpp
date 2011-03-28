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

#include "mainwin.h"

nmapClass::nmapClass()
     : uid(0),
       PFile(NULL),
       scanCounter(0),
       labelVersion(NULL),
       userMode(NULL) {
    initGUI();
    QTimer::singleShot( 0, this, SLOT(initObject()) );
}

void nmapClass::initGUI() {
    setupUi(this);
    hostEdit->setStyleSheet(QString::fromUtf8("color: rgb(153, 153, 153);"));
    hostEdit->insertItem(0, tr("Insert [ip] or [dns] or [ip range] or [ip/dns list with space separator] to scan (ip range ex. 192.168.1.10/20)"));
    comboVulnRis->setStyleSheet(QString::fromUtf8("color: rgb(153, 153, 153);"));
    comboVulnRis->insertItem(0, tr("Search Vulnerabilities"));
}

void nmapClass::initObject() {

#ifndef Q_WS_WIN
    uid = getuid();
#endif

#ifdef Q_WS_WIN
    // lookup fails on MS Windows
    tabWidget->removeTab(3);
#endif
    tWresult->setTabsClosable(true);
    webViewList.push_back(viewVuln);
    createBar();
    setNmapsiSlot();
    // Set default properties
    action_Scan_menu->setEnabled(false);
    action_Scan_2->setEnabled(false);
    hostEdit->setEnabled(false);
    actionAdd_Bookmark->setEnabled(false);
    action_Add_BookmarkToolBar->setEnabled(false);
    toolBarSearch->setVisible(false);
    scanSez->setChecked(true);
    Bdetails->setChecked(true);
    menuBar()->setContextMenuPolicy(Qt::PreventContextMenu);
    // preload mainwindow info 
    setTreeWidgetValues();
    checkProfile();
    optionListCreate();
    // check dig support
    digSupport *digC  = new digSupport();
    digC->checkDigSupport(digSupported);
    delete digC;
    // check nmap version
    checkNmapVersion();
    // set TreeWidget properties
    treeLogH->setColumnWidth(0, 400);
    treeBookPar->setColumnWidth(0, 400);
    scanMonitor->setColumnWidth(0, 300);
    scanMonitor->setColumnWidth(1, 350);
    //treeTraceroot->setColumnWidth(0, 250);
    treeTraceroot->setColumnWidth(1, 100);
    treeTraceroot->setColumnWidth(2, 200);
    treeTraceroot->setColumnWidth(3, 200);
    treeBookVuln->setColumnWidth(0, 400);
    treeWidgetVulnUrl->setColumnWidth(0, 400);
    treeMain->setColumnWidth(0, 200);
    // create mainwindow Qsplitter
    cW = new QSplitter();
    bW = new QSplitter();    
    cW->setOrientation(Qt::Horizontal);
    cW->addWidget(frameLeft);
    cW->addWidget(frameCenter);
    //frameCenter
    bW->setOrientation(Qt::Vertical);
    bW->addWidget(stackedMain);
    bW->addWidget(frameRight);
    // insert splitter
    tabUi->widget(0)->layout()->addWidget(cW);
    frameCenter->layout()->addWidget(bW);
    frameCenter->layout()->addWidget(frame_2);
    // restore window position
    QSettings settings("nmapsi4", "nmapsi4");
    QPoint pos = settings.value("window/pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("window/size", QSize(910, 672)).toSize();
    resize(size);
    move(pos);
    cW->restoreState(settings.value("splitterSizes").toByteArray());
    bW->restoreState(settings.value("splitterSizesRight").toByteArray());

    logHistory *historyScan_ = new logHistory(treeLogH, "nmapsi4/urlList", "nmapsi4/urlListTime", -1);
    historyScan_->updateBookMarks();
    delete historyScan_;
    // check for user or admin parameters bookmarks
    if (!uid) {
	logHistory *historyPar_ = new logHistory(treeBookPar, "nmapsi4/urlListPar", "nmapsi4/urlListTimePar", -1);
	historyPar_->updateBookMarks();
	delete historyPar_;
    } else {
	logHistory *historyPar_ = new logHistory(treeBookPar, "nmapsi4/urlListParUser", "nmapsi4/urlListTimeParUser", -1);
	historyPar_->updateBookMarks();
	delete historyPar_;
    }

    logHistory *historyVuln_ = new logHistory(treeBookVuln, "nmapsi4/urlListVuln", "nmapsi4/urlListTimeVuln", -1);
    historyVuln_->updateBookMarks();
    delete historyVuln_;
    
    logHistory *historyVulnUrl_ = new logHistory(treeWidgetVulnUrl, "nmapsi4/nameUrlVuln", "nmapsi4/nameUrlAddr", -1);
    historyVulnUrl_->updateBookMarks();
    delete historyVulnUrl_;

    this->rootMode(); // send uid value
    dialog = new mainProfile();
    m_dialogUrl = new mainUrlClass();

    nssAct->setChecked(NSSsupport); // set NSS support
    NSSCheck();
    parAct->setChecked(ADVSupport); // set ADV support
    parAdv();
    actTabMonitor->setChecked(MonitorEnabled);
    updateTabMonitor();
    actTabLook->setChecked(LookupEnabled);
    updateTabLook();
    actTabTrace->setChecked(TraceEnabled);
    updateTabTrace();
    // load quick combo items
    updateComboPar();
    updateComboBook();
    updateComboWebV();
    // call discover startup
    startDiscover();
}

void nmapClass::startScan() {

    if (hostEdit->currentText().isEmpty() && lineInputFile->text().isEmpty()) {
        QMessageBox::warning(this, "NmapSI4", tr("No Host Target\n"), tr("Close"));
        return;
    }

    QString hostname = hostEdit->currentText();
    // check wrong address
    hostname = clearHost(hostname);

    // check for ip list
    if(hostname.contains("/") && !hostname.contains("//")) {
	// not lookup, is a ip list
        QStringList addrPart_ = hostname.split('/');
        QStringList ipBase_ = addrPart_[0].split('.');
        int ipLeft_ = ipBase_[3].toInt();
        int ipRight_ = addrPart_[1].toInt();

        for(int index = ipLeft_; index <= ipRight_; index++) {
            ipBase_[3].setNum(index);
            hostname = ipBase_.join(".");
            addMonitorHost(scanMonitor, hostname);
            this->scan(hostname);
         }
         return;
     }

    //scan token DNS/IP parser
    if(hostname.contains(" ")) { // space delimiter
        QStringList addrPart_ = hostname.split(' ');
        addrPart_.removeAll("");
	// check for only one space in hostname
        if(addrPart_.size() > 1) {
	    // multiple ip or dns to scan
            for(int index=0; index < addrPart_.size(); index++) {
                addrPart_[index] = clearHost(addrPart_[index]);
		// dig lookup call
		preScanLookup(addrPart_[index]);
            }
            return;
        }
        // remove all space on hostname
        hostname.remove(' ');
    }

    // single ip or dns after the move
    preScanLookup(hostname);
    
}

void nmapClass::preScanLookup(const QString hostname) {
    // check for scan lookup
    if(lookupInternal) {
	// if internal lookUp is actived
        addMonitorHost(scanMonitor, hostname);
	// call internal lookup thread and save the pointer.
        lookUpT *internalLookupTh_ = new lookUpT(hostname,this);
	internealLookupList.push_back(internalLookupTh_);
	
        connect(internalLookupTh_, SIGNAL(threadEnd(QHostInfo,int,const QString)),
                       this, SLOT(scanLookup(QHostInfo,int,const QString)));

        internalLookupTh_->start();
    } else if(lookupDig && digSupported) {
	// if dig support is actived
	parserObjUtil* tmpParserObj_ = new parserObjUtil();
	digSupport *digC = new digSupport();
	digLookupList.push_back(digC);
        digC->digProcess(hostname,treeLookup,tmpParserObj_);
	parserObjUtilList.append(tmpParserObj_);
        addMonitorHost(scanMonitor, hostname);
        this->scan(hostname);
    } else {
	// lookup isn't actived or not supported
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


    QStringList parameters_; //parameters list declaration

    logHistory *history = new logHistory("nmapsi4/cacheHost", hostCache);
    history->addItemHistory(hostname);

    actionStop_Scan->setEnabled(true);
    action_Save_As->setEnabled(false);
    actionSave_As_Menu->setEnabled(false);
    actionSave->setEnabled(false);
    actionSave_Menu->setEnabled(false);

    if(!frameAdv->isVisible()) {
        parameters_ = this->check_extensions(); // extensions.cpp
    } else {
        parameters_ = comboAdv->lineEdit()->text().split(' ');
    }

    parameters_ << hostname; // add hostname
    
    QByteArray buff1, buff2;
    // start scan Thread
    QPointer<scanThread> th = new scanThread(buff1, buff2, parameters_, this);
    scanPointerList.push_front(th);
    // update progressbar for scan
    connect(th, SIGNAL(upgradePR()),
      this, SLOT(setProgress())); // nmapParser.cpp
    // read current data scan from the thread
    connect(th, SIGNAL(flowFromThread(const QString, const QString)),
      this, SLOT(readFlowFromThread(const QString, const QString)));
    // read scan data return
    connect(th, SIGNAL(threadEnd(const QStringList, QByteArray, QByteArray)),
      this, SLOT(nmapParser(const QStringList, QByteArray, QByteArray))); // nmapParser.cpp
    // start scan
    th->start();

    delete history;
}

nmapClass::~nmapClass() {
    if(dialog) {
        dialog->close();
    }
    
    if(m_dialogUrl) {
        m_dialogUrl->close();
    }
    
    itemDeleteAll(itemListScan);
    itemDeleteAll(digLookupList);
    itemDeleteAll(internealLookupList);
    itemDeleteAll(monitorElem);
    itemDeleteAll(mainTreeElem);
    itemDeleteAll(itemNseActive);
    itemDeleteAll(itemNseAvail);
    itemDeleteAll(parserObjList);
    itemDeleteAll(parserObjUtilList);
    itemDeleteAll(webViewList);
    delete progressScan;
    delete PFile;
    delete labelVersion;
    delete userMode;
    delete scanSez;
    delete logSez;
    delete vulnSez;
    delete nssAct;
    delete parAct;
    delete actBack;
    delete actForward;
    delete actStop;
    delete bW;
    delete cW;
}

