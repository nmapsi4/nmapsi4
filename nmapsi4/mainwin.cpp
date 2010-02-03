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
       labelVersion(NULL),
       userMode(NULL),
       th(NULL)
{
    scanSez_ = new QAction(this);
    logSez_ = new QAction(this);
    vulnSez_ = new QAction(this);
    nssAct_ = new QAction(this);

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
    // Section QAction
    scanSez_->setIcon(QIcon(QString::fromUtf8(":/images/images/network_local.png")));
    scanSez_->setIconText(tr("Scan"));
    scanSez_->setToolTip(tr("Scan host(s)"));

    logSez_->setIcon(QIcon(QString::fromUtf8(":/images/images/book.png")));
    logSez_->setIconText(tr("Log"));
    logSez_->setToolTip(tr("Scan Log"));

    vulnSez_->setIcon(QIcon(QString::fromUtf8(":/images/images/viewmag+.png")));
    vulnSez_->setIconText(tr("Services"));
    vulnSez_->setToolTip(tr("Check Vulnerabilities"));

    nssAct_->setIcon(QIcon(QString::fromUtf8(":/images/images/network_local.png")));
    nssAct_->setIconText(tr("Nss Script"));
    nssAct_->setToolTip(tr("Enable/Disable NSS script"));
    nssAct_->setCheckable(true);

    sezBar->addAction(scanSez_);
    sezBar->addAction(logSez_);
    sezBar->addAction(vulnSez_);
    sezBar->addSeparator();
    sezBar->addAction(nssAct_);

    setNmapsiSlot();
}

void nmapClass::initObject() {

    int uid = 0;

#ifndef Q_WS_WIN
    uid = getuid();
#endif

#ifdef Q_WS_WIN
    tabWidget->removeTab(3);
#endif

    // Disable scan action (nmap check)
    action_Scan_menu->setEnabled(false);
    action_Scan_2->setEnabled(false);
    hostEdit->setEnabled(false);
    actionAdd_Bookmark->setEnabled(false);
    action_Add_BookmarkToolBar->setEnabled(false);

    checkProfile();
    optionListCreate();

    digC  = new digSupport();
    digC->checkDigSupport();

    checkNmapVersion();
    nssAct_->setChecked(NSSsupport_); // set NSS support
    listWscan->setColumnWidth(0, 400);
    treeLogH->setColumnWidth(0, 400);
    scanMonitor->setColumnWidth(0, 400);
    treeTraceroot->setColumnWidth(0, 400);
    treeTraceroot->setColumnWidth(1, 100);
    treeTraceroot->setColumnWidth(2, 200);
    treeTraceroot->setColumnWidth(3, 200);
    treeBookVuln->setColumnWidth(0, 400);
    QSettings settings("nmapsi4", "nmapsi4");
    QPoint pos = settings.value("window/pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("window/size", QSize(869, 605)).toSize();
    resize(size);
    move(pos);

    logHistory *historyScan_ = new logHistory(treeLogH, "nmapsi4/urlList", "nmapsi4/urlListTime", hostCache);
    historyScan_->updateBookMarks();
    delete historyScan_;
    logHistory *historyVuln_ = new logHistory(treeBookVuln, "nmapsi4/urlListVuln", "nmapsi4/urlListTimeVuln", hostCache);
    historyVuln_->updateBookMarks();
    delete historyVuln_;
    this->rootMode(uid); // send uid value
    dialog = new mainProfile();
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

    if(lookupInternal_) {
        addMonitorHost(scanMonitor, hostname);
        lth = new lookUpT(hostname,this);
        connect(lth, SIGNAL(threadEnd(QHostInfo,int,const QString)),
                       this, SLOT(scanLookup(QHostInfo,int,const QString)));

        lth->start();
    } else if(lookupDig_ && digC->getDigSupport()) {
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
    QString title;

    history = new logHistory("nmapsi4/cacheHost", hostCache);
    history->addItemHistory(hostname);

    title.append("NmapSI4 ");
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
    
    QByteArray buff1, buff2;
    th = new scanThread(buff1, buff2, parametri, this);

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
    qDeleteAll(parserObjList_);
    
    delete progressScan;
    delete PFile;
    delete labelVersion;
    delete userMode;
    delete digC;
    delete scanSez_;
    delete logSez_;
    delete vulnSez_;
    delete nssAct_;
}

