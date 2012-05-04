/***************************************************************************
 *   Copyright (C) 2012 by Francesco Cecconi                               *
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

#include "profiler.h"
#include "profilerdialog.h"

Profiler::Profiler(ProfilerManager* parent)
: m_ui(parent)
{

}

void Profiler::restoreValuesFromProfile(const QStringList parameters)
{
    for (int index = 0; index < parameters.size(); ++index)
    {
        if (parameters[index].startsWith(QLatin1String("-")) || parameters[index].startsWith(QLatin1String("--")))
        {
            if (!parameters[index].startsWith(QLatin1String("-PA"))
                && !parameters[index].startsWith(QLatin1String("-PS"))
                && !parameters[index].startsWith(QLatin1String("-PU"))
                && !parameters[index].startsWith(QLatin1String("-PO"))
                && !parameters[index].startsWith(QLatin1String("-PY"))
                && !parameters[index].startsWith(QLatin1String("--script")))
            {
                bool isFounded = false;

                // check combo options
                QHash<QString,QPair<QComboBox*,int> >::const_iterator i = preLoadComboList.find(parameters[index]);

                if (i != preLoadComboList.end() )
                {
                    // value founded
                    (*i).first->setCurrentIndex((*i).second);

                    if ( index < parameters.size()-1)
                    {
                        // check if the next string value is an integer
                        if (!parameters[index+1].startsWith(QLatin1String("-")) && !parameters[index+1].startsWith(QLatin1String("--")))
                        {
                            // recover combo value
                            QHash<QString,QLineEdit*>::const_iterator j = lineEditList.find(parameters[index]);

                            if ( j != lineEditList.end() )
                            {
                                (*j)->setText(parameters[index+1]);
                                index++;
                            }
                        }
                    }
                    isFounded = true;
                } // end combo restore value

                if (!isFounded)
                {
                    // check checkBox options
                    QHash<QString,QPair<QCheckBox*,QString> >::const_iterator i = preLoadCheckBoxList.find(parameters[index]);

                    if (i != preLoadCheckBoxList.end())
                    {
                        (*i).first->setChecked(true);
                    }

                    if ( index < parameters.size()-1)
                    {
                        // check if the next string value is an integer
                        if (!parameters[index+1].startsWith(QLatin1String("-")) && !parameters[index+1].startsWith(QLatin1String("--")))
                        {
                            // check value for line edit
                            QHash<QString,QLineEdit*>::const_iterator j = lineEditList.find(parameters[index]);

                            if ( j != lineEditList.end() )
                            {
                                (*j)->setText(parameters[index+1]);
                            }
                            // check value for spin box
                            QHash<QString,QSpinBox*>::const_iterator z = spinBoxList.find(parameters[index]);

                            if ( z != spinBoxList.end() )
                            {
                                int value = parameters[index+1].toInt();
                                (*z)->setValue(value);
                            }
                        }
                    }
                }
            }
            else
            {
                if (!parameters[index].startsWith(QLatin1String("--script")))
                {
                    // parameter with value on append
                    QString token = parameters[index];
                    QString option = token.left(3);
                    token.remove(option);

                    QHash<QString,QPair<QCheckBox*,QString> >::const_iterator i = preLoadCheckBoxList.find(option);

                    if (i != preLoadCheckBoxList.end())
                    {
                        (*i).first->setChecked(true);
                    }

                    if (!token.isEmpty())
                    {
                        QHash<QString,QLineEdit*>::const_iterator j = lineEditList.find(option);

                        if ( j != lineEditList.end() )
                        {
                            (*j)->setText(token);
                        }
                    }
                }
                else
                {
                    // nse script value
                    if (parameters[index].startsWith(QLatin1String("--script-args=")))
                    {
                        QString values = parameters[index];
                        values.remove("--script-args=");
                        m_ui->comboNsePar->lineEdit()->setText(values);
                    }
                    else if (parameters[index].startsWith(QLatin1String("--script=")))
                    {
                        QString values = parameters[index];
                        values.remove("--script=");

                        QStringList scripts = values.split(',',QString::SkipEmptyParts);
                        foreach (const QString& script, scripts)
                        {
                            if (m_ui->m_nseManager->nseTreeActiveSingleScript(script))
                            {
                                scripts.removeAt(script.indexOf(script));
                            }
                        }

                        if (scripts.size())
                        {
                            m_ui->comboNseInv->lineEdit()->setText(scripts.join(","));
                        }
                    }
                }
            }
        } // end first if
    } // end for
}


void Profiler::loadDefaultHash()
{
    preLoadOptionsCheckBox();
    preLoadOptionsCombo();
}

void Profiler::preLoadOptionsCombo()
{
    QPair<QComboBox*,int> comboDefault;

    // TCP Scan
    comboDefault.first = m_ui->comboScanTcp;

    comboDefault.second = 1;
    preLoadComboList.insert("-sT",comboDefault);
    comboDefault.second = 2;
    preLoadComboList.insert("-sP",comboDefault);
    comboDefault.second = 3;
    preLoadComboList.insert("-sS",comboDefault);
    comboDefault.second = 4;
    preLoadComboList.insert("-sA",comboDefault);
    comboDefault.second = 5;
    preLoadComboList.insert("-sM",comboDefault);
    comboDefault.second = 6;
    preLoadComboList.insert("-sF",comboDefault);
    comboDefault.second = 7;
    preLoadComboList.insert("-sN",comboDefault);
    comboDefault.second = 8;
    preLoadComboList.insert("-sX",comboDefault);
    comboDefault.second = 9;
    preLoadComboList.insert("-sW",comboDefault);

    // Non-TCP scans
    comboDefault.first = m_ui->comboScanNonTcp;

    comboDefault.second = 1;
    preLoadComboList.insert("-sL",comboDefault);
    comboDefault.second = 2;
    preLoadComboList.insert("-sU",comboDefault);
    comboDefault.second = 3;
    preLoadComboList.insert("-sO",comboDefault);
    comboDefault.second = 4;
    preLoadComboList.insert("-sY",comboDefault);
    comboDefault.second = 5;
    preLoadComboList.insert("-sZ",comboDefault);

    // Timing Template
    comboDefault.first = m_ui->comboTiming;

    comboDefault.second = 1;
    preLoadComboList.insert("-T0",comboDefault);
    comboDefault.second = 2;
    preLoadComboList.insert("-T1",comboDefault);
    comboDefault.second = 3;
    preLoadComboList.insert("-T2",comboDefault);
    comboDefault.second = 4;
    preLoadComboList.insert("-T3",comboDefault);
    comboDefault.second = 5;
    preLoadComboList.insert("-T4",comboDefault);
    comboDefault.second = 6;
    preLoadComboList.insert("-T5",comboDefault);

    // Port Range
    comboDefault.first = m_ui->portCombo;

    comboDefault.second = 1;
    preLoadComboList.insert("-p-",comboDefault);
    comboDefault.second = 2;
    preLoadComboList.insert("-F",comboDefault);
    comboDefault.second = 3;
    //parameters is request
    preLoadComboList.insert("-p",comboDefault);
    lineEditList.insert("-p",m_ui->portEdit);

    // dns Resolution
    comboDefault.first = m_ui->comboDNSResolv;

    comboDefault.second = 1;
    preLoadComboList.insert("-R",comboDefault);
    comboDefault.second = 2;
    preLoadComboList.insert("-n",comboDefault);

    // verbosity level
    comboDefault.first = m_ui->comboVerbosity;

    comboDefault.second = 1;
    preLoadComboList.insert("-v",comboDefault);
    comboDefault.second = 2;
    preLoadComboList.insert("-vv",comboDefault);
    comboDefault.second = 3;
    preLoadComboList.insert("-d",comboDefault);
    comboDefault.second = 4;
    preLoadComboList.insert("-d2",comboDefault);
}

void Profiler::preLoadOptionsCheckBox()
{

    QPair<QCheckBox*,QString> comboDefault;

    // scan stack
    // ftp bounce
    comboDefault.first = m_ui->checkFtpBounce;
    comboDefault.second = "";
    preLoadCheckBoxList.insert("-b", comboDefault);
    lineEditList.insert("-b",m_ui->bounceEdit);

    // idle scan
    comboDefault.first = m_ui->checkIdleScan;
    comboDefault.second = "";
    preLoadCheckBoxList.insert("-sI", comboDefault);
    lineEditList.insert("-sI",m_ui->lineIdleScan);

    // Aggressive scan
    comboDefault.first = m_ui->checkAggressiveOptions;
    comboDefault.second = "none";
    preLoadCheckBoxList.insert("-A", comboDefault);

    // RPC scan
    comboDefault.first = m_ui->rpcBox;
    comboDefault.second = "none";
    preLoadCheckBoxList.insert("-sR", comboDefault);

    // OS Detection
    comboDefault.first = m_ui->checkOS;
    comboDefault.second = "none";
    preLoadCheckBoxList.insert("-O", comboDefault);

    // Version Probe
    comboDefault.first = m_ui->versionBox;
    comboDefault.second = "none";
    preLoadCheckBoxList.insert("-sV", comboDefault);

    // Ipv6
    comboDefault.first = m_ui->checkIpv6;
    comboDefault.second = "none";
    preLoadCheckBoxList.insert("-6", comboDefault);

    // end scan stack

    // start ping stack

    // Dont'ping
    comboDefault.first = m_ui->notpingBox;
    comboDefault.second = "none";
    preLoadCheckBoxList.insert("-P0", comboDefault);

    // ICMP echo
    comboDefault.first = m_ui->checkIcmpEcho;
    comboDefault.second = "none";
    preLoadCheckBoxList.insert("-PE", comboDefault);

    // ICMP timestamp
    comboDefault.first = m_ui->checkIcmpTimestamp;
    comboDefault.second = "none";
    preLoadCheckBoxList.insert("-PP", comboDefault);

    // ICMP netmask
    comboDefault.first = m_ui->checkIcmpNetmask;
    comboDefault.second = "none";
    preLoadCheckBoxList.insert("-PM", comboDefault);

    //TCP ack ping
    comboDefault.first = m_ui->checkTcpPing;
    comboDefault.second = "21,23,80,3389";
    preLoadCheckBoxList.insert("-PA", comboDefault);
    lineEditList.insert("-PA",m_ui->lineTcpPing);

    //TCP syn ping
    comboDefault.first = m_ui->checkTcpSyn;
    comboDefault.second = "22,25,80";
    preLoadCheckBoxList.insert("-PS", comboDefault);
    lineEditList.insert("-PS",m_ui->lineSynPing);

    // UDP Ping
    comboDefault.first = m_ui->checkUdpPing;
    comboDefault.second = "";
    preLoadCheckBoxList.insert("-PU", comboDefault);
    lineEditList.insert("-PU",m_ui->lineUdpPing);

    // IP proto
    comboDefault.first = m_ui->checkProtoPing;
    comboDefault.second = "";
    preLoadCheckBoxList.insert("-PO", comboDefault);
    lineEditList.insert("-PO",m_ui->lineProtoPing);

    //SCTP init ping
    comboDefault.first = m_ui->checkSctpPing;
    comboDefault.second = "";
    preLoadCheckBoxList.insert("-PY", comboDefault);
    lineEditList.insert("-PY",m_ui->lineSctpPing);

    // end ping stack
    // start timing stack

    //Min outstanding
    comboDefault.first = m_ui->TcheckMinPar;
    comboDefault.second = '1';
    preLoadCheckBoxList.insert("--min-parallelism", comboDefault);
    spinBoxList.insert("--min-parallelism",m_ui->TspinBoxMinP);

    //Min outstanding
    comboDefault.first = m_ui->TcheckMaxPar;
    comboDefault.second = '1';
    preLoadCheckBoxList.insert("--max-parallelism", comboDefault);
    spinBoxList.insert("--max-parallelism",m_ui->spinBoxMaxPar);

    //Max time
    comboDefault.first = m_ui->TcheckHostTime;
    comboDefault.second = "6000";
    preLoadCheckBoxList.insert("--host-timeout", comboDefault);
    spinBoxList.insert("--host-timeout",m_ui->spinBoxHostTime);

    //initial probe
    comboDefault.first = m_ui->TcheckInitRtt;
    comboDefault.second = "6000";
    preLoadCheckBoxList.insert("--initial-rtt-timeout", comboDefault);
    spinBoxList.insert("--initial-rtt-timeout",m_ui->spinBoxInitRtt);

    //min probe
    comboDefault.first = m_ui->TcheckMinRtt;
    comboDefault.second = "6000";
    preLoadCheckBoxList.insert("--min-rtt-timeout", comboDefault);
    spinBoxList.insert("--min-rtt-timeout",m_ui->spinBoxMinRtt);

    //max probe
    comboDefault.first = m_ui->TcheckMaxRtt;
    comboDefault.second = "6000";
    preLoadCheckBoxList.insert("--max-rtt-timeout", comboDefault);
    spinBoxList.insert("--max-rtt-timeout",m_ui->spinBoxMaxRtt);

    //scan delay
    comboDefault.first = m_ui->TcheckScanDelay;
    comboDefault.second = "6000";
    preLoadCheckBoxList.insert("--scan-delay", comboDefault);
    spinBoxList.insert("--scan-delay",m_ui->spinBoxScanDelay);

    //Max scan
    comboDefault.first = m_ui->TcheckScanDelayMax;
    comboDefault.second = "6000";
    preLoadCheckBoxList.insert("--max-scan-delay", comboDefault);
    spinBoxList.insert("--max-scan-delay",m_ui->spinBoxScanDelayMax);

    // end timing stack

    // start options

    // network interface
    comboDefault.first = m_ui->checkBoxDevice;
    comboDefault.second = "";
    preLoadCheckBoxList.insert("-e", comboDefault);
    lineEditList.insert("-e",m_ui->OlineDevice);

    // source spoof ip
    comboDefault.first = m_ui->checkSpoof;
    comboDefault.second = "";
    preLoadCheckBoxList.insert("-S", comboDefault);
    lineEditList.insert("-S",m_ui->lineEditSpoof);

    // set decoy
    comboDefault.first = m_ui->checkDecoy;
    comboDefault.second = "";
    preLoadCheckBoxList.insert("-D", comboDefault);
    lineEditList.insert("-D",m_ui->lineDecoy);

    // source spoof port
    comboDefault.first = m_ui->checkSourcePort;
    comboDefault.second = "";
    preLoadCheckBoxList.insert("-g", comboDefault);
    lineEditList.insert("-g",m_ui->lineSourcePort);

    // ipv4 ttl
    comboDefault.first = m_ui->TcheckIpv4ttl;
    comboDefault.second = "127";
    preLoadCheckBoxList.insert("--ttl", comboDefault);
    spinBoxList.insert("--ttl",m_ui->spinBoxIpv4ttl);

    // max retries
    comboDefault.first = m_ui->checkMaxRetries;
    comboDefault.second = "";
    preLoadCheckBoxList.insert("--max-retries", comboDefault);
    lineEditList.insert("--max-retries",m_ui->lineMaxRetries);

    // ordered scanned
    comboDefault.first = m_ui->checkOrdered;
    comboDefault.second = "none";
    preLoadCheckBoxList.insert("-r", comboDefault);

    // fragment IP
    comboDefault.first = m_ui->checkFrag;
    comboDefault.second = "none";
    preLoadCheckBoxList.insert("-f", comboDefault);

    // traceroute
    comboDefault.first = m_ui->checkTraceroute;
    comboDefault.second = "none";
    preLoadCheckBoxList.insert("--traceroute", comboDefault);

    // end options
}

QStringList Profiler::buildExtensions()
{
    QStringList parameters;

    // NSE check
    if (m_ui->m_nseManager->getActiveNseScript().size())
    {
        QString tmpListScript_("--script=");
        QString tmpListParam_("--script-args=");
        QString tmpList_;
        QString tmpListArgs_;

        // read nse category actived
        Q_FOREACH (const QString& token, m_ui->m_nseManager->getActiveNseScript())
        {
            tmpList_.append(token);
            tmpList_.append(",");
        }

        // load nse manual script
        if (!m_ui->comboNseInv->lineEdit()->text().isEmpty())
        {
            QStringList manualNse = m_ui->comboNseInv->lineEdit()->text().split(',');
            Q_FOREACH (const QString& token, manualNse)
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

        if (!m_ui->comboNsePar->lineEdit()->text().isEmpty())
        {
            QString argsClean = m_ui->comboNsePar->lineEdit()->text().remove('"');
            argsClean = argsClean.remove('\'');
            QStringList argsNse = argsClean.split(',');
            Q_FOREACH (const QString& token, argsNse)
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
    } // End NSE check

    switch (m_ui->comboScanTcp->currentIndex()) { //scan check
    case 1:
//    Connect Scan
        parameters << "-sT";
        break;
    case 2:
//    Ping Sweep
        parameters << "-sP";
        break;
    case 3:
//    SYN Stealth Scan (rootMode)
        parameters << "-sS";
        break;
    case 4:
//    ACK Stealth Scan (rootMode)
        parameters << "-sA";
        break;
    case 5:
//    Mainmon Stealth Scan (rootMode)
        parameters << "-sM";
        break;
    case 6:
//    FIN Stealth Scan (rootMode)
        parameters << "-sF";
        break;
    case 7:
//    NULL Stealth Scan (rootMode)
        parameters << "-sN";
        break;
    case 8:
//    XMAS Tree Stealth Scan (rootMode)
        parameters << "-sX";
        break;
    case 9:
//    TCP Window Scan (rootMode)
        parameters << "-sW";
        break;
    } // end switch scan

    switch(m_ui->comboScanNonTcp->currentIndex())
    {
    case 1:
        //Host List
        parameters << "-sL";
        break;
    case 2:
        //UDP Ports Scan (rootMode)
        parameters << "-sU";
        break;
    case 3:
        //IP Protocol Scan (rootMode)
        parameters << "-sO";
        break;
    case 4:
        //SCTP INIT port scan (rootMode)
        parameters << "-sY";
        break;
    case 5:
        //SCTP cookie-echo port scan (rootMode)
        parameters << "-sZ";
        break;
    }

    if (m_ui->checkFtpBounce->isChecked() && !m_ui->bounceEdit->text().isEmpty())
    {
        // FTP Bounce attack
        parameters << "-b";
        parameters << m_ui->bounceEdit->text();
    }
    else
    {
        m_ui->checkFtpBounce->setCheckState(Qt::Unchecked);
    }

    if (m_ui->checkIdleScan->isChecked() && !m_ui->lineIdleScan->text().isEmpty())
    {
        // Idle scan
        parameters << "-sI";
        parameters << m_ui->lineIdleScan->text();
    }
    else
    {
        m_ui->checkIdleScan->setCheckState(Qt::Unchecked);
    }

    // Aggressive options
    if (m_ui->checkAggressiveOptions->isChecked())
    {
        parameters << "-A";
    }
    // start option scan
    if (m_ui->rpcBox->isChecked())
    {
        parameters << "-sR";
    }

    if (m_ui->versionBox->isChecked())
    {
        parameters << "-sV";
    }

    if (m_ui->notpingBox->isChecked())
    {
        parameters << "-P0";
    }

    if (m_ui->checkOS->isChecked())
    {
        parameters << "-O";
    }
    //end Extension

    switch (m_ui->portCombo->currentIndex())
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
        if (!m_ui->portEdit->text().isEmpty())
        {
            parameters << "-p";
            parameters << m_ui->portEdit->text();
        }
        else
        {
            QMessageBox::warning(m_ui, "NmapSI4", m_ui->tr("No Ports Range (ex: 20-80)\n"), m_ui->tr("Close"));
        }
    default:
        break;
    }


    if (m_ui->checkTcpPing->isChecked())
    { // Discover options (tcp ack)
        QString tmpCommand;
        tmpCommand.append("-PA");
        tmpCommand.append(m_ui->lineTcpPing->text());
        parameters << tmpCommand;
    }

    if (m_ui->checkTcpSyn->isChecked())
    { // Discover options (tcp syn)

        QString tmpCommand;
        tmpCommand.append("-PS");
        tmpCommand.append(m_ui->lineSynPing->text());
        parameters << tmpCommand;
    }

    if (m_ui->checkUdpPing->isChecked())
    { // Discover options (tcp syn)
        QString tmpCommand;
        tmpCommand.append("-PU");
        if (!m_ui->lineUdpPing->text().isEmpty())
        {
            tmpCommand.append(m_ui->lineUdpPing->text());
        }
        parameters << tmpCommand;
    }

    if (m_ui->checkProtoPing->isChecked())
    { // IPProto ping (tcp syn)
        QString tmpCommand;
        tmpCommand.append("-PO");

        if (!m_ui->lineProtoPing->text().isEmpty())
        {
            tmpCommand.append(m_ui->lineUdpPing->text());
        }

        parameters << tmpCommand;
    }

    if (m_ui->checkSctpPing->isChecked())
    { // IPProto ping (tcp syn)
        QString tmpCommand;
        tmpCommand.append("-PY");

        if (!m_ui->lineSctpPing->text().isEmpty())
        {
            tmpCommand.append(m_ui->lineSctpPing->text());
        }

        parameters << tmpCommand;
    }

    // Discover option
    if (m_ui->checkIcmpEcho->isChecked())
    {
        parameters << "-PE";
    }

    if (m_ui->checkIcmpTimestamp->isChecked())
    {
        parameters << "-PP";
    }

    if (m_ui->checkIcmpNetmask->isChecked())
    {
        parameters << "-PM";
    }

    switch (m_ui->comboTiming->currentIndex())
    { // port combo Timing
    case 1:
        //Paranoid
        parameters << "-T0";
        break;
    case 2:
        //Sneaky
        parameters << "-T1";
        break;
    case 3:
        //Polite
        parameters << "-T2";
        break;
    case 4:
        //Normal
        parameters << "-T3";
        break;
    case 5:
        //Aggressive
        parameters << "-T4";
        break;
    case 6:
        //Insane
        parameters << "-T5";
        break;
    default:
        break;
    }

    switch (m_ui->comboDNSResolv->currentIndex())
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

    switch (m_ui->comboVerbosity->currentIndex())
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

    // Misc Options
    if (m_ui->checkOrdered->isChecked())
    {
        parameters << "-r"; // Ordered Port
    }

    if (m_ui->checkIpv6->isChecked())
    {
        parameters << "-6"; // Ipv6
    }

    if (m_ui->checkFrag->isChecked())
    {
        parameters << "-f";
    }

    // traceroute
    if (m_ui->checkTraceroute->isChecked())
    {
        parameters << "--traceroute";
    }

    if (m_ui->checkMaxRetries->isChecked())
    {
        parameters << "--max-retries";
        if (!m_ui->lineMaxRetries->text().isEmpty())
        {
            parameters << m_ui->lineMaxRetries->text();
        }
    }

    // Timing options
    if (m_ui->TcheckIpv4ttl->isChecked())
    {
        parameters << "--ttl";
        parameters << m_ui->spinBoxIpv4ttl->text();
    }

    if (m_ui->TcheckMinPar->isChecked())
    {
        parameters << "--min-parallelism";
        parameters << m_ui->TspinBoxMinP->text();
    }

    if (m_ui->TcheckMaxPar->isChecked())
    {
        parameters << "--max-parallelism";
        parameters << m_ui->spinBoxMaxPar->text();
    }

    if (m_ui->TcheckInitRtt->isChecked())
    {
        parameters << "--initial-rtt-timeout";
        parameters << m_ui->spinBoxInitRtt->text();
    }

    if (m_ui->TcheckMinRtt->isChecked())
    {
        parameters << "--min-rtt-timeout";
        parameters << m_ui->spinBoxMinRtt->text();
    }

    if (m_ui->TcheckMaxRtt->isChecked())
    {
        parameters << "--max-rtt-timeout";
        parameters << m_ui->spinBoxMaxRtt->text();
    }

    if (m_ui->TcheckHostTime->isChecked())
    {
        parameters << "--host-timeout";
        parameters << m_ui->spinBoxHostTime->text();
    }

    if (m_ui->TcheckScanDelay->isChecked())
    {
        parameters << "--scan-delay";
        parameters << m_ui->spinBoxScanDelay->text();
    }

    if (m_ui->TcheckScanDelayMax->isChecked())
    {
        parameters << "--max-scan-delay";
        parameters << m_ui->spinBoxScanDelayMax->text();
    }

    //Options
    if (m_ui->checkBoxDevice->isChecked() && !m_ui->OlineDevice->text().isEmpty())
    { // Discover options (tcp syn)
            parameters << "-e";
            parameters << m_ui->OlineDevice->text();
    }
    else
    {
        m_ui->checkBoxDevice->setCheckState(Qt::Unchecked);
    }

    if (m_ui->checkDecoy->isChecked())
    { // Discover options (tcp syn)
        if (!m_ui->lineDecoy->text().isEmpty())
        {
            parameters << "-D";
            parameters << m_ui->lineDecoy->text();

        }
        else
        {
            QMessageBox::warning(m_ui, "NmapSI4", "Please, first insert a Decoy\n", "Disable Option");
            m_ui->checkDecoy->setCheckState(Qt::Unchecked);
        }
    }

    if (m_ui->checkSpoof->isChecked())
    { // Spoof options
        if (!m_ui->lineEditSpoof->text().isEmpty())
        {
            parameters << "-S";
            parameters << m_ui->lineEditSpoof->text();

        }
        else
        {
            QMessageBox::warning(m_ui, "NmapSI4", "Please, insert spoof address\n", "Disable Option");
            m_ui->checkSpoof->setCheckState(Qt::Unchecked);
        }
    }

    if (m_ui->checkSourcePort->isChecked())
    { // Spoof options
        if (!m_ui->lineSourcePort->text().isEmpty())
        {
            parameters << "-g";
            parameters << m_ui->lineSourcePort->text();

        }
        else
        {
            QMessageBox::warning(m_ui, "NmapSI4", "Please, insert port address\n", "Disable Option");
            m_ui->checkSourcePort->setCheckState(Qt::Unchecked);
        }
    }

    return parameters;
}