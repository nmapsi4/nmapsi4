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

void nmapClass::nmapParser(const QString hostCheck, QByteArray Byte1, QByteArray Byte2)
{
    QString * StdoutStr;
    QString * StderrorStr;
    QString title = "NmapSI4 ";
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
    this->setWindowTitle(title.append("(75%)"));
    
    QTextStream stream(StdoutStr);
    QString tmp, buffer, buffer2, bufferInfo,bufferTraceroot,bufferNSS;
    QRegExp rxT_("^\\d\\d?");

    buffer.append(hostCheck);

    while (!stream.atEnd()) {
        tmp = stream.readLine();

        if ((rxT_.indexIn(tmp) != -1) &&
                (tmp.contains("open")
                || tmp.contains("closed")
                || tmp.contains("filtered")
                || tmp.contains("unfiltered"))
                && !tmp.contains("Not shown:")
                && !tmp.contains("Discovered")) {

            buffer2.append(tmp);
            buffer2.append("\n");
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
                || (tmp.contains("Service Info:") && tmp.compare(buffer))
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

    QTreeWidgetItem *root = new QTreeWidgetItem(listWscan);
    itemList.push_front(root);

    QTreeWidgetItem *root2 = new QTreeWidgetItem(listScan);
    itemList.push_front(root2);

    QTreeWidgetItem *error = new QTreeWidgetItem(listScanError);
    itemList.push_front(error);

    QTreeWidgetItem *infoItem = new QTreeWidgetItem(treeWinfo);
    itemList.push_front(infoItem);

    QTreeWidgetItem *infoTraceroot = new QTreeWidgetItem(treeTraceroot);
    itemList.push_front(infoTraceroot);

    QTreeWidgetItem *infoNSS = new QTreeWidgetItem(treeNSS);
    itemList.push_front(infoNSS);

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


    root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit_result.png")));
    root->setTextAlignment(1, Qt::AlignCenter);
    root->setTextAlignment(2, Qt::AlignCenter);
    root->setTextAlignment(3, Qt::AlignCenter);
    root->setTextAlignment(4, Qt::AlignCenter);

    root2->setIcon(0, QIcon(QString::fromUtf8(":/images/images/book.png")));
    error->setIcon(0, QIcon(QString::fromUtf8(":/images/images/messagebox_critical.png")));
    infoItem->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit_result.png")));
    infoTraceroot->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit_result.png")));
    infoNSS->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit_result.png")));
    mainTreeE->setIcon(0, QIcon(QString::fromUtf8(":/images/images/network_local.png")));

    if (!buffer.isEmpty()) { // Host line scan
        QFont rootFont = root->font(0);
        rootFont.setWeight(QFont::Normal);
        root->setFont(0, rootFont);
        root->setText(0, buffer);
        root2->setText(0, buffer);
        error->setText(0, buffer);
        infoItem->setText(0, buffer);
        infoTraceroot->setText(0, buffer);
        infoNSS->setText(0, buffer);
        mainTreeE->setText(0, buffer);
        if ((PFile) && (!verboseLog)) *out << root->text(0) << endl;
    } else {
        QFont rootFont = root->font(0);
        rootFont.setWeight(QFont::Normal);
        root->setFont(0, rootFont);
        root->setText(0, hostCheck);
        root2->setText(0, hostCheck);
        error->setText(0, hostCheck);
        infoItem->setText(0, hostCheck);
        infoTraceroot->setText(0, hostCheck);
        infoNSS->setText(0, hostCheck);
        mainTreeE->setText(0, hostCheck);
        if ((PFile) && (!verboseLog)) *out << root->text(0) << endl;
    }

    QTextStream b2(&buffer2); // QString to QtextStream (scan Tree)
    QString b2_line;
    QString tmp_mess = root->text(0);
    QString tmp_mess2 = root->text(0);

    if (!b2.atEnd()) { // check for scan informations
        while (!b2.atEnd()) {
            b2_line = b2.readLine();
            QTreeWidgetItem *item2 = new QTreeWidgetItem(root);
            itemList.push_front(item2); // save item address in QList

            if (b2_line.contains("open") || b2_line.contains("filtered")
                    || b2_line.contains("unfiltered")) {

                if (b2_line.contains("filtered") || b2_line.contains("unfiltered")) {
                    item2->setSizeHint(0, QSize::QSize(22, 22));
                    item2->setIcon(0, QIcon(QString::fromUtf8(":/images/images/flag_yellow.png")));
                    filtered_port++;
                } else {
                    item2->setSizeHint(0, QSize::QSize(22, 22));
                    item2->setIcon(0, QIcon(QString::fromUtf8(":/images/images/flag_green.png")));
                    open_port++;
                }
            } else {
                item2->setSizeHint(0, QSize::QSize(22, 22));
                item2->setIcon(0, QIcon(QString::fromUtf8(":/images/images/flag_red.png")));
                close_port++;
            }


            if (!b2_line.isEmpty()) {
                QString tmpStr = b2_line;
                QStringList lStr = tmpStr.split(" ", QString::SkipEmptyParts);
#ifndef PARSER_NO_DEBUG
                qDebug() << "Nmapsi4/parser:: --> Token:: " << lStr;
#endif

                if (lStr[1].contains("open")) {
                    item2->setText(1, lStr[0]);
                    item2->setToolTip(1, lStr[0]);
                    item2->setForeground(1, QBrush(QColor(0, 0, 255, 127)));
                } else if (lStr[1].contains("filtered") || lStr[1].contains("unfiltered")) {
                    item2->setText(3, lStr[0]);
                    item2->setToolTip(3, lStr[0]);
                } else {
                    item2->setText(2, lStr[0]);
                    item2->setToolTip(2, lStr[0]);
                    item2->setForeground(2, QBrush(QColor(255, 0, 0, 127)));
                }

                item2->setText(4, lStr[2]);
                item2->setToolTip(4, lStr[2]);
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
                QString noDes = tr("No description");
                if (lStr.size() == 3) {
                    item2->setText(0, noDes);
                    elemObj->setDescriptionServices(noDes);
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

                    if (token.contains("-")) {
                        if (comboVuln->findText(token.leftRef(token.indexOf("-")).toString(), Qt::MatchExactly) == -1)
                            comboVuln->addItem(token.leftRef(token.indexOf("-")).toString());
                    } else if (comboVuln->findText(token, Qt::MatchExactly) == -1)
                        comboVuln->addItem(token);

                    item2->setText(0, str);
                    item2->setToolTip(0, str);
                    elemObj->setDescriptionServices(str);

                }

                if ((PFile) && (!verboseLog)) {
                    *out << item2->text(0)
                            << " ("
                            << item2->text(4)
                            << " - "
                            << item2->text(1)
                            << item2->text(2)
                            << item2->text(3)
                            << ")"
                            << endl;
                }
            } else
                item2->setText(0, tr("No Ports"));

            ItemNumber++;

        }
    } else { // insert message for no info
        root->setText(0, tmp_mess);
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit_noresult.png")));
    }

    QTextStream b3(&bufferInfo); // QString to QtextStrem (scan Tree)
    QString b3_line;

    if (!b3.atEnd()) { // check for scan informations
        while (!b3.atEnd()) {
            b3_line = b3.readLine();

            checkViewOS(b3_line,root);

            QTreeWidgetItem *infoItemObj = new QTreeWidgetItem(infoItem);
            itemList.push_front(infoItemObj); // reference to address

            if (!b3_line.isEmpty()) {
                infoItemObj->setSizeHint(0, QSize::QSize(22, 22));
                infoItemObj->setIcon(0, QIcon(QString::fromUtf8(":/images/images/messagebox_info.png")));
                infoItemObj->setText(0, b3_line);
                infoItemObj->setToolTip(0, b3_line); // field information
                if ((PFile) && (!verboseLog)) *out << infoItemObj->text(0) << endl;
            } else
                infoItemObj->setText(0, tr("No Info"));
        }
    } else { // insert message for no info
        infoItem->setText(0, tmp_mess2);
        infoItem->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit_noresult.png")));
    }

    QTextStream bT(&bufferTraceroot); // Traceroute buffer
    QString bT_line;
    QTreeWidgetItem *infoTracerootObj = NULL;

    if (!bT.atEnd()) { // check for traceroute scan informations
        while (!bT.atEnd()) {
            bT_line = bT.readLine();

            if (!bT_line.isEmpty()) {
                if(!bT_line.contains("guessing hop")) {
                    infoTracerootObj = new QTreeWidgetItem(infoTraceroot);
                    itemList.push_front(infoTracerootObj);
                    QStringList tmpToken = bT_line.split(" ");

                    // split traceroute -------------------------------------------------
                    tmpToken.removeAll("");

                    // MS windows check for ms string
                    if(tmpToken.size() == 5) {
                        if(tmpToken[2].size() < 4) { // minimun dns lenght
                            tmpToken.removeAt(2);
                        }
                    }

                    if(tmpToken.size() == 4) {
                        if(tmpToken[2].size() < 4) { // minimun dns lenght
                            tmpToken.removeAt(2);
                        } else {
                            tmpToken[3].remove("(");
                            tmpToken[3].remove(")");
                        }
                    }
#ifndef PARSER_NO_DEBUG
                    qDebug() << "DEBUG::TracerouteSplit:: " << tmpToken.size();
                    for(int index=0; index < tmpToken.size(); index++) {
                        qDebug() << "DEBUG::TracerouteSplit:: " << tmpToken[index];

                    }
#endif

                    if(tmpToken.size() == 4) {
                        infoTracerootObj->setText(0, tmpToken[0]);
                        infoTracerootObj->setText(1, tmpToken[1]);
                        infoTracerootObj->setText(3, tmpToken[2]);
                        infoTracerootObj->setText(2, tmpToken[3]);

                    } else if(tmpToken.size() == 3) {
                        infoTracerootObj->setText(0, tmpToken[0]);
                        infoTracerootObj->setText(1, tmpToken[1]);
                        infoTracerootObj->setText(2, tmpToken[2]);
                        infoTracerootObj->setText(3, "no DNS");
                        infoTracerootObj->setForeground(3, QBrush(QColor(255, 0, 0, 127)));
                    } else {
                        infoTracerootObj->setText(0, bT_line);
                        infoTracerootObj->setToolTip(0, bT_line);
                    }
                    // ------------------------------------------------------------------------

                    infoTracerootObj->setSizeHint(0, QSize::QSize(22, 22));
                    infoTracerootObj->setIcon(0, QIcon(QString::fromUtf8(":/images/images/traceroute.png")));
                    if ((PFile) && (!verboseLog)) *out << infoTracerootObj->text(0) << endl;
                }
            } else {
                if(infoTracerootObj) {
                   infoTracerootObj->setText(0, tr("No Info"));
               }
            }
        }
    } else { // insert message for no info
        infoTraceroot->setText(0, tmp_mess2);
        infoTraceroot->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit_noresult.png")));
    }

    QTextStream bNSS(&bufferNSS); // NSS
    QString bNSS_line;
    QTreeWidgetItem *infoNSSObj = NULL;

    if (!bNSS.atEnd()) { //
        while (!bNSS.atEnd()) {
            bNSS_line = bNSS.readLine();

            if (!bNSS_line.isEmpty()) {
                infoNSSObj = new QTreeWidgetItem(infoNSS);
                itemList.push_front(infoNSSObj);

                infoNSSObj->setSizeHint(0, QSize::QSize(22, 22));
                infoNSSObj->setIcon(0, QIcon(QString::fromUtf8(":/images/images/traceroute.png")));
                infoNSSObj->setText(0, bNSS_line);
                infoNSSObj->setToolTip(0, bNSS_line); // field information
                if ((PFile) && (!verboseLog)) *out << infoNSSObj->text(0) << endl;
            } else {
                if(infoNSSObj) {
                   infoNSSObj->setText(0, tr("No Info"));
                }
            }
        }
    } else { // insert message for no info
        infoNSS->setText(0, tmp_mess2);
        infoNSS->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit_noresult.png")));
    }



    if (ItemNumber) {
        QString tmp_buffer = root->text(0);
        root->setText(1, QString("%1").arg(open_port));
        root->setText(2, QString("%1").arg(close_port));
        root->setText(3, QString("%1").arg(filtered_port));
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
    setWindowTitle(title.replace("(75%)", "(85%)"));
    progressScan->setValue(100);
    setWindowTitle(title.replace("(85%)", "(100%) [*]"));

    action_Scan_menu->setEnabled(true);
    action_Scan_2->setEnabled(true);
    hostEdit->setEnabled(true);
    action_Save_As->setEnabled(true);
    actionSave_As_Menu->setEnabled(true);
    if (!logSessionFile.isEmpty()) {
	actionSave->setEnabled(true);
	actionSave_Menu->setEnabled(true);
    }
    setWindowModified(true);
    parserObjList_.append(elemObj);
}
