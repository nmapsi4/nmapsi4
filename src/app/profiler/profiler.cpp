/*
Copyright 2012  Francesco Cecconi <francesco.cecconi@gmail.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "profiler.h"
#include "profilermanager.h"

Profiler::Profiler(ProfilerManager* parent)
    : m_ui(parent)
{
    loadDefaultHash();
}

void Profiler::restoreValuesFromProfile(const QStringList parameters)
{
    for (int index = 0; index < parameters.size(); ++index) {
        if (parameters[index].startsWith(QLatin1String("-")) || parameters[index].startsWith(QLatin1String("--"))) {
            if (!parameters[index].startsWith(QLatin1String("-PA"))
                    && !parameters[index].startsWith(QLatin1String("-PS"))
                    && !parameters[index].startsWith(QLatin1String("-PU"))
                    && !parameters[index].startsWith(QLatin1String("-PO"))
                    && !parameters[index].startsWith(QLatin1String("-PY"))
                    && !parameters[index].startsWith(QLatin1String("--script"))) {
                bool isFounded = false;

                // check combo options
                QHash<QString, QPair<QComboBox*, int> >::const_iterator i = preLoadComboList.find(parameters[index]);

                if (i != preLoadComboList.end()) {
                    // value founded
                    (*i).first->setCurrentIndex((*i).second);

                    if (index < parameters.size() - 1) {
                        // check if the next string value is an integer
                        if (!parameters[index + 1].startsWith(QLatin1String("-")) && !parameters[index + 1].startsWith(QLatin1String("--"))) {
                            // recover combo value
                            QHash<QString, QLineEdit*>::const_iterator j = lineEditList.find(parameters[index]);

                            if (j != lineEditList.end()) {
                                (*j)->setText(parameters[index + 1]);
                                index++;
                            }
                        }
                    }
                    isFounded = true;
                } // end combo restore value

                if (!isFounded) {
                    // check checkBox options
                    QHash<QString, QPair<QCheckBox*, QString> >::const_iterator i = preLoadCheckBoxList.find(parameters[index]);

                    if (i != preLoadCheckBoxList.end()) {
                        (*i).first->setChecked(true);
                    }

                    if (index < parameters.size() - 1) {
                        // check if the next string value is an integer
                        if (!parameters[index + 1].startsWith(QLatin1String("-")) && !parameters[index + 1].startsWith(QLatin1String("--"))) {
                            // check value for line edit
                            QHash<QString, QLineEdit*>::const_iterator j = lineEditList.find(parameters[index]);

                            if (j != lineEditList.end()) {
                                (*j)->setText(parameters[index + 1]);
                            }
                            // check value for spin box
                            QHash<QString, QSpinBox*>::const_iterator z = spinBoxList.find(parameters[index]);

                            if (z != spinBoxList.end()) {
                                int value = parameters[index + 1].toInt();
                                (*z)->setValue(value);
                            }
                        }
                    }
                }
            } else {
                if (!parameters[index].startsWith(QLatin1String("--script"))) {
                    // parameter with value on append
                    QString token = parameters[index];
                    QString option = token.left(3);
                    token.remove(option);

                    QHash<QString, QPair<QCheckBox*, QString> >::const_iterator i = preLoadCheckBoxList.find(option);

                    if (i != preLoadCheckBoxList.end()) {
                        (*i).first->setChecked(true);
                    }

                    if (!token.isEmpty()) {
                        QHash<QString, QLineEdit*>::const_iterator j = lineEditList.find(option);

                        if (j != lineEditList.end()) {
                            (*j)->setText(token);
                        }
                    }
                } else {
                    // nse script value
                    if (parameters[index].startsWith(QLatin1String("--script-args="))) {
                        QString values = parameters[index];
                        values.remove("--script-args=");
                        m_ui->m_dialogUi->comboNsePar->lineEdit()->setText(values);
                    } else if (parameters[index].startsWith(QLatin1String("--script="))) {
                        QString values = parameters[index];
                        values.remove("--script=");

                        QStringList scripts = values.split(',', QString::SkipEmptyParts);
                        foreach(const QString & script, scripts) {
                            if (m_ui->m_nseManager->nseTreeActiveSingleScript(script)) {
                                scripts.removeAt(script.indexOf(script));
                            }
                        }

                        if (scripts.size()) {
                            m_ui->m_dialogUi->comboNseInv->lineEdit()->setText(scripts.join(","));
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
    QPair<QComboBox*, int> comboDefault;

    // TCP Scan
    comboDefault.first = m_ui->m_dialogUi->comboScanTcp;

    comboDefault.second = 1;
    preLoadComboList.insert("-sT", comboDefault);
    comboDefault.second = 2;
    preLoadComboList.insert("-sP", comboDefault);
    comboDefault.second = 3;
    preLoadComboList.insert("-sS", comboDefault);
    comboDefault.second = 4;
    preLoadComboList.insert("-sA", comboDefault);
    comboDefault.second = 5;
    preLoadComboList.insert("-sM", comboDefault);
    comboDefault.second = 6;
    preLoadComboList.insert("-sF", comboDefault);
    comboDefault.second = 7;
    preLoadComboList.insert("-sN", comboDefault);
    comboDefault.second = 8;
    preLoadComboList.insert("-sX", comboDefault);
    comboDefault.second = 9;
    preLoadComboList.insert("-sW", comboDefault);

    // Non-TCP scans
    comboDefault.first = m_ui->m_dialogUi->comboScanNonTcp;

    comboDefault.second = 1;
    preLoadComboList.insert("-sL", comboDefault);
    comboDefault.second = 2;
    preLoadComboList.insert("-sU", comboDefault);
    comboDefault.second = 3;
    preLoadComboList.insert("-sO", comboDefault);
    comboDefault.second = 4;
    preLoadComboList.insert("-sY", comboDefault);
    comboDefault.second = 5;
    preLoadComboList.insert("-sZ", comboDefault);

    // Timing Template
    comboDefault.first = m_ui->m_dialogUi->comboTiming;

    comboDefault.second = 1;
    preLoadComboList.insert("-T0", comboDefault);
    comboDefault.second = 2;
    preLoadComboList.insert("-T1", comboDefault);
    comboDefault.second = 3;
    preLoadComboList.insert("-T2", comboDefault);
    comboDefault.second = 4;
    preLoadComboList.insert("-T3", comboDefault);
    comboDefault.second = 5;
    preLoadComboList.insert("-T4", comboDefault);
    comboDefault.second = 6;
    preLoadComboList.insert("-T5", comboDefault);

    // Port Range
    comboDefault.first = m_ui->m_dialogUi->portCombo;

    comboDefault.second = 1;
    preLoadComboList.insert("-p-", comboDefault);
    comboDefault.second = 2;
    preLoadComboList.insert("-F", comboDefault);
    comboDefault.second = 3;
    //parameters is request
    preLoadComboList.insert("-p", comboDefault);
    lineEditList.insert("-p", m_ui->m_dialogUi->portEdit);

    // dns Resolution
    comboDefault.first = m_ui->m_dialogUi->comboDNSResolv;

    comboDefault.second = 1;
    preLoadComboList.insert("-R", comboDefault);
    comboDefault.second = 2;
    preLoadComboList.insert("-n", comboDefault);

    // verbosity level
    comboDefault.first = m_ui->m_dialogUi->comboVerbosity;

    comboDefault.second = 1;
    preLoadComboList.insert("-v", comboDefault);
    comboDefault.second = 2;
    preLoadComboList.insert("-vv", comboDefault);
    comboDefault.second = 3;
    preLoadComboList.insert("-d", comboDefault);
    comboDefault.second = 4;
    preLoadComboList.insert("-d2", comboDefault);
}

void Profiler::preLoadOptionsCheckBox()
{

    QPair<QCheckBox*, QString> checkDefault;

    // scan stack
    // ftp bounce
    checkDefault.first = m_ui->m_dialogUi->checkFtpBounce;
    checkDefault.second = "";
    preLoadCheckBoxList.insert("-b", checkDefault);
    lineEditList.insert("-b", m_ui->m_dialogUi->bounceEdit);

    // idle scan
    checkDefault.first = m_ui->m_dialogUi->checkIdleScan;
    checkDefault.second = "";
    preLoadCheckBoxList.insert("-sI", checkDefault);
    lineEditList.insert("-sI", m_ui->m_dialogUi->lineIdleScan);

    // Aggressive scan
    checkDefault.first = m_ui->m_dialogUi->checkAggressiveOptions;
    checkDefault.second = "none";
    preLoadCheckBoxList.insert("-A", checkDefault);

    // RPC scan
    checkDefault.first = m_ui->m_dialogUi->rpcBox;
    checkDefault.second = "none";
    preLoadCheckBoxList.insert("-sR", checkDefault);

    // OS Detection
    checkDefault.first = m_ui->m_dialogUi->checkOS;
    checkDefault.second = "none";
    preLoadCheckBoxList.insert("-O", checkDefault);

    // Version Probe
    checkDefault.first = m_ui->m_dialogUi->versionBox;
    checkDefault.second = "none";
    preLoadCheckBoxList.insert("-sV", checkDefault);

    // Ipv6
    checkDefault.first = m_ui->m_dialogUi->checkIpv6;
    checkDefault.second = "none";
    preLoadCheckBoxList.insert("-6", checkDefault);

    // end scan stack

    // start ping stack

    // Dont'ping
    checkDefault.first = m_ui->m_dialogUi->notpingBox;
    checkDefault.second = "none";
    preLoadCheckBoxList.insert("-Pn", checkDefault);

    // ICMP echo
    checkDefault.first = m_ui->m_dialogUi->checkIcmpEcho;
    checkDefault.second = "none";
    preLoadCheckBoxList.insert("-PE", checkDefault);

    // ICMP timestamp
    checkDefault.first = m_ui->m_dialogUi->checkIcmpTimestamp;
    checkDefault.second = "none";
    preLoadCheckBoxList.insert("-PP", checkDefault);

    // ICMP netmask
    checkDefault.first = m_ui->m_dialogUi->checkIcmpNetmask;
    checkDefault.second = "none";
    preLoadCheckBoxList.insert("-PM", checkDefault);

    //TCP ack ping
    checkDefault.first = m_ui->m_dialogUi->checkTcpPing;
    checkDefault.second = "21,23,80,3389";
    preLoadCheckBoxList.insert("-PA", checkDefault);
    lineEditList.insert("-PA", m_ui->m_dialogUi->lineTcpPing);

    //TCP syn ping
    checkDefault.first = m_ui->m_dialogUi->checkTcpSyn;
    checkDefault.second = "22,25,80";
    preLoadCheckBoxList.insert("-PS", checkDefault);
    lineEditList.insert("-PS", m_ui->m_dialogUi->lineSynPing);

    // UDP Ping
    checkDefault.first = m_ui->m_dialogUi->checkUdpPing;
    checkDefault.second = "";
    preLoadCheckBoxList.insert("-PU", checkDefault);
    lineEditList.insert("-PU", m_ui->m_dialogUi->lineUdpPing);

    // IP proto
    checkDefault.first = m_ui->m_dialogUi->checkProtoPing;
    checkDefault.second = "";
    preLoadCheckBoxList.insert("-PO", checkDefault);
    lineEditList.insert("-PO", m_ui->m_dialogUi->lineProtoPing);

    //SCTP init ping
    checkDefault.first = m_ui->m_dialogUi->checkSctpPing;
    checkDefault.second = "";
    preLoadCheckBoxList.insert("-PY", checkDefault);
    lineEditList.insert("-PY", m_ui->m_dialogUi->lineSctpPing);

    // end ping stack
    // start timing stack

    //Min outstanding
    checkDefault.first = m_ui->m_dialogUi->TcheckMinPar;
    checkDefault.second = '1';
    preLoadCheckBoxList.insert("--min-parallelism", checkDefault);
    spinBoxList.insert("--min-parallelism", m_ui->m_dialogUi->TspinBoxMinP);

    //Min outstanding
    checkDefault.first = m_ui->m_dialogUi->TcheckMaxPar;
    checkDefault.second = '1';
    preLoadCheckBoxList.insert("--max-parallelism", checkDefault);
    spinBoxList.insert("--max-parallelism", m_ui->m_dialogUi->spinBoxMaxPar);

    //Max time
    checkDefault.first = m_ui->m_dialogUi->TcheckHostTime;
    checkDefault.second = "6000";
    preLoadCheckBoxList.insert("--host-timeout", checkDefault);
    spinBoxList.insert("--host-timeout", m_ui->m_dialogUi->spinBoxHostTime);

    //initial probe
    checkDefault.first = m_ui->m_dialogUi->TcheckInitRtt;
    checkDefault.second = "6000";
    preLoadCheckBoxList.insert("--initial-rtt-timeout", checkDefault);
    spinBoxList.insert("--initial-rtt-timeout", m_ui->m_dialogUi->spinBoxInitRtt);

    //min probe
    checkDefault.first = m_ui->m_dialogUi->TcheckMinRtt;
    checkDefault.second = "6000";
    preLoadCheckBoxList.insert("--min-rtt-timeout", checkDefault);
    spinBoxList.insert("--min-rtt-timeout", m_ui->m_dialogUi->spinBoxMinRtt);

    //max probe
    checkDefault.first = m_ui->m_dialogUi->TcheckMaxRtt;
    checkDefault.second = "6000";
    preLoadCheckBoxList.insert("--max-rtt-timeout", checkDefault);
    spinBoxList.insert("--max-rtt-timeout", m_ui->m_dialogUi->spinBoxMaxRtt);

    //scan delay
    checkDefault.first = m_ui->m_dialogUi->TcheckScanDelay;
    checkDefault.second = "6000";
    preLoadCheckBoxList.insert("--scan-delay", checkDefault);
    spinBoxList.insert("--scan-delay", m_ui->m_dialogUi->spinBoxScanDelay);

    //Max scan
    checkDefault.first = m_ui->m_dialogUi->TcheckScanDelayMax;
    checkDefault.second = "6000";
    preLoadCheckBoxList.insert("--max-scan-delay", checkDefault);
    spinBoxList.insert("--max-scan-delay", m_ui->m_dialogUi->spinBoxScanDelayMax);

    // end timing stack

    // start options

    // network interface
    checkDefault.first = m_ui->m_dialogUi->checkBoxDevice;
    checkDefault.second = "";
    preLoadCheckBoxList.insert("-e", checkDefault);
    lineEditList.insert("-e", m_ui->m_dialogUi->OlineDevice);

    // source spoof ip
    checkDefault.first = m_ui->m_dialogUi->checkSpoof;
    checkDefault.second = "";
    preLoadCheckBoxList.insert("-S", checkDefault);
    lineEditList.insert("-S", m_ui->m_dialogUi->lineEditSpoof);

    // set decoy
    checkDefault.first = m_ui->m_dialogUi->checkDecoy;
    checkDefault.second = "";
    preLoadCheckBoxList.insert("-D", checkDefault);
    lineEditList.insert("-D", m_ui->m_dialogUi->lineDecoy);

    // source spoof port
    checkDefault.first = m_ui->m_dialogUi->checkSourcePort;
    checkDefault.second = "";
    preLoadCheckBoxList.insert("-g", checkDefault);
    lineEditList.insert("-g", m_ui->m_dialogUi->lineSourcePort);

    // ipv4 ttl
    checkDefault.first = m_ui->m_dialogUi->TcheckIpv4ttl;
    checkDefault.second = "127";
    preLoadCheckBoxList.insert("--ttl", checkDefault);
    spinBoxList.insert("--ttl", m_ui->m_dialogUi->spinBoxIpv4ttl);

    // max retries
    checkDefault.first = m_ui->m_dialogUi->checkMaxRetries;
    checkDefault.second = "";
    preLoadCheckBoxList.insert("--max-retries", checkDefault);
    lineEditList.insert("--max-retries", m_ui->m_dialogUi->lineMaxRetries);

    // ordered scanned
    checkDefault.first = m_ui->m_dialogUi->checkOrdered;
    checkDefault.second = "none";
    preLoadCheckBoxList.insert("-r", checkDefault);

    // fragment IP
    checkDefault.first = m_ui->m_dialogUi->checkFrag;
    checkDefault.second = "none";
    preLoadCheckBoxList.insert("-f", checkDefault);

    // traceroute
    checkDefault.first = m_ui->m_dialogUi->checkTraceroute;
    checkDefault.second = "none";
    preLoadCheckBoxList.insert("--traceroute", checkDefault);

    // packet-trace
    checkDefault.first = m_ui->m_dialogUi->checkPacketTrace;
    checkDefault.second = "none";
    preLoadCheckBoxList.insert("--packet-trace", checkDefault);

    // end options
}

void Profiler::resetOptions()
{
    QHash<QString, QPair<QCheckBox*, QString> >::const_iterator i;

    for (i = preLoadCheckBoxList.constBegin(); i != preLoadCheckBoxList.constEnd(); ++i) {
        i.value().first->setChecked(false);
    }

    QHash<QString, QPair<QComboBox*, int> >::const_iterator j;

    for (j = preLoadComboList.constBegin(); j != preLoadComboList.constEnd(); ++j) {
        j.value().first->setCurrentIndex(0);
    }

    m_ui->m_nseManager->nseTreeResetItem();
}

QStringList Profiler::buildExtensions()
{
    QStringList parameters;

    // NSE check
    if (m_ui->m_nseManager->getActiveNseScript().size()) {
        QString tmpListScript_("--script=");
        QString tmpListParam_("--script-args=");
        QString tmpList_;
        QString tmpListArgs_;

        // read nse category actived
        Q_FOREACH(const QString & token, m_ui->m_nseManager->getActiveNseScript()) {
            tmpList_.append(token);
            tmpList_.append(",");
        }

        // load nse manual script
        if (!m_ui->m_dialogUi->comboNseInv->lineEdit()->text().isEmpty()) {
            QStringList manualNse = m_ui->m_dialogUi->comboNseInv->lineEdit()->text().split(',');
            Q_FOREACH(const QString & token, manualNse) {
                tmpList_.append(token);
                tmpList_.append(",");
            }
        }

        if (tmpList_.size()) {
            tmpList_.remove(' ');
            tmpList_.resize(tmpList_.size() - 1);
            tmpListScript_.append(tmpList_);
        }

        if (!m_ui->m_dialogUi->comboNsePar->lineEdit()->text().isEmpty()) {
            QString argsClean = m_ui->m_dialogUi->comboNsePar->lineEdit()->text().remove('"');
            argsClean = argsClean.remove('\'');
            QStringList argsNse = argsClean.split(',');
            Q_FOREACH(const QString & token, argsNse) {
                tmpListArgs_.append(token);
                tmpListArgs_.append(",");
            }
        }

        if (tmpListArgs_.size()) {
            tmpListArgs_.remove(' ');
            tmpListArgs_.resize(tmpListArgs_.size() - 1);
            tmpListParam_.append(tmpListArgs_);
            parameters << tmpListParam_;
        }

        parameters << tmpListScript_;
    } // End NSE check

    switch (m_ui->m_dialogUi->comboScanTcp->currentIndex()) { //scan check
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

    switch (m_ui->m_dialogUi->comboScanNonTcp->currentIndex()) {
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

    if (m_ui->m_dialogUi->checkFtpBounce->isChecked() && !m_ui->m_dialogUi->bounceEdit->text().isEmpty()) {
        // FTP Bounce attack
        parameters << "-b";
        parameters << m_ui->m_dialogUi->bounceEdit->text();
    } else {
        m_ui->m_dialogUi->checkFtpBounce->setCheckState(Qt::Unchecked);
    }

    if (m_ui->m_dialogUi->checkIdleScan->isChecked() && !m_ui->m_dialogUi->lineIdleScan->text().isEmpty()) {
        // Idle scan
        parameters << "-sI";
        parameters << m_ui->m_dialogUi->lineIdleScan->text();
    } else {
        m_ui->m_dialogUi->checkIdleScan->setCheckState(Qt::Unchecked);
    }

    // Aggressive options
    if (m_ui->m_dialogUi->checkAggressiveOptions->isChecked()) {
        parameters << "-A";
    }
    // start option scan
    if (m_ui->m_dialogUi->rpcBox->isChecked()) {
        parameters << "-sR";
    }

    if (m_ui->m_dialogUi->versionBox->isChecked()) {
        parameters << "-sV";
    }

    if (m_ui->m_dialogUi->notpingBox->isChecked()) {
        parameters << "-Pn";
    }

    if (m_ui->m_dialogUi->checkOS->isChecked()) {
        parameters << "-O";
    }
    //end Extension

    switch (m_ui->m_dialogUi->portCombo->currentIndex()) {
        // port combo check
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
        if (!m_ui->m_dialogUi->portEdit->text().isEmpty()) {
            parameters << "-p";
            parameters << m_ui->m_dialogUi->portEdit->text();
        } else {
            QMessageBox::warning(m_ui, "NmapSI4", tr("No Ports Range (ex: 20-80)\n"), tr("Close"));
        }
    default:
        break;
    }


    if (m_ui->m_dialogUi->checkTcpPing->isChecked()) {
        // Discover options (tcp ack)
        QString tmpCommand;
        tmpCommand.append("-PA");
        tmpCommand.append(m_ui->m_dialogUi->lineTcpPing->text());
        parameters << tmpCommand;
    }

    if (m_ui->m_dialogUi->checkTcpSyn->isChecked()) {
        // Discover options (tcp syn)

        QString tmpCommand;
        tmpCommand.append("-PS");
        tmpCommand.append(m_ui->m_dialogUi->lineSynPing->text());
        parameters << tmpCommand;
    }

    if (m_ui->m_dialogUi->checkUdpPing->isChecked()) {
        // Discover options (tcp syn)
        QString tmpCommand;
        tmpCommand.append("-PU");
        if (!m_ui->m_dialogUi->lineUdpPing->text().isEmpty()) {
            tmpCommand.append(m_ui->m_dialogUi->lineUdpPing->text());
        }
        parameters << tmpCommand;
    }

    if (m_ui->m_dialogUi->checkProtoPing->isChecked()) {
        // IPProto ping (tcp syn)
        QString tmpCommand;
        tmpCommand.append("-PO");

        if (!m_ui->m_dialogUi->lineProtoPing->text().isEmpty()) {
            tmpCommand.append(m_ui->m_dialogUi->lineUdpPing->text());
        }

        parameters << tmpCommand;
    }

    if (m_ui->m_dialogUi->checkSctpPing->isChecked()) {
        // IPProto ping (tcp syn)
        QString tmpCommand;
        tmpCommand.append("-PY");

        if (!m_ui->m_dialogUi->lineSctpPing->text().isEmpty()) {
            tmpCommand.append(m_ui->m_dialogUi->lineSctpPing->text());
        }

        parameters << tmpCommand;
    }

    // Discover option
    if (m_ui->m_dialogUi->checkIcmpEcho->isChecked()) {
        parameters << "-PE";
    }

    if (m_ui->m_dialogUi->checkIcmpTimestamp->isChecked()) {
        parameters << "-PP";
    }

    if (m_ui->m_dialogUi->checkIcmpNetmask->isChecked()) {
        parameters << "-PM";
    }

    switch (m_ui->m_dialogUi->comboTiming->currentIndex()) {
        // port combo Timing
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

    switch (m_ui->m_dialogUi->comboDNSResolv->currentIndex()) {
        // port DNS resolv
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

    switch (m_ui->m_dialogUi->comboVerbosity->currentIndex()) {
        // port DNS resolv
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
    if (m_ui->m_dialogUi->checkOrdered->isChecked()) {
        parameters << "-r"; // Ordered Port
    }

    if (m_ui->m_dialogUi->checkIpv6->isChecked()) {
        parameters << "-6"; // Ipv6
    }

    if (m_ui->m_dialogUi->checkFrag->isChecked()) {
        parameters << "-f";
    }

    // traceroute
    if (m_ui->m_dialogUi->checkTraceroute->isChecked()) {
        parameters << "--traceroute";
    }

    // packet-trace
    if (m_ui->m_dialogUi->checkPacketTrace->isChecked()) {
        parameters << "--packet-trace";
    }

    if (m_ui->m_dialogUi->checkMaxRetries->isChecked()) {
        parameters << "--max-retries";
        if (!m_ui->m_dialogUi->lineMaxRetries->text().isEmpty()) {
            parameters << m_ui->m_dialogUi->lineMaxRetries->text();
        }
    }

    // Timing options
    if (m_ui->m_dialogUi->TcheckIpv4ttl->isChecked()) {
        parameters << "--ttl";
        parameters << m_ui->m_dialogUi->spinBoxIpv4ttl->text();
    }

    if (m_ui->m_dialogUi->TcheckMinPar->isChecked()) {
        parameters << "--min-parallelism";
        parameters << m_ui->m_dialogUi->TspinBoxMinP->text();
    }

    if (m_ui->m_dialogUi->TcheckMaxPar->isChecked()) {
        parameters << "--max-parallelism";
        parameters << m_ui->m_dialogUi->spinBoxMaxPar->text();
    }

    if (m_ui->m_dialogUi->TcheckInitRtt->isChecked()) {
        parameters << "--initial-rtt-timeout";
        parameters << m_ui->m_dialogUi->spinBoxInitRtt->text();
    }

    if (m_ui->m_dialogUi->TcheckMinRtt->isChecked()) {
        parameters << "--min-rtt-timeout";
        parameters << m_ui->m_dialogUi->spinBoxMinRtt->text();
    }

    if (m_ui->m_dialogUi->TcheckMaxRtt->isChecked()) {
        parameters << "--max-rtt-timeout";
        parameters << m_ui->m_dialogUi->spinBoxMaxRtt->text();
    }

    if (m_ui->m_dialogUi->TcheckHostTime->isChecked()) {
        parameters << "--host-timeout";
        parameters << m_ui->m_dialogUi->spinBoxHostTime->text();
    }

    if (m_ui->m_dialogUi->TcheckScanDelay->isChecked()) {
        parameters << "--scan-delay";
        parameters << m_ui->m_dialogUi->spinBoxScanDelay->text();
    }

    if (m_ui->m_dialogUi->TcheckScanDelayMax->isChecked()) {
        parameters << "--max-scan-delay";
        parameters << m_ui->m_dialogUi->spinBoxScanDelayMax->text();
    }

    //Options
    if (m_ui->m_dialogUi->checkBoxDevice->isChecked() && !m_ui->m_dialogUi->OlineDevice->text().isEmpty()) {
        // Discover options (tcp syn)
        parameters << "-e";
        parameters << m_ui->m_dialogUi->OlineDevice->text();
    } else {
        m_ui->m_dialogUi->checkBoxDevice->setCheckState(Qt::Unchecked);
    }

    if (m_ui->m_dialogUi->checkDecoy->isChecked()) {
        // Discover options (tcp syn)
        if (!m_ui->m_dialogUi->lineDecoy->text().isEmpty()) {
            parameters << "-D";
            parameters << m_ui->m_dialogUi->lineDecoy->text();

        } else {
            QMessageBox::warning(m_ui, "NmapSI4", tr("Please, first insert a Decoy\n"), tr("Disable Option"));
            m_ui->m_dialogUi->checkDecoy->setCheckState(Qt::Unchecked);
        }
    }

    if (m_ui->m_dialogUi->checkSpoof->isChecked()) {
        // Spoof options
        if (!m_ui->m_dialogUi->lineEditSpoof->text().isEmpty()) {
            parameters << "-S";
            parameters << m_ui->m_dialogUi->lineEditSpoof->text();

        } else {
            QMessageBox::warning(m_ui, "NmapSI4", tr("Please, insert spoof address\n"), tr("Disable Option"));
            m_ui->m_dialogUi->checkSpoof->setCheckState(Qt::Unchecked);
        }
    }

    if (m_ui->m_dialogUi->checkSourcePort->isChecked()) {
        // Spoof options
        if (!m_ui->m_dialogUi->lineSourcePort->text().isEmpty()) {
            parameters << "-g";
            parameters << m_ui->m_dialogUi->lineSourcePort->text();

        } else {
            QMessageBox::warning(m_ui, "NmapSI4", tr("Please, insert port address\n"), tr("Disable Option"));
            m_ui->m_dialogUi->checkSourcePort->setCheckState(Qt::Unchecked);
        }
    }

    return parameters;
}
