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

#include "parser.h"
#include "mainwin.h"

void parser::startParser(const QStringList parList, QByteArray dataBuffer, QByteArray errorBuffer)
{
    // check nmap error
    if(!dataBuffer.size() && errorBuffer.size())
    {
        QMessageBox::critical(this, "NmapSI4", tr("Error: check nmap Installation.\n"), tr("Close"));
        return;
    }

    _ui->listClearFlag = false; // the listScan is not empty

    _ui->progressScan->setValue(75);
    
    QString StdoutStr(dataBuffer);
    QString StderrorStr(errorBuffer);
    
    // create a scan host item.
    QTreeWidgetItem *mainTreeE = new QTreeWidgetItem(_ui->treeMain);
    _ui->mainTreeElem.push_front(mainTreeE);
    mainTreeE->setSizeHint(0, QSize(32, 32));
    
    _ui->comboScanLog->insertItem(3, parList.join(" "));
    
    // call real parser
    parserObj* elemObj = parserCore(parList,StdoutStr,StderrorStr,mainTreeE);

    int tmpBox = _ui->SWscan->currentIndex();
 
    switch(tmpBox) 
    {
      case 0:
        _ui->Bnss->setIcon(QIcon(QString::fromUtf8(":/images/images/reload.png")));
        break;
      case 1:
        _ui->Bdetails->setIcon(QIcon(QString::fromUtf8(":/images/images/reload.png")));
        break;
      case 2:
        _ui->Bdetails->setIcon(QIcon(QString::fromUtf8(":/images/images/reload.png")));
        _ui->Bnss->setIcon(QIcon(QString::fromUtf8(":/images/images/reload.png")));
        break;
    }

    // clear output and error thread buffer.
    dataBuffer.clear();
    errorBuffer.clear();

    _ui->progressScan->setValue(85);

    if(!_ui->_monitor->monitorHostNumber()) 
    {
        _ui->progressScan->setValue(100);
        _ui->monitorStopAllScanButt->setEnabled(false);
        _ui->monitorStopCurrentScanButt->setEnabled(false);
        _ui->monitorDetailsScanButt->setEnabled(false);
        _ui->tabUi->setTabIcon(_ui->tabUi->indexOf(_ui->tabMainMonitor),
                          QIcon(QString::fromUtf8(":/images/images/utilities-system-monitor.png")));
    } 
    else 
    {
        _ui->progressScan->setValue(55);
    }

    _ui->action_Scan_menu->setEnabled(true);
    _ui->action_Scan_2->setEnabled(true);
    _ui->hostEdit->setEnabled(true);
    _ui->action_Save_As->setEnabled(true);
    _ui->actionSave_As_Menu->setEnabled(true);
    
    if (!_ui->logSessionFile.isEmpty()) 
    {
        _ui->actionSave->setEnabled(true);
        _ui->actionSave_Menu->setEnabled(true);
    }
    
    if(!_ui->_monitor->monitorHostNumber()) 
    {
        _ui->_monitor->clearHostMonitor();
        freelist<lookUpT*>::itemDeleteAll(_ui->internealLookupList);
        freelist<digSupport*>::itemDeleteAll(_ui->digLookupList);
    }

    _parserObjList.append(elemObj);
}

void parser::showParserResult(QTreeWidgetItem *item, int column) 
{ // SLOT
    Q_UNUSED(column);

    QString hostName_ = item->text(0);
    hostName_  = hostName_.left(hostName_.indexOf("\n"));

    if(_ui->hostEdit->itemText(0).isEmpty() && item->parent() == NULL) 
    {
        _ui->hostEdit->addItem(hostName_);
    } 
    else if(item->parent() == NULL) 
    {
        _ui->hostEdit->setItemText(0, hostName_);
    }

    int indexObj = _ui->treeMain->indexOfTopLevelItem(item);

    if(indexObj != -1) 
    {
        showParserObj(indexObj);
	showParserObjPlugins(indexObj);
    }
}

void parser::showParserTracerouteResult(QTreeWidgetItem *item, int column) 
{ // SLOT
    Q_UNUSED(column);

    if(_ui->hostEdit->itemText(0).isEmpty() && !item->parent() && !item->text(2).isEmpty()) 
    {
        if(!item->text(3).contains("DNS")) 
        {
            _ui->hostEdit->addItem(item->text(3));
        } 
        else 
        {
            _ui->hostEdit->addItem(item->text(2));
        }
    } 
    else if(!item->parent() && !item->text(2).isEmpty()) 
    {
        if(!item->text(3).contains("DNS")) 
        {
            _ui->hostEdit->setItemText(0, item->text(3));
        } 
        else 
        {
            _ui->hostEdit->setItemText(0, item->text(2));
        }
    }
}
