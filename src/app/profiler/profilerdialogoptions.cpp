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

#include "profilerdialog.h"

void ProfilerManager::restoreValuesFromProfile(const QStringList parameters)
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
                        comboNsePar->lineEdit()->setText(values);
                    }
                    else if (parameters[index].startsWith(QLatin1String("--script=")))
                    {
                        QString values = parameters[index];
                        values.remove("--script=");

                        QStringList scripts = values.split(',',QString::SkipEmptyParts);
                        foreach (const QString& script, scripts)
                        {
                            if (m_nseManager->nseTreeActiveSingleScript(script))
                            {
                                scripts.removeAt(script.indexOf(script));
                            }
                        }

                        if (scripts.size())
                        {
                            comboNseInv->lineEdit()->setText(scripts.join(","));
                        }
                    }
                }
            }
        } // end first if
    } // end for
}


void ProfilerManager::loadDefaultHash()
{
    preLoadOptionsCheckBox();
    preLoadOptionsCombo();
}

void ProfilerManager::preLoadOptionsCombo()
{
    QPair<QComboBox*,int> comboDefault;

    // TCP Scan
    comboDefault.first = comboScanTcp;

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
    comboDefault.first = comboScanNonTcp;

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
    comboDefault.first = comboTiming;

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
    comboDefault.first = portCombo;

    comboDefault.second = 1;
    preLoadComboList.insert("-p-",comboDefault);
    comboDefault.second = 2;
    preLoadComboList.insert("-F",comboDefault);
    comboDefault.second = 3;
    //parameters is request
    preLoadComboList.insert("-p",comboDefault);
    lineEditList.insert("-p",portEdit);

    // dns Resolution
    comboDefault.first = comboDNSResolv;

    comboDefault.second = 1;
    preLoadComboList.insert("-R",comboDefault);
    comboDefault.second = 2;
    preLoadComboList.insert("-n",comboDefault);

    // verbosity level
    comboDefault.first = comboVerbosity;

    comboDefault.second = 1;
    preLoadComboList.insert("-v",comboDefault);
    comboDefault.second = 2;
    preLoadComboList.insert("-vv",comboDefault);
    comboDefault.second = 3;
    preLoadComboList.insert("-d",comboDefault);
    comboDefault.second = 4;
    preLoadComboList.insert("-d2",comboDefault);
}

void ProfilerManager::preLoadOptionsCheckBox()
{

    QPair<QCheckBox*,QString> comboDefault;

    // scan stack
    // ftp bounce
    comboDefault.first = checkFtpBounce;
    comboDefault.second = "";
    preLoadCheckBoxList.insert("-b", comboDefault);
    lineEditList.insert("-b",bounceEdit);

    // idle scan
    comboDefault.first = checkIdleScan;
    comboDefault.second = "";
    preLoadCheckBoxList.insert("-sI", comboDefault);
    lineEditList.insert("-sI",lineIdleScan);

    // Aggressive scan
    comboDefault.first = checkAggressiveOptions;
    comboDefault.second = "none";
    preLoadCheckBoxList.insert("-A", comboDefault);

    // RPC scan
    comboDefault.first = rpcBox;
    comboDefault.second = "none";
    preLoadCheckBoxList.insert("-sR", comboDefault);

    // OS Detection
    comboDefault.first = checkOS;
    comboDefault.second = "none";
    preLoadCheckBoxList.insert("-O", comboDefault);

    // Version Probe
    comboDefault.first = versionBox;
    comboDefault.second = "none";
    preLoadCheckBoxList.insert("-sV", comboDefault);

    // Ipv6
    comboDefault.first = checkIpv6;
    comboDefault.second = "none";
    preLoadCheckBoxList.insert("-6", comboDefault);

    // end scan stack

    // start ping stack

    // Dont'ping
    comboDefault.first = notpingBox;
    comboDefault.second = "none";
    preLoadCheckBoxList.insert("-P0", comboDefault);

    // ICMP echo
    comboDefault.first = checkIcmpEcho;
    comboDefault.second = "none";
    preLoadCheckBoxList.insert("-PE", comboDefault);

    // ICMP timestamp
    comboDefault.first = checkIcmpTimestamp;
    comboDefault.second = "none";
    preLoadCheckBoxList.insert("-PP", comboDefault);

    // ICMP netmask
    comboDefault.first = checkIcmpNetmask;
    comboDefault.second = "none";
    preLoadCheckBoxList.insert("-PM", comboDefault);

    //TCP ack ping
    comboDefault.first = checkTcpPing;
    comboDefault.second = "21,23,80,3389";
    preLoadCheckBoxList.insert("-PA", comboDefault);
    lineEditList.insert("-PA",lineTcpPing);

    //TCP syn ping
    comboDefault.first = checkTcpSyn;
    comboDefault.second = "22,25,80";
    preLoadCheckBoxList.insert("-PS", comboDefault);
    lineEditList.insert("-PS",lineSynPing);

    // UDP Ping
    comboDefault.first = checkUdpPing;
    comboDefault.second = "";
    preLoadCheckBoxList.insert("-PU", comboDefault);
    lineEditList.insert("-PU",lineUdpPing);

    // IP proto
    comboDefault.first = checkProtoPing;
    comboDefault.second = "";
    preLoadCheckBoxList.insert("-PO", comboDefault);
    lineEditList.insert("-PO",lineProtoPing);

    //SCTP init ping
    comboDefault.first = checkSctpPing;
    comboDefault.second = "";
    preLoadCheckBoxList.insert("-PY", comboDefault);
    lineEditList.insert("-PY",lineSctpPing);

    // end ping stack
    // start timing stack

    //Min outstanding
    comboDefault.first = TcheckMinPar;
    comboDefault.second = '1';
    preLoadCheckBoxList.insert("--min-parallelism", comboDefault);
    spinBoxList.insert("--min-parallelism",TspinBoxMinP);

    //Min outstanding
    comboDefault.first = TcheckMaxPar;
    comboDefault.second = '1';
    preLoadCheckBoxList.insert("--max-parallelism", comboDefault);
    spinBoxList.insert("--max-parallelism",spinBoxMaxPar);

    //Max time
    comboDefault.first = TcheckHostTime;
    comboDefault.second = "6000";
    preLoadCheckBoxList.insert("--host-timeout", comboDefault);
    spinBoxList.insert("--host-timeout",spinBoxHostTime);

    //initial probe
    comboDefault.first = TcheckInitRtt;
    comboDefault.second = "6000";
    preLoadCheckBoxList.insert("--initial-rtt-timeout", comboDefault);
    spinBoxList.insert("--initial-rtt-timeout",spinBoxInitRtt);

    //min probe
    comboDefault.first = TcheckMinRtt;
    comboDefault.second = "6000";
    preLoadCheckBoxList.insert("--min-rtt-timeout", comboDefault);
    spinBoxList.insert("--min-rtt-timeout",spinBoxMinRtt);

    //max probe
    comboDefault.first = TcheckMaxRtt;
    comboDefault.second = "6000";
    preLoadCheckBoxList.insert("--max-rtt-timeout", comboDefault);
    spinBoxList.insert("--max-rtt-timeout",spinBoxMaxRtt);

    //scan delay
    comboDefault.first = TcheckScanDelay;
    comboDefault.second = "6000";
    preLoadCheckBoxList.insert("--scan-delay", comboDefault);
    spinBoxList.insert("--scan-delay",spinBoxScanDelay);

    //Max scan
    comboDefault.first = TcheckScanDelayMax;
    comboDefault.second = "6000";
    preLoadCheckBoxList.insert("--max-scan-delay", comboDefault);
    spinBoxList.insert("--max-scan-delay",spinBoxScanDelayMax);

    // end timing stack

    // start options

    // network interface
    comboDefault.first = checkBoxDevice;
    comboDefault.second = "";
    preLoadCheckBoxList.insert("-e", comboDefault);
    lineEditList.insert("-e",OlineDevice);

    // source spoof ip
    comboDefault.first = checkSpoof;
    comboDefault.second = "";
    preLoadCheckBoxList.insert("-S", comboDefault);
    lineEditList.insert("-S",lineEditSpoof);

    // set decoy
    comboDefault.first = checkDecoy;
    comboDefault.second = "";
    preLoadCheckBoxList.insert("-D", comboDefault);
    lineEditList.insert("-D",lineDecoy);

    // source spoof port
    comboDefault.first = checkSourcePort;
    comboDefault.second = "";
    preLoadCheckBoxList.insert("-g", comboDefault);
    lineEditList.insert("-g",lineSourcePort);

    // ipv4 ttl
    comboDefault.first = TcheckIpv4ttl;
    comboDefault.second = "127";
    preLoadCheckBoxList.insert("--ttl", comboDefault);
    spinBoxList.insert("--ttl",spinBoxIpv4ttl);

    // max retries
    comboDefault.first = checkMaxRetries;
    comboDefault.second = "";
    preLoadCheckBoxList.insert("--max-retries", comboDefault);
    lineEditList.insert("--max-retries",lineMaxRetries);

    // ordered scanned
    comboDefault.first = checkOrdered;
    comboDefault.second = "none";
    preLoadCheckBoxList.insert("-r", comboDefault);

    // fragment IP
    comboDefault.first = checkFrag;
    comboDefault.second = "none";
    preLoadCheckBoxList.insert("-f", comboDefault);

    // traceroute
    comboDefault.first = checkTraceroute;
    comboDefault.second = "none";
    preLoadCheckBoxList.insert("--traceroute", comboDefault);

    // end options
}
