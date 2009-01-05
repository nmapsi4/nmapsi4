/***************************************************************************
 *   Copyright (C) 2007-2008 by Francesco Cecconi                          *
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

void nmapClass::nmapParser(QString hostCheck)
{
    QString * StdoutStr;
    QString * StderrorStr;
    QString title = "NmapSI4 ";
    int open_port = 0;
    int close_port = 0;
    int filtered_port = 0;
    int error_count = 0;
    int ItemNumber = 0;

    //qDebug() << "return...." << StdoutStr;
    qDebug() << "Host Checked:::: " << hostCheck;
    delMonitorHost(scanMonitor, hostCheck);

    listClearFlag = false; // the listScan is not empty

    StdoutStr = new QString(Byte1); // read std buffer
    StderrorStr = new QString(Byte2); // read error buffer

    progressScan->setValue(75);
    this->setWindowTitle(title.append("(75%)"));
    
    QTextStream stream(StdoutStr);
    QString tmp, buffer, buffer2, bufferInfo;

    while (!stream.atEnd()) {
        tmp = stream.readLine();

        if ((tmp.contains("open") || tmp.contains("closed")
                || tmp.contains("filtered")
                || tmp.contains("unfiltered"))
                && !tmp.contains("Not shown:")
                && !tmp.contains("Discovered")) {

            buffer2.append(tmp);
            buffer2.append("\n");
        }

        if (tmp.contains("Hosts:") || tmp.contains("Host:")) {
            buffer.append(tmp);
        }

        if (tmp.contains("MAC")
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
           ) {

            bufferInfo.append(tmp);
            bufferInfo.append("\n");
        }

    }


    root = new QTreeWidgetItem(listWscan);
    itemList.push_front(root);

    root2 = new QTreeWidgetItem(listScan);
    itemList.push_front(root2);

    error = new QTreeWidgetItem(listScanError);
    itemList.push_front(error);

    infoItem = new QTreeWidgetItem(treeWinfo);
    itemList.push_front(infoItem);

    // check for log file
    QTextStream *out = NULL;

    if (PFile) {
        out = new QTextStream(PFile);
        QString nmap_command;
        *out << "--------------------------------------------------------------------" << endl;
        nmap_command.append("\nLog Start: ");
        nmap_command.append("\nnmap ");
        nmap_command.append(lineOptions->text());
        nmap_command.append(hostCheck); // write host target in the log
        *out << nmap_command << endl << endl;
    }

    listWscan->setIconSize(QSize::QSize(32, 32));
    listWscan->header()->setResizeMode(0, QHeaderView::Interactive);
    listScanError->setIconSize(QSize::QSize(32, 32));
    listScanError->header()->setResizeMode(0, QHeaderView::Interactive);
    listScan->setIconSize(QSize::QSize(32, 32));
    listScan->header()->setResizeMode(0, QHeaderView::Interactive);
    treeWinfo->setIconSize(QSize::QSize(32, 32));
    treeWinfo->header()->setResizeMode(0, QHeaderView::Interactive);

    int tmpBox = toolBox->currentIndex();
 
    switch(tmpBox) {
      case 0:	
	toolBox->setItemIcon(1, QIcon(QString::fromUtf8(":/images/images/reload.png")));
        toolBox->setItemIcon(2, QIcon(QString::fromUtf8(":/images/images/reload.png")));
	break;
      case 1:
	toolBox->setItemIcon(0, QIcon(QString::fromUtf8(":/images/images/reload.png")));
        toolBox->setItemIcon(2, QIcon(QString::fromUtf8(":/images/images/reload.png")));
	break;
      case 2:
	toolBox->setItemIcon(0, QIcon(QString::fromUtf8(":/images/images/reload.png")));
        toolBox->setItemIcon(1, QIcon(QString::fromUtf8(":/images/images/reload.png")));
	break;
    }

    if (!hostEdit->currentText().contains("(Stopped)"))
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit.png")));
    else
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/messagebox_critical.png")));

    root2->setIcon(0, QIcon(QString::fromUtf8(":/images/images/book.png")));
    error->setIcon(0, QIcon(QString::fromUtf8(":/images/images/messagebox_critical.png")));
    infoItem->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit.png")));

    if (!buffer.isEmpty()) { // Host line scan
	root->setBackground(0, QColor::fromRgb(75, 75, 75));
	root->setBackground(1, QColor::fromRgb(75, 75, 75));
	root->setBackground(2, QColor::fromRgb(75, 75, 75));
	root->setBackground(3, QColor::fromRgb(75, 75, 75));
	root->setBackground(4, QColor::fromRgb(75, 75, 75));
	root->setForeground(0, Qt::white);
	root->setForeground(1, Qt::white);
	root->setForeground(2, Qt::white);
	root->setForeground(3, Qt::white);
	root->setForeground(4, Qt::white);
	infoItem->setBackground(0, QColor::fromRgb(75, 75, 75));
	infoItem->setForeground(0, Qt::white);
	root2->setBackground(0, QColor::fromRgb(75, 75, 75));
	root2->setForeground(0, Qt::white);
	error->setBackground(0, QColor::fromRgb(75, 75, 75));
	error->setForeground(0, Qt::white);

        QFont rootFont = root->font(0);
        rootFont.setWeight(QFont::Normal);
        root->setFont(0, rootFont);
        root->setText(0, buffer);
        root2->setText(0, buffer);
        error->setText(0, buffer);
        infoItem->setText(0, buffer);
        if ((PFile) && (!verboseLog)) *out << root->text(0) << endl;
    } else {
      	root->setBackground(0, QColor::fromRgb(75, 75, 75));
	root->setBackground(1, QColor::fromRgb(75, 75, 75));
	root->setBackground(2, QColor::fromRgb(75, 75, 75));
	root->setBackground(3, QColor::fromRgb(75, 75, 75));
	root->setBackground(4, QColor::fromRgb(75, 75, 75));
	root->setForeground(0, Qt::white);
	root->setForeground(1, Qt::white);
	root->setForeground(2, Qt::white);
	root->setForeground(3, Qt::white);
	root->setForeground(4, Qt::white);
	infoItem->setBackground(0, QColor::fromRgb(75, 75, 75));
	infoItem->setForeground(0, Qt::white);
	root2->setBackground(0, QColor::fromRgb(75, 75, 75));
	root2->setForeground(0, Qt::white);
	error->setBackground(0, QColor::fromRgb(75, 75, 75));
	error->setForeground(0, Qt::white);

        QFont rootFont = root->font(0);
        rootFont.setWeight(QFont::Normal);
        root->setFont(0, rootFont);
/*        root->setText(0, hostEdit->currentText());
        root2->setText(0, hostEdit->currentText());
        error->setText(0, hostEdit->currentText());
        infoItem->setText(0, hostEdit->currentText());*/
        root->setText(0, hostCheck);
        root2->setText(0, hostCheck);
        error->setText(0, hostCheck);
        infoItem->setText(0, hostCheck);
        if ((PFile) && (!verboseLog)) *out << root->text(0) << endl;
    }

    QTextStream b2(&buffer2); // QString to QtextStream (scan Tree)
    QString b2_line;
    QString tmp_mess = root->text(0);
    QString tmp_mess2 = root->text(0);

    if (!b2.atEnd()) { // check for scan informations
        while (!b2.atEnd()) {
            b2_line = b2.readLine();
            item2 = new QTreeWidgetItem(root);
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
                qDebug() << lStr;

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
                int index = 3;
                QString str;
                // Description
                if (lStr.size() == 3)
                    item2->setText(0, tr("No description"));
                else {
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
                }

                if ((PFile) && (!verboseLog)) *out << item2->text(0) << endl;
            } else
                item2->setText(0, tr("No Ports"));

            ItemNumber++;

        }
    } else { // insert message for no info
        tmp_mess.append(tr("\n(No scan informations)"));
        root->setText(0, tmp_mess);
    }

    QTextStream b3(&bufferInfo); // QString to QtextStrem (scan Tree)
    QString b3_line;

    if (!b3.atEnd()) { // check for scan informations
        while (!b3.atEnd()) {
            b3_line = b3.readLine();
            infoItemObj = new QTreeWidgetItem(infoItem);
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
        tmp_mess2.append(tr("\n(No Host Informations)"));
        infoItem->setText(0, tmp_mess2);
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
        item_root2 = new QTreeWidgetItem(root2); // append scan log
        itemList.push_front(item_root2); // reference to address

        item_root2->setText(0, blog_line);

        if ((PFile) && (verboseLog)) {
            *out << blog_line << "\n";
        }
    }
    *out << "Log End";
    *out << "--------------------------------------------------------------------" << endl;
    if (PFile) delete out;

    QTextStream b_error(StderrorStr);
    QString error_line;

    if (!b_error.atEnd()) { // check for no error
        while (!b_error.atEnd()) { // print error buffer informations
            item_error = new QTreeWidgetItem(error); // item_error memory allocation
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

    progressScan->setValue(85);
    this->setWindowTitle(title.replace("(75%)", "(85%)"));
    progressScan->setValue(100);
    this->setWindowTitle(title.replace("(85%)", "(100%) [*]"));

    this->setWindowIcon(QIcon(QString::fromUtf8(":/images/icons/nmapsi4_endScan.svg")));

    action_Scan_menu->setEnabled(true);
    action_Scan_2->setEnabled(true);
    hostEdit->setEnabled(true);
    //actionStop_Scan->setEnabled(false);
    action_Save_As->setEnabled(true);
    actionSave_As_Menu->setEnabled(true);
    if (!logSessionFile.isEmpty()) {
	actionSave->setEnabled(true);
	actionSave_Menu->setEnabled(true);
    }
    this->setWindowModified(true);
    listWscan->expandItem(root);
}
