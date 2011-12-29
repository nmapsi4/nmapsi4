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

parserManager::parserManager(nmapClass* parent) : QWidget(parent), m_ui(parent)
{

}

parserManager::~parserManager()
{
    freelist<QTreeWidgetItem*>::itemDeleteAll(m_itemListScan);
    freelist<parserObj*>::itemDeleteAll(m_parserObjList);
    freelist<parserObjUtil*>::itemDeleteAll(m_parserObjUtilList);
    freelist<QTreeWidgetItem*>::itemDeleteAll(m_treeItems);
}

void parserManager::clearParserItems()
{
    freelist<parserObj*>::itemDeleteAll(m_parserObjList);
    freelist<parserObjUtil*>::itemDeleteAll(m_parserObjUtilList);
    freelist<QTreeWidgetItem*>::itemDeleteAll(m_objectItems);
    freelist<QTreeWidgetItem*>::itemDeleteAll(m_itemListScan);
    freelist<QTreeWidgetItem*>::itemDeleteAll(m_treeItems);
}

void parserManager::addUtilObject(parserObjUtil* object)
{
    m_parserObjUtilList.append(object);
}

void parserManager::startParser(const QStringList parList, QByteArray dataBuffer, QByteArray errorBuffer)
{
    // check nmap error
    if(!dataBuffer.size() && errorBuffer.size())
    {
        QMessageBox::critical(this, "NmapSI4", tr("Error: check nmap Installation.\n"), tr("Close"));
        return;
    }

    m_ui->progressScan->setValue(75);

    QString StdoutStr(dataBuffer);
    QString StderrorStr(errorBuffer);

    // create a scan host item.
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(m_ui->treeMain);
    m_treeItems.push_front(treeItem);
    treeItem->setSizeHint(0, QSize(32, 32));

    // TODO: move to runtime parser
    m_ui->comboScanLog->insertItem(3, parList.join(" "));

    // call real parser
    parserObj* elemObj = parserCore(parList,StdoutStr,StderrorStr,treeItem);

    elemObj->setParameters(parList.join(" "));

    int tmpBox = m_ui->SWscan->currentIndex();

    switch(tmpBox)
    {
      case 0:
        m_ui->Bnss->setIcon(QIcon(QString::fromUtf8(":/images/images/reload.png")));
        break;
      case 1:
        m_ui->Bdetails->setIcon(QIcon(QString::fromUtf8(":/images/images/reload.png")));
        break;
      case 2:
        m_ui->Bdetails->setIcon(QIcon(QString::fromUtf8(":/images/images/reload.png")));
        m_ui->Bnss->setIcon(QIcon(QString::fromUtf8(":/images/images/reload.png")));
        break;
    }

    // clear output and error thread buffer.
    dataBuffer.clear();
    errorBuffer.clear();

    m_ui->progressScan->setValue(85);

    if(!m_ui->_monitor->monitorHostNumber())
    {
        m_ui->progressScan->setValue(100);
        m_ui->monitorStopAllScanButt->setEnabled(false);
        m_ui->monitorStopCurrentScanButt->setEnabled(false);
        m_ui->monitorDetailsScanButt->setEnabled(false);
        m_ui->tabUi->setTabIcon(m_ui->tabUi->indexOf(m_ui->tabMainMonitor),
                          QIcon(QString::fromUtf8(":/images/images/utilities-system-monitor.png")));
    }
    else
    {
        m_ui->progressScan->setValue(55);
    }

    m_ui->action_Scan_menu->setEnabled(true);
    m_ui->action_Scan_2->setEnabled(true);
    m_ui->hostEdit->setEnabled(true);
    m_ui->actionSave->setEnabled(true);
    m_ui->actionSave_Menu->setEnabled(true);
    m_ui->actionSave_As_Menu->setEnabled(true);

    if(!m_ui->_monitor->monitorHostNumber())
    {
        m_ui->_monitor->clearHostMonitor();
    }

    m_parserObjList.append(elemObj);
}

void parserManager::showParserResult(QTreeWidgetItem *item, int column)
{ // SLOT
    Q_UNUSED(column);

    QString hostName_ = item->text(0);
    hostName_  = hostName_.left(hostName_.indexOf("\n"));

    if(m_ui->hostEdit->itemText(0).isEmpty() && item->parent() == NULL)
    {
        m_ui->hostEdit->addItem(hostName_);
    }
    else if(item->parent() == NULL)
    {
        m_ui->hostEdit->setItemText(0, hostName_);
    }

    int indexObj = m_ui->treeMain->indexOfTopLevelItem(item);

    if(indexObj != -1)
    {
        showParserObj(indexObj);
        showParserObjPlugins(indexObj);
    }
}

void parserManager::showParserTracerouteResult(QTreeWidgetItem *item, int column)
{ // SLOT
    Q_UNUSED(column);

    if(m_ui->hostEdit->itemText(0).isEmpty() && !item->parent() && !item->text(2).isEmpty())
    {
        if(!item->text(3).contains("DNS"))
        {
            m_ui->hostEdit->addItem(item->text(3));
        }
        else
        {
            m_ui->hostEdit->addItem(item->text(2));
        }
    }
    else if(!item->parent() && !item->text(2).isEmpty())
    {
        if(!item->text(3).contains("DNS"))
        {
            m_ui->hostEdit->setItemText(0, item->text(3));
        }
        else
        {
            m_ui->hostEdit->setItemText(0, item->text(2));
        }
    }
}

parserObj* parserManager::parserCore(const QStringList parList, QString StdoutStr,
                       QString StderrorStr, QTreeWidgetItem* mainTreeE)
{
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

            nseBuffer.append(tmpClean + "\n");
        }

        if((rxT_.indexIn(tmp) != -1) && (!tmp.contains("/")))
        {
            bufferTraceroot.append(tmp);
            bufferTraceroot.append("\n");
        }

    } // End first While

    QString tmp_host;

    if (!generalBuffer_.isEmpty())
    {
        //QFont rootFont = root->font(0);
        //rootFont.setWeight(QFont::Normal);
        tmp_host.append(generalBuffer_ + '\n' + QDateTime::currentDateTime().toString("ddd MMM d yy - hh:mm:ss"));
        mainTreeE->setText(0, tmp_host);
    }
    else
    {
        tmp_host.append(hostCheck + '\n' + QDateTime::currentDateTime().toString("ddd MMM d yy - hh:mm:ss"));
        mainTreeE->setText(0, tmp_host);
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
            }
        } // end while

        elemObj->setValidity(true);
    }
    else
    {
        mainTreeE->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit_noresult.png")));
        elemObj->setValidity(false);
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
    m_ui->actionClear_History->setEnabled(true);

    QTextStream bufferLogStream(&StdoutStr);
    QString bufferLogStream_line;

    // check for full log scan
    while (!bufferLogStream.atEnd())
    {
        bufferLogStream_line = bufferLogStream.readLine();
        if (!bufferLogStream_line.isEmpty())
        {
            elemObj->setFullScanLog(bufferLogStream_line);
        }
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
    freelist<QTreeWidgetItem*>::itemDeleteAll(m_itemListScan);
    freelist<QTreeWidgetItem*>::itemDeleteAll(m_objectItems);
    m_ui->listWscan->clear();
    m_ui->GItree->clear();
    m_ui->treeNSS->clear();

    // set combo scan parameters
    int treeMainIndex_ = m_ui->treeMain->indexOfTopLevelItem(m_ui->treeMain->currentItem());
    m_ui->comboScanLog->setCurrentIndex(treeMainIndex_+1);

    QString noInfo("not Discovered");

    foreach (const QString &token, m_parserObjList[hostIndex]->getMainInfo())
    {
        QTreeWidgetItem *root = new QTreeWidgetItem(m_ui->treeHostDet);
        m_itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/messagebox_info.png")));
        root->setText(0, token);
        root->setToolTip(0, token);
    }

    QString noDes = tr("No description");
    // clear combo Vulnerabilities
    m_ui->comboVuln->clear();
    m_ui->comboVuln->insertItem(0,"Services");

    // Show open ports
    foreach (const QString &token, m_parserObjList[hostIndex]->getPortOpen())
    {
        QTreeWidgetItem *root = new QTreeWidgetItem(m_ui->listWscan);
        m_itemListScan.push_front(root);
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
                m_ui->comboVuln->addItem(split[3]);
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
                m_ui->comboVuln->addItem(lineDescription_);
            }
        }
        else
        {
            root->setText(3, noDes);
        }
    }

    // Show Close ports
    foreach (const QString &token, m_parserObjList[hostIndex]->getPortClose())
    {
        QTreeWidgetItem *root = new QTreeWidgetItem(m_ui->listWscan);
        m_itemListScan.push_front(root);
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
                m_ui->comboVuln->addItem(split[3]);
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
                m_ui->comboVuln->addItem(lineDescription_);
            }
        }
        else
        {
            root->setText(3, noDes);
        }
    }

    // Show Filtered ports
    foreach (const QString &token, m_parserObjList[hostIndex]->getPortFiltered())
    {
        QTreeWidgetItem *root = new QTreeWidgetItem(m_ui->listWscan);
        m_itemListScan.push_front(root);
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
                m_ui->comboVuln->addItem(split[3]);
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
                m_ui->comboVuln->addItem(lineDescription_);
            }
        }
        else
        {
            root->setText(3, noDes);
        }
    }

    // show services
    foreach (const QString &token, m_parserObjList[hostIndex]->getServices())
    {
        if (!m_ui->listWscan->findItems(token, Qt::MatchExactly, 2)[0]->text(3).contains(noDes))
        {
            QTreeWidgetItem *objItem = new QTreeWidgetItem(m_ui->GItree);
            objItem->setSizeHint(0, QSize(22, 22));
            objItem->setIcon(0, QIcon(QString::fromUtf8(":/images/images/network_local.png")));
            m_objectItems.push_front(objItem);
            objItem->setText(0,token);
        }
    }

    // Show Nss Info
    QHash<QString, QStringList> nseResult = m_parserObjList[hostIndex]->getNseResult();
    QHash<QString, QStringList>::const_iterator i;
    m_ui->treeNSS->setRootIsDecorated(true);

    for (i = nseResult.constBegin(); i != nseResult.constEnd(); ++i)
    {
        QTreeWidgetItem *root = new QTreeWidgetItem(m_ui->treeNSS);
        m_itemListScan.push_front(root);
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
            m_itemListScan.push_front(item);

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
    foreach (const QString &token, m_parserObjList[hostIndex]->getFullScanLog())
    {
        QTreeWidgetItem *root = new QTreeWidgetItem(m_ui->listScan);
        m_itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/book.png")));
        root->setText(0, token);
        root->setToolTip(0, token);
    }

    // Show scan error
    foreach (const QString &token, m_parserObjList[hostIndex]->getErrorScan())
    {
        QTreeWidgetItem *root = new QTreeWidgetItem(m_ui->listScanError);
        m_itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/messagebox_critical.png")));
        root->setText(0, token);
        root->setToolTip(0, token);
    }
}

void parserManager::showParserObjPlugins(int hostIndex)
{
    // show traceroute
    foreach (const QString &token, m_parserObjList[hostIndex]->getTraceRouteInfo())
    {
        QTreeWidgetItem *root = new QTreeWidgetItem(m_ui->treeTraceroot);
        m_itemListScan.push_front(root);
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
    foreach (parserObjUtil* elem, m_parserObjUtilList)
    {
        if(m_parserObjList[hostIndex]->getHostName() == elem->getHostName())
        {
            foreach (const QString &token, elem->getInfoLookup())
            {
                QTreeWidgetItem *root = new QTreeWidgetItem(m_ui->treeLookup);
                m_itemListScan.push_front(root);
                root->setSizeHint(0, QSize(22, 22));
                root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit.png")));
                root->setText(0, token);
            }
            break;
        }
    }
}

void parserManager::callSaveSingleLogWriter()
{
    if (!m_ui->treeMain->selectedItems().size())
    {
        return;
    }

    int selectedItemsIndex = m_ui->treeMain->indexOfTopLevelItem(m_ui->treeMain->selectedItems()[0]);
    parserObj *object = m_parserObjList[selectedItemsIndex];

    if (!object->isValidObject())
    {
        return;
    }

    const QString& path = QFileDialog::getSaveFileName(
                              this,
                              tr("Save Log"),
                              QDir::homePath() + QDir::toNativeSeparators("/") + ".log",
                              tr("Log (*.log)")
                          );

    if (!path.isEmpty())
    {
        logWriter *writer = new logWriter();
        writer->writeSingleLogFile(object, path);
        delete writer;
    }
}

void parserManager::callSaveAllLogWriter()
{
    if (!m_parserObjList.size())
    {
        return;
    }

    const QString& directoryPath = QFileDialog::getExistingDirectory(
                                       this,
                                       "Open Directory",
                                       QDir::homePath(),
                                       QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
                                   );

    if (!directoryPath.isEmpty())
    {
        logWriter *writer = new logWriter();
        writer->writeAllLogFile(m_parserObjList,directoryPath);
        delete writer;
    }
}
