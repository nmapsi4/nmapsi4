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

#include "../../mainwin.h"

void nmapClass::nmapParser(const QString hostCheck, QByteArray Byte1, QByteArray Byte2)
{
    QString * StdoutStr;
    QString * StderrorStr;
    parserObj *elemObj = new parserObj();

    int open_port = 0;
    int close_port = 0;
    int filtered_port = 0;
    int error_count = 0;
    int ItemNumber = 0;
    int pos;

#ifndef PARSER_NO_DEBUG
    qDebug() << "Host Checked:::: " << hostCheck;
#endif
    delMonitorHost(scanMonitor, hostCheck);

    listClearFlag = false; // the listScan is not empty

    StdoutStr = new QString(Byte1); // read std buffer
    StderrorStr = new QString(Byte2); // read error buffer

    progressScan->setValue(75);
    
    QTextStream stream(StdoutStr);
    QString tmp, generalBuffer_, scanBuffer, bufferInfo,bufferTraceroot,bufferNSS;
    QRegExp rxT_("^\\d\\d?");

    generalBuffer_.append(hostCheck);

    while (!stream.atEnd()) {
        tmp = stream.readLine();

        if ((rxT_.indexIn(tmp) != -1) &&
                (tmp.contains("open")
                || tmp.contains("closed")
                || tmp.contains("filtered")
                || tmp.contains("unfiltered"))
                && !tmp.contains("Not shown:")
                && !tmp.contains("Discovered")) {

            scanBuffer.append(tmp);
            scanBuffer.append("\n");
        }

        if ((tmp.contains("MAC")
                || tmp.contains("Running:")
                || tmp.contains("Running")
                || tmp.contains("OS details:")
                || tmp.contains("Aggressive OS guesses:")
                || tmp.contains("Device type:")
                || tmp.contains("Uptime:")
                || tmp.contains("TCP Sequence Prediction:")
                || tmp.contains("IPID Sequence Generation:")
                || tmp.contains("IP ID Sequence Generation:")
                || (tmp.contains("Service Info:") && tmp.compare(generalBuffer_))
                || tmp.contains("Initiating Ping ")
                || tmp.contains("Completed Ping ")
                || tmp.contains("Network Distance:")
                || tmp.contains("Note:")
                || tmp.contains("Nmap done:")
                || tmp.startsWith("Hosts")
                || tmp.startsWith("Host"))
                && !tmp.startsWith("|")
           ) {
            bufferInfo.append(tmp);
            bufferInfo.append("\n");
        }

        if(tmp.startsWith("Uptime")) {
            QString uptime;
            uptime.clear();
            uptime.append(tmp);
            uptime.remove("Uptime guess:");
            elemObj->setUptime(uptime);
        }

        if(tmp.startsWith("TCP Sequence Prediction:")) {
            QString sequence_(tmp);
            sequence_.remove("TCP Sequence Prediction: ");
            elemObj->setTcpSequence(sequence_);
        }

        if(tmp.startsWith("Running:")) {
            QString running_(tmp);
            running_.remove("Running: ");
            elemObj->setRunning(running_);
        }

        if(tmp.startsWith("Device type:")) {
            QString device_(tmp);
            device_.remove("Device type: ");
            elemObj->setDeviceType(device_);
        }

        if(tmp.startsWith("|")) {
            QString tmpClean(tmp);
            if(tmpClean.startsWith("|")) {
                tmpClean.remove("|");
            }
            if(tmpClean.startsWith("_")) {
                tmpClean.remove("_");
            }

            while(tmpClean.startsWith(" ")) {
                pos = tmpClean.indexOf(" ");
                if(pos == 0) {
                    tmpClean.remove(pos,1);
                }
            }

            bufferNSS.append(tmpClean);
            bufferNSS.append("\n");
        }

        if((rxT_.indexIn(tmp) != -1) && (!tmp.contains("/"))) {
#ifndef PARSER_NO_DEBUG
            qDebug() << "Match String:: " << tmp;
#endif
            bufferTraceroot.append(tmp);
            bufferTraceroot.append("\n");
        }

    }

    QTreeWidgetItem *root2 = new QTreeWidgetItem(listScan);
    itemList.push_front(root2);

    QTreeWidgetItem *error = new QTreeWidgetItem(listScanError);
    itemList.push_front(error);

    QTreeWidgetItem *mainTreeE = new QTreeWidgetItem(treeMain);
    mainTreeElem.push_front(mainTreeE);

    // check for log file
    QTextStream *out = NULL;

    if (PFile) {
        out = new QTextStream(PFile);
        QString nmap_command;
        nmap_command.append("\n==LogStart: ");
        nmap_command.append("\nnmap ");
        if(!frameAdv-isVisible()) {
            nmap_command.append(check_extensions().join(" "));
        } else {
            nmap_command.append(comboAdv->lineEdit()->text());
        }
        nmap_command.append(hostCheck); // write host target in the log
        *out << nmap_command << endl << endl;
    }

    int tmpBox = SWscan->currentIndex();
 
    switch(tmpBox) {
      case 0:	
        Bnss->setIcon(QIcon(QString::fromUtf8(":/images/images/reload.png")));
        Binfo->setIcon(QIcon(QString::fromUtf8(":/images/images/reload.png")));
	break;
      case 1:
        Bdetails->setIcon(QIcon(QString::fromUtf8(":/images/images/reload.png")));
        Binfo->setIcon(QIcon(QString::fromUtf8(":/images/images/reload.png")));
	break;
      case 2:
        Bdetails->setIcon(QIcon(QString::fromUtf8(":/images/images/reload.png")));
        Bnss->setIcon(QIcon(QString::fromUtf8(":/images/images/reload.png")));
        Binfo->setIcon(QIcon(QString::fromUtf8(":/images/images/reload.png")));
	break;
    }

    root2->setIcon(0, QIcon(QString::fromUtf8(":/images/images/book.png")));
    error->setIcon(0, QIcon(QString::fromUtf8(":/images/images/messagebox_critical.png")));
    mainTreeE->setIcon(0, QIcon(QString::fromUtf8(":/images/images/network_local.png")));

    QString tmp_mess("");
    QString tmp_mess2("");
    QString tmp_host("");

    if (!generalBuffer_.isEmpty()) { // Host line scan
        root2->setText(0, generalBuffer_);
        error->setText(0, generalBuffer_);
        //QFont rootFont = root->font(0);
        //rootFont.setWeight(QFont::Normal);
        tmp_host.append(generalBuffer_);
        tmp_host.append("\n");
        tmp_host.append(QDateTime::currentDateTime().toString("ddd MMM d yy - hh:mm:ss"));
        mainTreeE->setText(0, tmp_host);
        tmp_mess.append(generalBuffer_);
        tmp_mess2.append(generalBuffer_);
        if ((PFile) && (!verboseLog)) *out << generalBuffer_ << endl;
    } else {
        root2->setText(0, hostCheck);
        error->setText(0, hostCheck);
        tmp_host.append(hostCheck);
        tmp_host.append("\n");
        tmp_host.append(QDateTime::currentDateTime().toString("ddd MMM d yy - hh:mm:ss"));
        mainTreeE->setText(0, tmp_host);
        tmp_mess.append(hostCheck);
        tmp_mess2.append(hostCheck);
        if ((PFile) && (!verboseLog)) *out << hostCheck << endl;
    }

    QTextStream scanBufferToStream_(&scanBuffer); // QString to QtextStream (scan Tree)
    QString scanBufferToStream_line;

    // check for scan result
    if (!scanBufferToStream_.atEnd()) {
        while (!scanBufferToStream_.atEnd()) {
            scanBufferToStream_line = scanBufferToStream_.readLine();
            if (scanBufferToStream_line.contains("open") || scanBufferToStream_line.contains("filtered")
                    || scanBufferToStream_line.contains("unfiltered")) {

                if (scanBufferToStream_line.contains("filtered") || scanBufferToStream_line.contains("unfiltered")) {
                    elemObj->setPortFiltered(scanBufferToStream_line);
                    filtered_port++;
                } else {
                    elemObj->setPortOpen(scanBufferToStream_line);
                    open_port++;
                }
            } else {
                elemObj->setPortClose(scanBufferToStream_line);
                close_port++;
            }


            if (!scanBufferToStream_line.isEmpty()) {
                QString tmpStr = scanBufferToStream_line;
                QStringList lStr = tmpStr.split(" ", QString::SkipEmptyParts);
#ifndef PARSER_NO_DEBUG
                qDebug() << "Nmapsi4/parser:: --> Token:: " << lStr;
#endif

                elemObj->setServices(lStr[2]); // Obj Services
                elemObj->setPortServices(lStr[0]);

                int index = 3;
                QString str;
                for(int index=0; index < lStr.size(); index++) {
                    if(lStr[index].contains("(") || lStr[index].contains(")")
                                                 || lStr[index].contains("version")
                                                 || lStr[index].contains("protocol")) {
                        lStr.removeAt(index);
                        index--;
                    }
                }
                // Description
                if (lStr.size() == 3) {
                    elemObj->setDescriptionServices(tmpStr);
                } else {
                    while (index < lStr.size()) {
                        str.append(lStr[index]);
                        str.append(" ");
                        index++;

                    }

                    QStringList tmpToken = str.split(" ");
                    QString token;
                    if (tmpToken.size() <= 2) {
                        token.append(tmpToken[0]);
                        token.append(" ");
                        token.append(tmpToken[1]);
                    } else {
                        token.append(tmpToken[0]);
                        token.append(" ");
                        token.append(tmpToken[1]);
                        token.append(" ");
                        token.append(tmpToken[2]);
                    }

                    // loading services for vulnerability plugin
                    if (token.contains("-")) {
                        if (comboVuln->findText(token.leftRef(token.indexOf("-")).toString(),
                                                Qt::MatchExactly) == -1) {
                            comboVuln->addItem(token.leftRef(token.indexOf("-")).toString());
                        }
                    } else if (comboVuln->findText(token, Qt::MatchExactly) == -1) {
                        comboVuln->addItem(token);
                    }

                    elemObj->setDescriptionServices(str);

                }

                // FIX log
                if ((PFile) && (!verboseLog)) {
                    *out << str
                            << " ("
                            << lStr[2]
                            << " - "
                            << lStr[0]
                            << ")"
                            << endl;
                }
            }
            ItemNumber++;
        }
    } else {
        mainTreeE->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit_noresult.png")));
    }

    QTextStream bufferInfoStream(&bufferInfo); // QString to QtextStrem (scan Tree)
    QString bufferInfoStream_line;

    // check for Host informations
    if (!bufferInfoStream.atEnd()) {
        while (!bufferInfoStream.atEnd()) {
            bufferInfoStream_line = bufferInfoStream.readLine();
            // Check OS String
            if (bufferInfoStream_line.contains("OS")) {
                checkViewOS(bufferInfoStream_line,mainTreeE);
            }

            if (!bufferInfoStream_line.isEmpty()) {
                elemObj->setScanInfo(bufferInfoStream_line);
                if (PFile && !verboseLog)
                    *out << bufferInfoStream_line << endl;
            }
        }
    } else { // insert message for no info
        //infoItem->setText(0, tmp_mess2);
        //infoItem->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit_noresult.png")));
    }

    QTextStream bufferTraceStream(&bufferTraceroot); // Traceroute buffer
    QString bufferTraceStream_line;

    // check for traceroute scan informations
    if (!bufferTraceStream.atEnd()) {
        while (!bufferTraceStream.atEnd()) {
            bufferTraceStream_line = bufferTraceStream.readLine();

            if (!bufferTraceStream_line.isEmpty()) {
                if(!bufferTraceStream_line.contains("guessing hop")) {

                    elemObj->setTraceRouteInfo(bufferTraceStream_line);

                    if (PFile && !verboseLog) {
                        *out << bufferTraceStream_line << endl;
                    }
                }
            }
        }
    } else { // insert message for no info
        //infoTraceroot->setText(0, tmp_mess2);
        //infoTraceroot->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit_noresult.png")));
    }

    QTextStream bufferNssStream(&bufferNSS); // NSS
    QString bufferNssStream_line("");

    // check for NSS scan informations
    if (!bufferNssStream.atEnd()) {
        while (!bufferNssStream.atEnd()) {
            bufferNssStream_line = bufferNssStream.readLine();

            if (!bufferNssStream_line.isEmpty()) {
                elemObj->setNssInfo(bufferNssStream_line);
                if (PFile && !verboseLog) {
                    *out << bufferNssStream_line << endl;
                }
            }
        }
    } else { // insert message for no info
        //infoNSS->setText(0, tmp_mess2);
        //infoNSS->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit_noresult.png")));
    }



    if (ItemNumber) {
        // TODO:: FIX counter
        //QString tmp_buffer = root->text(0);
        //root->setText(1, QString("%1").arg(open_port));
        //root->setText(2, QString("%1").arg(close_port));
        //root->setText(3, QString("%1").arg(filtered_port));
    }

    actionClear_History->setEnabled(true);

    QTextStream b_log(StdoutStr);
    QString blog_line;

    while (!b_log.atEnd()) {
        blog_line = b_log.readLine();
        if(!blog_line.isEmpty()) {
           QTreeWidgetItem *item_root2 = new QTreeWidgetItem(root2); // append scan log
           itemList.push_front(item_root2); // reference to address

           item_root2->setText(0, blog_line);

           if ((PFile) && (verboseLog)) {
            *out << blog_line << "\n";
           }
        }
    }
    *out << "==LogEnd\n";
    if (PFile) delete out;

    QTextStream b_error(StderrorStr);
    QString error_line;

    if (!b_error.atEnd()) { // check for no error
        while (!b_error.atEnd()) { // print error buffer informations
            QTreeWidgetItem *item_error = new QTreeWidgetItem(error); // item_error memory allocation
            itemList.push_front(item_error); // reference to address
            error_line = b_error.readLine();
            item_error->setText(0, error_line);
            error_count++;
        }

    } else {
        QString tmp_mess = error->text(0);
        tmp_mess.append(tr("\n(No Errors or Warnings)"));
        error->setText(0, tmp_mess);
    }

    if (error_count) {
        QString tmp_buffer_error = error->text(0);
        tmp_buffer_error.append(tr("\n(Report: "));
        tmp_buffer_error.append(tr("Errors or Warnings: "));
        tmp_buffer_error.append(QString("%1").arg(error_count));
        tmp_buffer_error.append(" )");
        error->setText(0, tmp_buffer_error);
    }


    delete StdoutStr;
    delete StderrorStr;

    if(!monitorElemHost.size()) {
        emit killScan();
        qDeleteAll(scanPointerList);
        scanPointerList.clear();
    }

    Byte1.clear();
    Byte2.clear();

    progressScan->setValue(85);
    if(!scanCounter) {
        progressScan->setValue(100);
    } else {
        progressScan->setValue(55);
    }

    action_Scan_menu->setEnabled(true);
    action_Scan_2->setEnabled(true);
    hostEdit->setEnabled(true);
    action_Save_As->setEnabled(true);
    actionSave_As_Menu->setEnabled(true);
    if (!logSessionFile.isEmpty()) {
	actionSave->setEnabled(true);
	actionSave_Menu->setEnabled(true);
    }

    parserObjList.append(elemObj);
}
