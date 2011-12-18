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

QStringList nmapClass::loadExtensions()
{
    return comboAdv->lineEdit()->text().split(' ');
}

QStringList nmapClass::check_extensions()
{
    QStringList parameters;

    if(_monitor->monitorHostNumber())
    {
        progressScan->setValue(10); // start progress bar
    }

    if(NSSsupport)
    {
        parameters << "-A";
    }
    else
    {
        if (!uid)
        {
            parameters << "--traceroute";
        }
    }

    if (nseComboScript->currentIndex())
    {
        //set --script "xx,xx"
        if (parameters.contains("-A"))
        {
            parameters.removeAll("-A");
            if (!uid)
            {
                parameters << "--traceroute";
            }
        }
        QString tmpListScript_("--script=");
        QString tmpListParam_("--script-args=");
        QString tmpList_;
        QString tmpListArgs_;

        if (_nseManager->getActiveNseScript().size())
        {
            foreach (const QString &token, _nseManager->getActiveNseScript())
            {
                tmpList_.append(token);
                tmpList_.append(",");
            }
        }

        // load nse manual script
        if (!comboNseInv->lineEdit()->text().isEmpty())
        {
            QStringList manualNse_ = comboNseInv->lineEdit()->text().split(',');
            foreach (const QString &token, manualNse_)
            {
                tmpList_.append(token);
                tmpList_.append(",");
            }
        }

        if (tmpList_.size())
        {
            tmpList_.remove(' ');
            tmpList_.resize(tmpList_.size()-1);
            tmpListScript_.append(tmpList_);
        }
        else
        {
            tmpListScript_.clear();
            tmpListScript_.append("-sC");
        }

        if (!comboNsePar->lineEdit()->text().isEmpty())
        {
            QString argsClean_ = comboNsePar->lineEdit()->text().remove('"');
            argsClean_ = argsClean_.remove('\'');
            QStringList argsNse_ = argsClean_.split(',');
            foreach (const QString &token, argsNse_)
            {
                tmpListArgs_.append(token);
                tmpListArgs_.append(",");
            }
        }

        if (tmpListArgs_.size())
        {
            tmpListArgs_.remove(' ');
            tmpListArgs_.resize(tmpListArgs_.size()-1);
            tmpListParam_.append(tmpListArgs_);
            parameters << tmpListParam_;
        }

        parameters << tmpListScript_;
    }

    switch (comboScan->currentIndex()) { //scan check
    case 0:
//    Connect Scan
        parameters << "-sT";
        break;
    case 1:
//    Ping Sweep
        parameters << "-sP";
        break;
    case 2:
//    Host List
        parameters << "-sL";
        break;
    case 3:
//    FTP Bounce Attack
        if ((bounceEdit->text()).isEmpty())
        {
            QMessageBox::warning(this, "NmapSI4", tr("No Ftp Address \n"), tr("Disable Ftp bounce"));
            comboScan->setCurrentIndex(0);
            bounceEdit->setEnabled(false);
            parameters << "-sT";
        }
        else
        {
            parameters << "-b";
            parameters << bounceEdit->text();
        }
        break;
    case 4:
//    Idle Scan
        parameters << "-sI";
        parameters << bounceEdit->text();
        break;
    case 5:
//    SYN Stealth Scan (rootMode)
        parameters << "-sS";
        break;
    case 6:
//    ACK Stealth Scan (rootMode)
        parameters << "-sA";
        break;
    case 7:
//    FIN|ACK Stealth Scan (rootMode)
        parameters << "-sM";
        break;
    case 8:
//    FIN Stealth Scan (rootMode)
        parameters << "-sF";
        break;
    case 9:
//    NULL Stealth Scan (rootMode)
        parameters << "-sN";
        break;
    case 10:
//    XMAS Tree Stealth Scan (rootMode)
        parameters << "-sX";
        break;
    case 11:
//    TCP Window Scan (rootMode)
        parameters << "-sW";
        break;
    case 12:
//    UDP Ports Scan (rootMode)
        parameters << "-sU";
        break;
    case 13:
//    IP Protocol Scan (rootMode)
        parameters << "-sO";
        break;
    default:
        comboScan->setCurrentIndex(0);
        parameters << "-sT";
        break;

    }

    if(_monitor->monitorHostNumber())
    {
        progressScan->setValue(15);
    }


    // start option scan
    if (rpcBox->isChecked())
        parameters << "-sR";

    if (versionBox->isChecked())
        parameters << "-sV";

    if (notpingBox->isChecked())
        parameters << "-P0";

    if (checkOS->isChecked())
        parameters << "-O";
    //end Extension

    if(_monitor->monitorHostNumber())
    {
        progressScan->setValue(20);
    }

    switch (portCombo->currentIndex())
    { // port combo check
    case 1:
//    All
        parameters << "-p-";
        break;
    case 2:
//    Most Important
        parameters << "-F";
        break;
    case 3:
//    Range
        if (!portEdit->text().isEmpty())
        {
            parameters << "-p";
            parameters << portEdit->text();
        }
        else
        {
            QMessageBox::warning(this, "NmapSI4", tr("No Ports Range (ex: 20-80)\n"), tr("Close"));
        }
    default:
        break;
    }

    if(_monitor->monitorHostNumber())
    {
        progressScan->setValue(30);
    }

    if (checkInputFile->isChecked())
    { // File options
        if (!lineInputFile->text().isEmpty())
        {
            parameters << "-iL";
            parameters << lineInputFile->text();
        }
        else
        {
            QMessageBox::warning(this, "NmapSI4", tr("no input file\n"), tr("Normal Scan"));
        }
    }

    if (checkTcpPing->isChecked())
    { // Discover options (tcp ack)
        if (!lineTcpPing->text().isEmpty())
        {
            QString tmpCommand;
            tmpCommand.append("-PT");
            tmpCommand.append(lineTcpPing->text());
            parameters << tmpCommand;

        }
        else
        {
            QMessageBox::warning(this, "NmapSI4", "no port (ex: 80)\n", "Normal Scan");
        }
    }

    if (checkTcpSyn->isChecked())
    { // Discover options (tcp syn)
        if (!lineSynPing->text().isEmpty())
        {
            QString tmpCommand;
            tmpCommand.append("-PS");
            tmpCommand.append(lineSynPing->text());
            parameters << tmpCommand;

        }
        else
        {
            QMessageBox::warning(this, "NmapSI4", "no port (ex: 80)\n", "Normal Scan");
        }
    }

    if (checkUdpPing->isChecked())
    { // Discover options (tcp syn)
        if (!lineUdpPing->text().isEmpty())
        {
            QString tmpCommand;
            tmpCommand.append("-PU");
            tmpCommand.append(lineUdpPing->text());
            parameters << tmpCommand;

        }
        else
        {
            QMessageBox::warning(this, "NmapSI4", "no port (ex: 80)\n", "Normal Scan");
        }
    }

    if (checkIcmpEcho->isChecked()) // Discover option
        parameters << "-PI";
    if (checkIcmpTimestamp->isChecked()) // Discover option
        parameters << "-PP";
    if (checkIcmpNetmask->isChecked()) // Discover option
        parameters << "-PM";

    switch (comboTiming->currentIndex())
    { // port combo Timing
    case 1:
//    Paranoid
        parameters << "-T0";
        break;
    case 2:
//    Sneaky
        parameters << "-T1";
        break;
    case 3:
//    Polite
        parameters << "-T2";
        break;
    case 4:
//    Aggressive
        parameters << "-T4";
        break;
    case 5:
//    Insane
        parameters << "-T5";
        break;
    default:
        break;
    }

    switch (comboDNSResolv->currentIndex())
    { // port DNS resolv
    case 1:
//    Always
        parameters << "-R";
        break;
    case 2:
//    Never
        parameters << "-n";
        break;
    default:
        break;
    }

    switch (comboVerbosity->currentIndex())
    { // port DNS resolv
    case 1:
//    Verbose
        parameters << "-v";
        break;
    case 2:
//    Very Verbose
        parameters << "-vv";
        break;
    case 3:
//    Debug
        parameters << "-d";
        break;
    case 4:
//    Verbose Debug
        parameters << "-d2";
        break;
    default:
        break;
    }

    if (checkOrdered->isChecked()) // Misc Options
        parameters << "-r"; // Ordered Port
    if (checkIpv6->isChecked())
        parameters << "-6"; // Ipv6
    if (checkFrag->isChecked())
        parameters << "-f"; // Ipv6

    if(_monitor->monitorHostNumber())
    {
        progressScan->setValue(50);
    }

    // Timing options
    if (TcheckIpv4ttl->isChecked())
    {
        parameters << "--ttl";
        parameters << spinBoxIpv4ttl->text();
    }

    if (TcheckMinPar->isChecked())
    {
        parameters << "--min_parallelism";
        parameters << TspinBoxMinP->text();
    }

    if (TcheckMaxPar->isChecked())
    {
        parameters << "--max_parallelism";
        parameters << spinBoxMaxPar->text();
    }

    if (TcheckInitRtt->isChecked())
    {
        parameters << "--initial_rtt_timeout";
        parameters << spinBoxInitRtt->text();
    }

    if (TcheckMinRtt->isChecked())
    {
        parameters << "--min_rtt_timeout";
        parameters << spinBoxMinRtt->text();
    }

    if (TcheckMaxRtt->isChecked())
    {
        parameters << "--max_rtt_timeout";
        parameters << spinBoxMaxRtt->text();
    }

    if (TcheckHostTime->isChecked())
    {
        parameters << "--host_timeout";
        parameters << spinBoxHostTime->text();
    }

    if (TcheckScanDelay->isChecked())
    {
        parameters << "--scan_delay";
        parameters << spinBoxScanDelay->text();
    }

    //Options
    if (checkBoxDevice->isChecked())
    { // Discover options (tcp syn)
        if (!OlineDevice->text().isEmpty())
        {
            parameters << "-e";
            parameters << OlineDevice->text();

        }
        else
        {
            QMessageBox::warning(this, "NmapSI4", "Please, first insert a Device\n", "Disable Option");
            checkBoxDevice->setCheckState(Qt::Unchecked);
        }
    }

    if (checkDecoy->isChecked())
    { // Discover options (tcp syn)
        if (!lineDecoy->text().isEmpty())
        {
            parameters << "-D";
            parameters << lineDecoy->text();

        }
        else
        {
            QMessageBox::warning(this, "NmapSI4", "Please, first insert a Decoy\n", "Disable Option");
            checkDecoy->setCheckState(Qt::Unchecked);
        }
    }

    if (checkSpoof->isChecked())
    { // Spoof options
        if (!lineEditSpoof->text().isEmpty())
        {
            parameters << "-S";
            parameters << lineEditSpoof->text();

        }
        else
        {
            QMessageBox::warning(this, "NmapSI4", "Please, insert spoof address\n", "Disable Option");
            checkSpoof->setCheckState(Qt::Unchecked);
        }
    }

    if (checkSourcePort->isChecked())
    { // Spoof options
        if (!lineSourcePort->text().isEmpty())
        {
            parameters << "-g";
            parameters << lineSourcePort->text();

        }
        else
        {
            QMessageBox::warning(this, "NmapSI4", "Please, insert port address\n", "Disable Option");
            checkSourcePort->setCheckState(Qt::Unchecked);
        }
    }
    if(_monitor->monitorHostNumber())
    {
        progressScan->setValue(55);
    }

    return parameters;
}
