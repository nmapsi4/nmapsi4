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

void nmapClass::startScan()
{
    if (hostEdit->currentText().isEmpty() && lineInputFile->text().isEmpty()) 
    {
        QMessageBox::warning(this, "NmapSI4", tr("No Host Target\n"), tr("Close"));
        return;
    }
    
    QString hostname = hostEdit->currentText();
    // check wrong address
    hostname = clearHost(hostname);
    
    // check for duplicate hostname in the monitor 
    if (searchMonitorElem(hostname))
    {
        QMessageBox::warning(this, "NmapSI4", tr("Hostname already scanning\n"), tr("Close"));
        return;
    }
    
    if(!monitorElemHost.size()) 
    {
        // clear details QHash
        scanHashListFlow.clear();
    }


    // check for ip list
    if(hostname.contains("/") && !hostname.contains("//")) 
    {
        // is a ip list
        QStringList addrPart_ = hostname.split('/');
        QStringList ipBase_ = addrPart_[0].split('.');
        int ipLeft_ = ipBase_[3].toInt();
        int ipRight_ = addrPart_[1].toInt();
        // TODO limit parallel ip scan
        for(int index = ipLeft_; index <= ipRight_; index++) 
        {
            ipBase_[3].setNum(index);
            hostname = ipBase_.join(".");
            preScanLookup(hostname);
        }
        return;
    }

    //scan token DNS/IP parser
    if(hostname.contains(" ")) 
    { // space delimiter
        QStringList addrPart_ = hostname.split(' ');
        addrPart_.removeAll("");
        // check for only one space in hostname
        if(addrPart_.size() > 1) 
        {
            // multiple ip or dns to scan
            for(int index=0; index < addrPart_.size(); index++) 
            {
                addrPart_[index] = clearHost(addrPart_[index]);
                // check for lookup support
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

void nmapClass::preScanLookup(const QString hostname)
{
    // log check
    if (checkLog) 
    { // create a file log
        fileSession();
    } 
    else 
    {
        if (!logPath.contains(QDir::tempPath())) 
        {
            QSettings ptr("nmapsi4", "nmapsi4");
            ptr.setValue("confPath", QDir::tempPath());
            logPath = QDir::tempPath();
            checkProfile();
        }

        fileSession();
    }

    // save ip or dns to history
    logHistory *history = new logHistory("nmapsi4/cacheHost", hostCache);
    history->addItemHistory(hostname);
    delete history;

    // default action
    monitorStopAllScanButt->setEnabled(true);
    action_Save_As->setEnabled(false);
    actionSave_As_Menu->setEnabled(false);
    actionSave->setEnabled(false);
    actionSave_Menu->setEnabled(false);

    // check for scan lookup
    if(lookupInternal) 
    {
        // if internal lookUp is actived
        addMonitorHost(scanMonitor, hostname);
        // call internal lookup thread and save the pointer.
        lookUpT *internalLookupTh_ = new lookUpT(hostname,this);
        internealLookupList.push_back(internalLookupTh_);

        connect(internalLookupTh_, SIGNAL(threadEnd(QHostInfo,int,const QString)),
                this, SLOT(scanLookup(QHostInfo,int,const QString)));

        internalLookupTh_->start();
    } 
    else if(lookupDig && digSupported) 
    {
        // if dig support is actived
        parserObjUtil* tmpParserObj_ = new parserObjUtil();
        digSupport *digC = new digSupport();
        digLookupList.push_back(digC);
        digC->digProcess(hostname,tmpParserObj_);
        parserObjUtilList.append(tmpParserObj_);
        addMonitorHost(scanMonitor, hostname);
        scan(hostname);
    } 
    else 
    {
        // lookup isn't actived or not supported
        addMonitorHost(scanMonitor, hostname);
        this->scan(hostname);
    }
}

void nmapClass::scan(const QString hostname)
{

    QStringList parameters_; //parameters list declaration

    if(!frameAdv->isVisible()) 
    {
        parameters_ = check_extensions(); // extensions.cpp
    } 
    else 
    {
        parameters_ = comboAdv->lineEdit()->text().split(' ');
    }

    parameters_.append(hostname); // add hostname

    QByteArray buff1;
    QByteArray buff2;
    // start scan Thread
    QPointer<scanThread> thread = new scanThread(buff1, buff2, parameters_, this);
    //scanPointerList.push_front(th);
    scanHashList.insert(hostname,thread);
    // update progressbar for scan
    connect(thread, SIGNAL(upgradePR()),
            this, SLOT(setProgress()));
    // read current data scan from the thread
    connect(thread, SIGNAL(flowFromThread(const QString, const QString)),
            this, SLOT(readFlowFromThread(const QString, const QString)));
    // read scan data return
    connect(thread, SIGNAL(threadEnd(const QStringList, QByteArray, QByteArray)),
            this, SLOT(nmapParser(const QStringList, QByteArray, QByteArray))); // nmapParser.cpp
    // start scan
    thread->start();
}
