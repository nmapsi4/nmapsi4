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

#include "../mainwin.h"


QStringList nmapClass::check_extensions(QStringList parametri, QString& winTitle)
{
    if(!winTitle.isEmpty()) {
        progressScan->setValue(10); // start progress bar
        this->setWindowTitle(winTitle.append("(10%)"));
    }
    parametri << "-A";

    switch (comboScan->currentIndex()) { //scan check
    case 0:
//    Connect Scan
        parametri << "-sT";
        break;
    case 1:
//    Ping Sweep
        parametri << "-sP";
        break;
    case 2:
//    Host List
        parametri << "-sL";
        break;
    case 3:
//    FTP Bounce Attack
        if ((bounceEdit->text()).isEmpty()) {
            QMessageBox::warning(this, "NmapSI4", tr("No Ftp Address \n"), tr("Disable Ftp bounce"));
            comboScan->setCurrentIndex(0);
            bounceEdit->setEnabled(false);
            parametri << "-sT";
        } else {
            parametri << "-b";
            parametri << bounceEdit->text();
        }
        break;
    case 4:
//    Idle Scan
        parametri << "-sI";
        parametri << bounceEdit->text();
        break;
    case 5:
//    SYN Stealth Scan (rootMode)
        parametri << "-sS";
        break;
    case 6:
//    ACK Stealth Scan (rootMode)
        parametri << "-sA";
        break;
    case 7:
//    FIN|ACK Stealth Scan (rootMode)
        parametri << "-sM";
        break;
    case 8:
//    FIN Stealth Scan (rootMode)
        parametri << "-sF";
        break;
    case 9:
//    NULL Stealth Scan (rootMode)
        parametri << "-sN";
        break;
    case 10:
//    XMAS Tree Stealth Scan (rootMode)
        parametri << "-sX";
        break;
    case 11:
//    TCP Window Scan (rootMode)
        parametri << "-sW";
        break;
    case 12:
//    UDP Ports Scan (rootMode)
        parametri << "-sU";
        break;
    case 13:
//    IP Protocol Scan (rootMode)
        parametri << "-sO";
        break;
    default:
        comboScan->setCurrentIndex(0);
        parametri << "-sT";
        break;

    }
    if(!winTitle.isEmpty()) {
        progressScan->setValue(15);
        this->setWindowTitle(winTitle.replace("(10%)", "(15%)"));
    }

    // start option scan
    if (rpcBox->isChecked())
        parametri << "-sR";

    if (versionBox->isChecked())
        parametri << "-sV";

    if (notpingBox->isChecked())
        parametri << "-P0";

    if (checkOS->isChecked())
        parametri << "-O";
    //end Extension

    if(!winTitle.isEmpty()) {
        progressScan->setValue(20);
        this->setWindowTitle(winTitle.replace("(15%)", "(20%)"));
    }

    switch (portCombo->currentIndex()) { // port combo check
    case 1:
//    All
        parametri << "-p-";
        break;
    case 2:
//    Most Important
        parametri << "-F";
        break;
    case 3:
//    Range
        if (!portEdit->text().isEmpty()) {
            parametri << "-p";
            parametri << portEdit->text();
        } else
            QMessageBox::warning(this, "NmapSI4", tr("No Ports Range (ex: 20-80)\n"), tr("Close"));
    default:
        break;
    }

    if(!winTitle.isEmpty()) {
        progressScan->setValue(30);
        this->setWindowTitle(winTitle.replace("(20%)", "(30%)"));
    }

    if (checkInputFile->isChecked()) { // File options
        if (!lineInputFile->text().isEmpty()) {
            parametri << "-iL";
            parametri << lineInputFile->text();
        } else
            QMessageBox::warning(this, "NmapSI4", tr("no input file\n"), tr("Normal Scan"));
    }

    if (checkTcpPing->isChecked()) { // Discover options (tcp ack)
        if (!lineTcpPing->text().isEmpty()) {
            QString tmpCommand;
            tmpCommand.append("-PT");
            tmpCommand.append(lineTcpPing->text());
            parametri << tmpCommand;

        } else
            QMessageBox::warning(this, "NmapSI4", "no port (ex: 80)\n", "Normal Scan");
    }

    if (checkTcpSyn->isChecked()) { // Discover options (tcp syn)
        if (!lineSynPing->text().isEmpty()) {
            QString tmpCommand;
            tmpCommand.append("-PS");
            tmpCommand.append(lineSynPing->text());
            parametri << tmpCommand;

        } else
            QMessageBox::warning(this, "NmapSI4", "no port (ex: 80)\n", "Normal Scan");
    }

    if (checkUdpPing->isChecked()) { // Discover options (tcp syn)
        if (!lineUdpPing->text().isEmpty()) {
            QString tmpCommand;
            tmpCommand.append("-PU");
            tmpCommand.append(lineUdpPing->text());
            parametri << tmpCommand;

        } else
            QMessageBox::warning(this, "NmapSI4", "no port (ex: 80)\n", "Normal Scan");
    }

    if (checkIcmpEcho->isChecked()) // Discover option
        parametri << "-PI";
    if (checkIcmpTimestamp->isChecked()) // Discover option
        parametri << "-PP";
    if (checkIcmpNetmask->isChecked()) // Discover option
        parametri << "-PM";

    switch (comboTiming->currentIndex()) { // port combo Timing
    case 1:
//    Paranoid
        parametri << "-T0";
        break;
    case 2:
//    Sneaky
        parametri << "-T1";
        break;
    case 3:
//    Polite
        parametri << "-T2";
        break;
    case 4:
//    Aggressive
        parametri << "-T4";
        break;
    case 5:
//    Insane
        parametri << "-T5";
        break;
    default:
        break;
    }

    switch (comboDNSResolv->currentIndex()) { // port DNS resolv
    case 1:
//    Always
        parametri << "-R";
        break;
    case 2:
//    Never
        parametri << "-n";
        break;
    default:
        break;
    }

    switch (comboVerbosity->currentIndex()) { // port DNS resolv
    case 1:
//    Verbose
        parametri << "-v";
        break;
    case 2:
//    Very Verbose
        parametri << "-vv";
        break;
    case 3:
//    Debug
        parametri << "-d";
        break;
    case 4:
//    Verbose Debug
        parametri << "-d2";
        break;
    default:
        break;
    }

    if (checkOrdered->isChecked()) // Misc Options
        parametri << "-r"; // Ordered Port
    if (checkIpv6->isChecked())
        parametri << "-6"; // Ipv6
    if (checkFrag->isChecked())
        parametri << "-f"; // Ipv6

    if(!winTitle.isEmpty()) {
        progressScan->setValue(50);
        this->setWindowTitle(winTitle.replace("(30%)", "(50%)"));
    }

    // Timing options
    if (TcheckIpv4ttl->isChecked()) {
        parametri << "--ttl";
        parametri << spinBoxIpv4ttl->text();
    }

    if (TcheckMinPar->isChecked()) {
        parametri << "--min_parallelism";
        parametri << TspinBoxMinP->text();
    }

    if (TcheckMaxPar->isChecked()) {
        parametri << "--max_parallelism";
        parametri << spinBoxMaxPar->text();
    }

    if (TcheckInitRtt->isChecked()) {
        parametri << "--initial_rtt_timeout";
        parametri << spinBoxInitRtt->text();
    }

    if (TcheckMinRtt->isChecked()) {
        parametri << "--min_rtt_timeout";
        parametri << spinBoxMinRtt->text();
    }

    if (TcheckMaxRtt->isChecked()) {
        parametri << "--max_rtt_timeout";
        parametri << spinBoxMaxRtt->text();
    }

    if (TcheckHostTime->isChecked()) {
        parametri << "--host_timeout";
        parametri << spinBoxHostTime->text();
    }

    if (TcheckScanDelay->isChecked()) {
        parametri << "--scan_delay";
        parametri << spinBoxScanDelay->text();
    }

    //Options
    if (checkBoxDevice->isChecked()) { // Discover options (tcp syn)
        if (!OlineDevice->text().isEmpty()) {
            parametri << "-e";
            parametri << OlineDevice->text();

        } else {
            QMessageBox::warning(this, "NmapSI4", "Please, first insert a Device\n", "Disable Option");
            checkBoxDevice->setCheckState(Qt::Unchecked);
        }
    }

    if (checkDecoy->isChecked()) { // Discover options (tcp syn)
        if (!lineDecoy->text().isEmpty()) {
            parametri << "-D";
            parametri << lineDecoy->text();

        } else {
            QMessageBox::warning(this, "NmapSI4", "Please, first insert a Decoy\n", "Disable Option");
            checkDecoy->setCheckState(Qt::Unchecked);
        }
    }

    if (checkSpoof->isChecked()) { // Spoof options
        if (!lineEditSpoof->text().isEmpty()) {
            parametri << "-S";
            parametri << lineEditSpoof->text();

        } else {
            QMessageBox::warning(this, "NmapSI4", "Please, insert spoof address\n", "Disable Option");
            checkSpoof->setCheckState(Qt::Unchecked);
        }
    }

    if (checkSourcePort->isChecked()) { // Spoof options
        if (!lineSourcePort->text().isEmpty()) {
            parametri << "-g";
            parametri << lineSourcePort->text();

        } else {
            QMessageBox::warning(this, "NmapSI4", "Please, insert port address\n", "Disable Option");
            checkSourcePort->setCheckState(Qt::Unchecked);
        }
    }

    if(!winTitle.isEmpty()) {
        progressScan->setValue(55);
        this->setWindowTitle(winTitle.replace("(50%)", "(55%)"));
    }


    return parametri;
}
