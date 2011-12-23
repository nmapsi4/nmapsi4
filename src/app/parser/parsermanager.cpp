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

#include "parsermanager.h"
#include "mainwin.h"

parserManager::parserManager(nmapClass* parent) : QWidget(parent), _ui(parent)
{

}

parserManager::~parserManager()
{
    freelist<QTreeWidgetItem*>::itemDeleteAll(_itemListScan);
    freelist<parserObj*>::itemDeleteAll(_parserObjList);
    freelist<parserObjUtil*>::itemDeleteAll(_parserObjUtilList);

}

void parserManager::clearParserItems()
{
    freelist<parserObj*>::itemDeleteAll(_parserObjList);
    freelist<parserObjUtil*>::itemDeleteAll(_parserObjUtilList);
    freelist<QTreeWidgetItem*>::itemDeleteAll(_objectItems);
    freelist<QTreeWidgetItem*>::itemDeleteAll(_itemListScan);
}

void parserManager::addUtilObject(parserObjUtil* object)
{
    _parserObjUtilList.append(object);
}

void parserManager::startParser(const QStringList parList, QByteArray dataBuffer, QByteArray errorBuffer)
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
    }

    _parserObjList.append(elemObj);
}

void parserManager::showParserResult(QTreeWidgetItem *item, int column)
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

void parserManager::showParserTracerouteResult(QTreeWidgetItem *item, int column)
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

parserObj* parserManager::parserCore(const QStringList parList, QString StdoutStr,
                       QString StderrorStr, QTreeWidgetItem* mainTreeE)
{
    /*
     * _logFilePath and verboseLog is global in Ui::MainWindow
     * TODO:: remove _logFilePath and verboleLog with porting to new save log.
     */
    // Create parser Obect
    parserObj *elemObj = new parserObj();
    QString hostCheck = parList[parList.size()-1];
    elemObj->setHostName(hostCheck);

    QRegExp rxT_("^\\d\\d?");
    QString generalBuffer_(hostCheck);
    QString scanBuffer;
    QString bufferInfo;
    QString bufferTraceroot;
    QString nseBuffer;
    QString tmp;

    QTextStream stream(&StdoutStr);

    while (!stream.atEnd())
    {
        tmp = stream.readLine();

        if ((rxT_.indexIn(tmp) != -1) &&
                (tmp.contains("open")
                || tmp.contains("closed")
                || tmp.contains("filtered")
                || tmp.contains("unfiltered"))
                && !tmp.contains("Not shown:")
                && !tmp.contains("Discovered"))
        {

            scanBuffer.append(tmp);
            scanBuffer.append("\n");

            // Insert new elem to nse buffer result
            nseBuffer.append("|--" + tmp + "\n");
        }

        if (tmp.startsWith("Host script results:"))
        {
            nseBuffer.append("|--" + tmp + "\n");
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
                || tmp.startsWith(QLatin1String("Hosts"))
                || (tmp.startsWith(QLatin1String("Host")) && !tmp.contains("Host script results:")))
                && !tmp.startsWith(QLatin1String("|")))
        {
            bufferInfo.append(tmp);
            bufferInfo.append("\n");
        }

        // pars for subtree service
        if(tmp.startsWith(QLatin1String("|")) && !nseBuffer.isEmpty())
        {
            QString tmpClean(tmp);
            if(tmpClean.startsWith(QLatin1String("|")))
            {
                tmpClean.remove('|');
            }

            if(tmpClean.startsWith(QLatin1String("_")))
            {
                tmpClean.remove('_');
            }

            int pos;
            while(tmpClean.startsWith(QLatin1String(" ")))
            {
                pos = tmpClean.indexOf(" ");
                if(pos == 0)
                {
                    tmpClean.remove(pos,1);
                }
            }

            // FIXME: create new nse buffer
            nseBuffer.append(tmpClean + "\n");
        }

        if((rxT_.indexIn(tmp) != -1) && (!tmp.contains("/")))
        {
            bufferTraceroot.append(tmp);
            bufferTraceroot.append("\n");
        }

    } // End first While

    // check for log file
    QTextStream *file = NULL;
    if (_ui->_logFilePath)
    {
        file = new QTextStream(_ui->_logFilePath);
        // parameters list for log
        QString nmap_command(QString("\n==LogStart: ") + QString("\nnmap ") + parList.join(" "));
        *file << nmap_command << endl << endl;
    }

    QString tmp_host;

    if (!generalBuffer_.isEmpty())
    {
        //QFont rootFont = root->font(0);
        //rootFont.setWeight(QFont::Normal);
        tmp_host.append(generalBuffer_ + '\n' + QDateTime::currentDateTime().toString("ddd MMM d yy - hh:mm:ss"));
        mainTreeE->setText(0, tmp_host);

        if ((_ui->_logFilePath) && (!_ui->verboseLog))
        {
            *file << generalBuffer_ << endl;
        }
    }
    else
    {
        tmp_host.append(hostCheck + '\n' + QDateTime::currentDateTime().toString("ddd MMM d yy - hh:mm:ss"));
        mainTreeE->setText(0, tmp_host);

        if ((_ui->_logFilePath) && (!_ui->verboseLog))
        {
            *file << hostCheck << endl;
        }
    }

    QTextStream scanBufferToStream_(&scanBuffer); // QString to QtextStream (scan Tree)
    QString scanBufferToStream_line;

    // check for scan result
    if (!scanBufferToStream_.atEnd())
    {
        while (!scanBufferToStream_.atEnd())
        {
            scanBufferToStream_line = scanBufferToStream_.readLine();
            if (scanBufferToStream_line.contains("open") || scanBufferToStream_line.contains("filtered")
                    || scanBufferToStream_line.contains("unfiltered"))
            {

                if (scanBufferToStream_line.contains("filtered") || scanBufferToStream_line.contains("unfiltered"))
                {
                    elemObj->setPortFiltered(scanBufferToStream_line);
                }
                else
                {
                    elemObj->setPortOpen(scanBufferToStream_line);
                }
            }
            else
            {
                elemObj->setPortClose(scanBufferToStream_line);
            }

            if (!scanBufferToStream_line.isEmpty())
            {
                QString tmpStr = scanBufferToStream_line;
                QStringList lStr = tmpStr.split(' ', QString::SkipEmptyParts);
                elemObj->setServices(lStr[2]); // Obj Services
                elemObj->setPortServices(lStr[0]);

                if ((_ui->_logFilePath) && (!_ui->verboseLog))
                {
                    *file << scanBufferToStream_line << endl;
                }
            }
        } // end while
    }
    else
    {
        mainTreeE->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit_noresult.png")));
    }

    QTextStream bufferInfoStream(&bufferInfo); // QString to QtextStrem (scan Tree)
    QString bufferInfoStream_line;

    // check for Host information
    // OS not detected
    bool state_ = false;
    while (!bufferInfoStream.atEnd())
    {
         bufferInfoStream_line = bufferInfoStream.readLine();
         // Check OS String
         if (bufferInfoStream_line.contains("OS") && !state_)
         {
            // OS was found ?
            state_ = hostTools::checkViewOS(bufferInfoStream_line,mainTreeE);
         }

         elemObj->setMainInfo(bufferInfoStream_line);
         if (_ui->_logFilePath && !_ui->verboseLog)
         {
            *file << bufferInfoStream_line << endl;
         }
    }

    if (mainTreeE->icon(0).isNull())
    {
        mainTreeE->setTextAlignment(1, Qt::AlignVCenter | Qt::AlignRight);
        mainTreeE->setIcon(0, QIcon(QString::fromUtf8(":/images/images/network_local.png")));
        mainTreeE->setText(1, "Undiscovered");
    }

    QTextStream bufferTraceStream(&bufferTraceroot); // Traceroute buffer
    QString bufferTraceStream_line;

    // check for traceroute scan information
    while (!bufferTraceStream.atEnd())
    {
        bufferTraceStream_line = bufferTraceStream.readLine();
        if (!bufferTraceStream_line.isEmpty() && !bufferTraceStream_line.contains("guessing hop"))
        {
            elemObj->setTraceRouteInfo(bufferTraceStream_line);
            if (_ui->_logFilePath && !_ui->verboseLog)
            {
                *file << bufferTraceStream_line << endl;
            }
        }
    }

    QTextStream nseStream(&nseBuffer);
    QString nseStreamLine = nseStream.readLine();
    QHash<QString, QStringList> nseResult;

    while (!nseStream.atEnd())
    {
        QString service;
        QStringList serviceNseResult;

        if (nseStreamLine.startsWith("|--"))
        {
            service = nseStreamLine.remove("|--");
            nseStreamLine = nseStream.readLine();
        }

        while (!nseStreamLine.startsWith("|--") && !nseStream.atEnd())
        {
            if (!nseStreamLine.isEmpty())
            {
                serviceNseResult.append(nseStreamLine);
            }
            nseStreamLine = nseStream.readLine();
        }

        if (!service.isEmpty() && serviceNseResult.size())
        {
            nseResult.insert(service,serviceNseResult);
        }
    }

    // save nse result with QHash
    elemObj->setNseResult(nseResult);
    _ui->actionClear_History->setEnabled(true);

    QTextStream bufferLogStream(&StdoutStr);
    QString bufferLogStream_line;

    // check for full log scan
    while (!bufferLogStream.atEnd())
    {
        bufferLogStream_line = bufferLogStream.readLine();
        if (!bufferLogStream_line.isEmpty())
        {
            elemObj->setFullScanLog(bufferLogStream_line);
            if ((_ui->_logFilePath) && (_ui->verboseLog))
            {
                *file << bufferLogStream_line << "\n";
            }
        }
    }

    *file << "==LogEnd\n";
    if (_ui->_logFilePath)
    {
        delete file;
    }

    QTextStream bufferErrorStream(&StderrorStr);
    QString bufferErrorStream_line;

    // check for scan error
    while (!bufferErrorStream.atEnd())
    {
        bufferErrorStream_line = bufferErrorStream.readLine();
        elemObj->setErrorScan(bufferErrorStream_line);
    }

    return elemObj;
}

void parserManager::showParserObj(int hostIndex)
{
    // Clear widget
    freelist<QTreeWidgetItem*>::itemDeleteAll(_itemListScan);
    freelist<QTreeWidgetItem*>::itemDeleteAll(_objectItems);
    _ui->listWscan->clear();
    _ui->GItree->clear();
    _ui->treeNSS->clear();

    // set combo scan parameters
    int treeMainIndex_ = _ui->treeMain->indexOfTopLevelItem(_ui->treeMain->currentItem());
    _ui->comboScanLog->setCurrentIndex(treeMainIndex_+1);

    QString noInfo("not Discovered");

    foreach (const QString &token, _parserObjList[hostIndex]->getMainInfo())
    {
        QTreeWidgetItem *root = new QTreeWidgetItem(_ui->treeHostDet);
        _itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/messagebox_info.png")));
        root->setText(0, token);
        root->setToolTip(0, token);
    }

    QString noDes = tr("No description");
    // clear combo Vulnerabilities
    _ui->comboVuln->clear();
    _ui->comboVuln->insertItem(0,"Services");

    // Show open ports
    foreach (const QString &token, _parserObjList[hostIndex]->getPortOpen())
    {
        QTreeWidgetItem *root = new QTreeWidgetItem(_ui->listWscan);
        _itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/flag_green.png")));
        root->setForeground(0, QBrush(QColor(0, 0, 255, 127)));
        QStringList split = token.split(' ', QString::SkipEmptyParts);
        root->setText(0, split[0]);
        root->setText(1, split[1]);
        root->setText(2, split[2]);
        if (split.size() == 4)
        {
            root->setText(3, split[3]);
            root->setToolTip(3, split[3]);
            if (!split[3].isEmpty())
            {
                _ui->comboVuln->addItem(split[3]);
            }
        }
        else if (split.size() > 4)
        {
            QString lineDescription_("");
            for(int index=3; index < split.size(); index++)
            {
                lineDescription_.append(split[index]);
                lineDescription_.append(" ");
            }
            root->setText(3, lineDescription_);
            root->setToolTip(3, lineDescription_);
            //load comboVuln
            if (!lineDescription_.isEmpty())
            {
                _ui->comboVuln->addItem(lineDescription_);
            }
        }
        else
        {
            root->setText(3, noDes);
        }
    }

    // Show Close ports
    foreach (const QString &token, _parserObjList[hostIndex]->getPortClose())
    {
        QTreeWidgetItem *root = new QTreeWidgetItem(_ui->listWscan);
        _itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/flag_red.png")));
        QStringList split = token.split(' ', QString::SkipEmptyParts);
        root->setText(0, split[0]);
        root->setText(1, split[1]);
        root->setText(2, split[2]);
        if (split.size() == 4)
        {
            root->setText(3, split[3]);
            root->setToolTip(3, split[3]);
            if (!split[3].isEmpty())
            {
                _ui->comboVuln->addItem(split[3]);
            }
        }
        else if (split.size() > 4)
        {
            QString lineDescription_("");
            for(int index=3; index < split.size(); index++)
            {
                lineDescription_.append(split[index]);
                lineDescription_.append(" ");
            }
            root->setText(3, lineDescription_);
            root->setToolTip(3, lineDescription_);
            //load comboVuln
            if (!lineDescription_.isEmpty())
            {
                _ui->comboVuln->addItem(lineDescription_);
            }
        }
        else
        {
            root->setText(3, noDes);
        }
    }

    // Show Filtered ports
    foreach (const QString &token, _parserObjList[hostIndex]->getPortFiltered())
    {
        QTreeWidgetItem *root = new QTreeWidgetItem(_ui->listWscan);
        _itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/flag_yellow.png")));
        root->setForeground(0, QBrush(QColor(255, 0, 0, 127)));
        QStringList split = token.split(' ', QString::SkipEmptyParts);
        root->setText(0, split[0]);
        root->setText(1, split[1]);
        root->setText(2, split[2]);
        if (split.size() == 4)
        {
            root->setText(3, split[3]);
            root->setToolTip(3, split[3]);
            if (!split[3].isEmpty())
            {
                _ui->comboVuln->addItem(split[3]);
            }
        }
        else if (split.size() > 4)
        {
            QString lineDescription_("");
            for(int index=3; index < split.size(); index++)
            {
                lineDescription_.append(split[index]);
                lineDescription_.append(" ");
            }
            root->setText(3, lineDescription_);
            root->setToolTip(3, lineDescription_);
            //load comboVuln
            if (!lineDescription_.isEmpty())
            {
                _ui->comboVuln->addItem(lineDescription_);
            }
        }
        else
        {
            root->setText(3, noDes);
        }
    }

    // show services
    foreach (const QString &token, _parserObjList[hostIndex]->getServices())
    {
        if (!_ui->listWscan->findItems(token, Qt::MatchExactly, 2)[0]->text(3).contains(noDes))
        {
            QTreeWidgetItem *objItem = new QTreeWidgetItem(_ui->GItree);
            objItem->setSizeHint(0, QSize(22, 22));
            objItem->setIcon(0, QIcon(QString::fromUtf8(":/images/images/network_local.png")));
            _objectItems.push_front(objItem);
            objItem->setText(0,token);
        }
    }

    // Show Nss Info
    QHash<QString, QStringList> nseResult = _parserObjList[hostIndex]->getNseResult();
    QHash<QString, QStringList>::const_iterator i;
    _ui->treeNSS->setRootIsDecorated(true);

    for (i = nseResult.constBegin(); i != nseResult.constEnd(); ++i)
    {
        QTreeWidgetItem *root = new QTreeWidgetItem(_ui->treeNSS);
        _itemListScan.push_front(root);
        root->setSizeHint(0, QSize(32, 32));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/traceroute.png")));

        QStringList rootValue = i.key().split(' ', QString::SkipEmptyParts);

        if (rootValue.size() >= 3)
        {
            root->setText(0, rootValue[0] + ' ' + rootValue[2]);
        }
        else
        {
            root->setText(0, rootValue[0]);
        }

        foreach (const QString& value, i.value())
        {
            QTreeWidgetItem *item = new QTreeWidgetItem(root);
            _itemListScan.push_front(item);

            if (value.contains(":") && !value.contains("//")
                    && !value.contains("ERROR"))
            {
                item->setForeground(0, QBrush(QColor(0, 0, 255, 127)));
            }

            if (value.contains("ERROR"))
            {
                item->setForeground(0, QBrush(QColor(255, 0, 0, 127)));
            }

            item->setText(0, value);
        }
    }

    // Show full scan log
    foreach (const QString &token, _parserObjList[hostIndex]->getFullScanLog())
    {
        QTreeWidgetItem *root = new QTreeWidgetItem(_ui->listScan);
        _itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/book.png")));
        root->setText(0, token);
        root->setToolTip(0, token);
    }

    // Show scan error
    foreach (const QString &token, _parserObjList[hostIndex]->getErrorScan())
    {
        QTreeWidgetItem *root = new QTreeWidgetItem(_ui->listScanError);
        _itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/messagebox_critical.png")));
        root->setText(0, token);
        root->setToolTip(0, token);
    }
}

void parserManager::showParserObjPlugins(int hostIndex)
{
    // show traceroute
    foreach (const QString &token, _parserObjList[hostIndex]->getTraceRouteInfo())
    {
        QTreeWidgetItem *root = new QTreeWidgetItem(_ui->treeTraceroot);
        _itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/traceroute.png")));
        QStringList tmpToken = token.split(' ',QString::SkipEmptyParts);

        // MS windows check for ms string
        if(tmpToken.size() == 5)
        {
            if(tmpToken[2].size() < 4)
            { // minimum dns length
                tmpToken.removeAt(2);
            }
        }

        if(tmpToken.size() == 4)
        {
            if(tmpToken[2].size() < 4)
            { // minimum dns length
                tmpToken.removeAt(2);
            }
            else
            {
                tmpToken[3].remove('(');
                tmpToken[3].remove(')');
            }
        }

        if(tmpToken.size() == 4)
        {
            root->setText(0, tmpToken[0]);
            root->setText(1, tmpToken[1]);
            root->setText(3, tmpToken[2]);
            root->setText(2, tmpToken[3]);

        }
        else if(tmpToken.size() == 3)
        {
            root->setText(0, tmpToken[0]);
            root->setText(1, tmpToken[1]);
            root->setText(2, tmpToken[2]);
            root->setText(3, "no DNS");
            root->setForeground(3, QBrush(QColor(255, 0, 0, 127)));
        }
        else
        {
            root->setText(0, token);
            root->setToolTip(0, token);
        }
    }

    // show lookUp info
    foreach (parserObjUtil* elem, _parserObjUtilList)
    {
        if(_parserObjList[hostIndex]->getHostName() == elem->getHostName())
        {
            foreach (const QString &token, elem->getInfoLookup())
            {
                QTreeWidgetItem *root = new QTreeWidgetItem(_ui->treeLookup);
                _itemListScan.push_front(root);
                root->setSizeHint(0, QSize(22, 22));
                root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit.png")));
                root->setText(0, token);
            }
            break;
        }
    }
}
