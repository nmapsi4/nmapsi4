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

void nmapClass::nmapParser(const QStringList parList, QByteArray dataBuffer, QByteArray errorBuffer)
{
    // check nmap error
    if(!dataBuffer.size() && errorBuffer.size())
    {
        QMessageBox::critical(this, "NmapSI4", tr("Error: check nmap Installation.\n"), tr("Close"));
        return;
    }

    listClearFlag = false; // the listScan is not empty

    progressScan->setValue(75);
    
    QString StdoutStr(dataBuffer);
    QString StderrorStr(errorBuffer);
    
    // remove host from monitor
    _monitor->delMonitorHost(parList[parList.size()-1]);
    
    // create a scan host item.
    QTreeWidgetItem *mainTreeE = new QTreeWidgetItem(treeMain);
    mainTreeElem.push_front(mainTreeE);
    mainTreeE->setSizeHint(0, QSize(32, 32));
    
    comboScanLog->insertItem(3, parList.join(" "));
    
    // call real parser
    parserObj* elemObj = parser(parList,StdoutStr,StderrorStr,mainTreeE);

    int tmpBox = SWscan->currentIndex();
 
    switch(tmpBox) 
    {
      case 0:
        Bnss->setIcon(QIcon(QString::fromUtf8(":/images/images/reload.png")));
        break;
      case 1:
        Bdetails->setIcon(QIcon(QString::fromUtf8(":/images/images/reload.png")));
        break;
      case 2:
        Bdetails->setIcon(QIcon(QString::fromUtf8(":/images/images/reload.png")));
        Bnss->setIcon(QIcon(QString::fromUtf8(":/images/images/reload.png")));
        break;
    }

    // clear ouput and error thread buffer.
    dataBuffer.clear();
    errorBuffer.clear();

    progressScan->setValue(85);
    if(!_monitor->monitorHostNumber()) 
    {
        progressScan->setValue(100);
        monitorStopAllScanButt->setEnabled(false);
        monitorStopCurrentScanButt->setEnabled(false);
        monitorDetailsScanButt->setEnabled(false);
        tabUi->setTabIcon(tabUi->indexOf(tabMainMonitor),
                          QIcon(QString::fromUtf8(":/images/images/utilities-system-monitor.png")));
    } 
    else 
    {
        progressScan->setValue(55);
    }

    action_Scan_menu->setEnabled(true);
    action_Scan_2->setEnabled(true);
    hostEdit->setEnabled(true);
    action_Save_As->setEnabled(true);
    actionSave_As_Menu->setEnabled(true);
    
    if (!logSessionFile.isEmpty()) 
    {
        actionSave->setEnabled(true);
        actionSave_Menu->setEnabled(true);
    }
    
    if(!_monitor->monitorHostNumber()) 
    {
        _monitor->clearHostMonitor();
        freelist<lookUpT*>::itemDeleteAll(internealLookupList);
        freelist<digSupport*>::itemDeleteAll(digLookupList);
    }

    parserObjList.append(elemObj);
}

void nmapClass::runtimePars(QTreeWidgetItem *item, int column) 
{ // SLOT
    Q_UNUSED(column);

    QString hostName_ = item->text(0);
    hostName_  = hostName_.left(hostName_.indexOf("\n"));

    if(hostEdit->itemText(0).isEmpty() && item->parent() == NULL) 
    {
        hostEdit->addItem(hostName_);
    } 
    else if(item->parent() == NULL) 
    {
        hostEdit->setItemText(0, hostName_);
    }

    int indexObj = treeMain->indexOfTopLevelItem(item);

    if(indexObj != -1) 
    {
        showParserObj(indexObj);
    }
}

void nmapClass::runtimeTraceroutePars(QTreeWidgetItem *item, int column) 
{ // SLOT
    Q_UNUSED(column);

    if(hostEdit->itemText(0).isEmpty() && !item->parent() && !item->text(2).isEmpty()) 
    {
        if(!item->text(3).contains("DNS")) 
        {
            hostEdit->addItem(item->text(3));
        } 
        else 
        {
            hostEdit->addItem(item->text(2));
        }
    } 
    else if(!item->parent() && !item->text(2).isEmpty()) 
    {
        if(!item->text(3).contains("DNS")) 
        {
            hostEdit->setItemText(0, item->text(3));
        } 
        else 
        {
            hostEdit->setItemText(0, item->text(2));
        }
    }
}
