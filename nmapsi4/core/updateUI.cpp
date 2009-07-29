/***************************************************************************
 *   Copyright (C) 2007-2009 by Francesco Cecconi                          *
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


void nmapClass::update_portCombo() {
    switch (portCombo->currentIndex()) {
    case 0:
//    Normal
        portEdit->setEnabled(false);
        break;
    case 1:
//    All
        portEdit->setEnabled(false);
        break;
    case 2:
//    Most Important
        portEdit->setEnabled(false);
        break;
    case 3:
//    Range
        portEdit->setEnabled(true);
        break;
    default:
        portEdit->setEnabled(false);
        break;
    }

}


void nmapClass::update_scanCombo() {

    switch (comboScan->currentIndex()) {
    case 0:
//    Connect Scan
        bounceEdit->setEnabled(false);
        break;
    case 1:
//    Ping Sweep
        bounceEdit->setEnabled(false);
        break;
    case 2:
//    Host List
        bounceEdit->setEnabled(false);
        break;
    case 3:
//    FTP Bounce Attack
        bounceEdit->setEnabled(true);
        break;
    case 4:
//    Idle Scan
        bounceEdit->setEnabled(true);
        break;
    case 5:
//    SYN Stealth Scan
        bounceEdit->setEnabled(false);
        break;
    case 6:
//    ACK Stealth Scan
        bounceEdit->setEnabled(false);
        break;
    case 7:
//    FIN|ACK Stealth Scan
        bounceEdit->setEnabled(false);
        break;
    case 8:
//    FIN Stealth Scan
        bounceEdit->setEnabled(false);
        break;
    case 9:
//    NULL Stealth Scan
        bounceEdit->setEnabled(false);
        break;
    case 10:
//    XMAS Tree Stealth Scan
        bounceEdit->setEnabled(false);
        break;
    case 11:
//    TCP Window Scan
        bounceEdit->setEnabled(false);
        break;
    case 12:
//    UDP Ports Scan
        bounceEdit->setEnabled(false);
        break;
    case 13:
//    IP Protocol Scan
        bounceEdit->setEnabled(false);
        break;
    default:
        bounceEdit->setEnabled(false);
        break;

    }
}

void nmapClass::rootMode(int uid) {

    if (!uid) { // if root
        comboScan->addItem(QApplication::translate("nmapClass", "Idle Scan", 0, QApplication::UnicodeUTF8));
        comboScan->addItem(QApplication::translate("nmapClass", "SYN Stealth Scan", 0, QApplication::UnicodeUTF8));
        comboScan->addItem(QApplication::translate("nmapClass", "ACK Stealth Scan", 0, QApplication::UnicodeUTF8));
        comboScan->addItem(QApplication::translate("nmapClass", "FIN|ACK Stealth Scan", 0, QApplication::UnicodeUTF8));
        comboScan->addItem(QApplication::translate("nmapClass", "FIN Stealth Scan", 0, QApplication::UnicodeUTF8));
        comboScan->addItem(QApplication::translate("nmapClass", "NULL Stealth Scan", 0, QApplication::UnicodeUTF8));
        comboScan->addItem(QApplication::translate("nmapClass", "XMAS Tree Stealth Scan", 0, QApplication::UnicodeUTF8));
        comboScan->addItem(QApplication::translate("nmapClass", "TCP Window Scan", 0, QApplication::UnicodeUTF8));
        comboScan->addItem(QApplication::translate("nmapClass", "UDP Ports Scan", 0, QApplication::UnicodeUTF8));
        comboScan->addItem(QApplication::translate("nmapClass", "IP Protocol Scan", 0, QApplication::UnicodeUTF8));

        comboScan->setCurrentIndex(5);

    } else {
        checkIcmpEcho->setVisible(false);
        checkIcmpTimestamp->setVisible(false);
        checkIcmpNetmask->setVisible(false);
        checkOS->setVisible(false);

        // discover Udp Ping
        checkUdpPing->setVisible(false);
        lineUdpPing->setVisible(false);
        label_7->setVisible(false);

        // Misc Option
        checkFrag->setVisible(false);
        checkDecoy->setVisible(false);
        lineDecoy->setVisible(false);
        checkSourcePort->setVisible(false);
        lineSourcePort->setVisible(false);
    }

}

void nmapClass::update_inputcheck() {
    if (checkInputFile->isChecked()) {
        lineInputFile->setEnabled(true);
        buttonBrowser->setEnabled(true);
        lineInputFile->clear();
        hostEdit->clear();
    } else {
        lineInputFile->setEnabled(false);
        buttonBrowser->setEnabled(false);
        lineInputFile->clear();

        hostEdit->setStyleSheet(QString::fromUtf8("color: rgb(153, 153, 153);"));
        hostEdit->setItemText(0, tr("Insert HostName to scan"));
    }
}

void nmapClass::update_discover() {
    if (checkTcpPing->isChecked()) {
        lineTcpPing->setEnabled(true);
        lineTcpPing->clear();
    } else {
        lineTcpPing->setEnabled(false);
        lineTcpPing->clear();
    }

    if (checkTcpSyn->isChecked()) {
        lineSynPing->setEnabled(true);
        lineSynPing->clear();
    } else {
        lineSynPing->setEnabled(false);
        lineSynPing->clear();
    }

    if (checkUdpPing->isChecked()) {
        lineUdpPing->setEnabled(true);
        lineUdpPing->clear();
    } else {
        lineUdpPing->setEnabled(false);
        lineUdpPing->clear();
    }
}

void nmapClass::update_timing() {
    if (TcheckIpv4ttl->isChecked())
        spinBoxIpv4ttl->setEnabled(true);
    else
        spinBoxIpv4ttl->setEnabled(false);

    if (TcheckMinPar->isChecked())
        TspinBoxMinP->setEnabled(true);
    else
        TspinBoxMinP->setEnabled(false);

    if (TcheckMaxPar->isChecked())
        spinBoxMaxPar->setEnabled(true);
    else
        spinBoxMaxPar->setEnabled(false);

    if (TcheckInitRtt->isChecked())
        spinBoxInitRtt->setEnabled(true);
    else
        spinBoxInitRtt->setEnabled(false);

    if (TcheckMinRtt->isChecked())
        spinBoxMinRtt->setEnabled(true);
    else
        spinBoxMinRtt->setEnabled(false);

    if (TcheckMaxRtt->isChecked())
        spinBoxMaxRtt->setEnabled(true);
    else
        spinBoxMaxRtt->setEnabled(false);

    if (TcheckHostTime->isChecked())
        spinBoxHostTime->setEnabled(true);
    else
        spinBoxHostTime->setEnabled(false);

    if (TcheckScanDelay->isChecked())
        spinBoxScanDelay->setEnabled(true);
    else
        spinBoxScanDelay->setEnabled(false);
}

void nmapClass::update_options() {
    if (checkBoxDevice->isChecked())
        OlineDevice->setEnabled(true);
    else {
        OlineDevice->setEnabled(false);
        OlineDevice->clear();
    }

    if (checkDecoy->isChecked())
        lineDecoy->setEnabled(true);
    else {
        lineDecoy->setEnabled(false);
        lineDecoy->clear();
    }

    if (checkSpoof->isChecked()) {
        lineEditSpoof->setEnabled(true);
        comboScan->setCurrentIndex(0);
        notpingBox->setChecked(true);
        checkBoxDevice->setChecked(true);
    } else {
        lineEditSpoof->setEnabled(false);
        lineEditSpoof->clear();
        checkBoxDevice->setCheckState(Qt::Unchecked);
        notpingBox->setChecked(false);
    }

    if (checkSourcePort->isChecked()) {
        lineSourcePort->setEnabled(true);
        comboScan->setCurrentIndex(5);
    } else {
        lineSourcePort->setEnabled(false);
        lineSourcePort->clear();
    }
}

void nmapClass::update_comboVerbosity() {

    if (comboVerbosity->currentIndex() == 4)
        QMessageBox::warning(this, "NmapSI4", tr("Warning: Operation more expansive.\n"), tr("Close"));

}


void nmapClass::listClear() {
    listClearFlag = true;
    itemDeleteAll(itemList); //clear items list
    itemList.clear();

    listWscan->clear();
    treeWinfo->clear();
    listScanError->clear();
    actionClear_History->setEnabled(false);
    action_Save_As->setEnabled(false);
}

void nmapClass::closeTree() {
    listWscan->collapseAll();
    treeWinfo->collapseAll();
    listScanError->collapseAll();
    listScan->collapseAll();
}

void nmapClass::updateIconsBox() {
    int tmpBox = toolBox->currentIndex();
    switch (tmpBox) {
      case 0:
	toolBox->setItemIcon(tmpBox, QIcon(QString::fromUtf8(":/images/images/network_local.png")));
	break;
      case 1:
	toolBox->setItemIcon(tmpBox, QIcon(QString::fromUtf8(":/images/images/viewmag+.png")));
	break;
      case 2:
	toolBox->setItemIcon(tmpBox, QIcon(QString::fromUtf8(":/images/images/bookmark_folder.png")));
	break;
    }

}

void nmapClass::updateFontHost(const QString hostName) {
    Q_UNUSED(hostName);
    action_Scan_menu->setEnabled(true);
    action_Scan_2->setEnabled(true);
    hostEdit->clear();
    hostEdit->setStyleSheet(QString::fromUtf8(""));
    hostEdit->disconnect(SIGNAL(editTextChanged(QString)));
    connect(hostEdit, SIGNAL(editTextChanged(QString)),
            this, SLOT(callSearchHistory()));
}

void nmapClass::activeBookTool() {
    deleteBook->setEnabled(true);
    deleteBookVuln->setEnabled(true);
}

void nmapClass::setProgress() {
    //this->setWindowIcon(QIcon(QString::fromUtf8(":/images/icons/nmapsi4_scan.svg")));
    progressScan->setValue(60);
    this->setWindowTitle("Nmapsi4 (60%)");
}

void nmapClass::updateMainSlide() {
     
     switch(comboMain->currentIndex()) {
     case 0:
	  // scan 
	  stackedMain->setCurrentIndex(0);
	  this->showMainToolBar();
	  this->showActionToolBar();
	  break;
     case 1:
	  // log
	  stackedMain->setCurrentIndex(1);
	  // main and action bar only in scan index
	  toolBar->setVisible(false);
	  actionMain_Toolbars->setChecked(false);
	  toolBar_2->setVisible(false);
          toolBarBook->setVisible(false);
	  actionActions_Toolbar->setChecked(false);
	  break;
     case 2:
	  // Vuln
	  stackedMain->setCurrentIndex(2);
	  // main and action bar only in scan index
	  toolBar->setVisible(false);
          toolBarBook->setVisible(true);
	  actionMain_Toolbars->setChecked(false);
	  toolBar_2->setVisible(false);
	  actionActions_Toolbar->setChecked(false);
	  break;
     }
}
