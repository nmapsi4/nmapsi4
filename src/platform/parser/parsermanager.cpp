/*
Copyright 2011-2012  Francesco Cecconi <francesco.cecconi@gmail.com>

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

#include "parsermanager.h"
#include "mainwindow.h"

ParserManager::ParserManager(MainWindow* parent)
    : QObject(parent), m_ui(parent)
{
    m_rawlogHorizontalSplitter = new QSplitter(m_ui);
    m_rawlogHorizontalSplitter->setOrientation(Qt::Horizontal);
    m_rawlogHorizontalSplitter->addWidget(m_ui->m_scanWidget->listScan);
    m_rawlogHorizontalSplitter->addWidget(m_ui->m_scanWidget->listScanError);

    m_ui->m_scanWidget->horizontalLayoutRawlog->layout()->addWidget(m_rawlogHorizontalSplitter);

    QSettings settings("nmapsi4", "nmapsi4");

    if (!settings.value("rawlogHorizontalSplitter").toByteArray().isEmpty()) {
        m_rawlogHorizontalSplitter->restoreState(settings.value("rawlogHorizontalSplitter").toByteArray());
    }

    connect(m_ui->m_scanWidget->treeMain, SIGNAL(itemActivated(QTreeWidgetItem*,int)),
            this, SLOT(showParserResult(QTreeWidgetItem*,int)));
    connect(m_ui->m_scanWidget->treeTraceroot, SIGNAL(itemActivated(QTreeWidgetItem*,int)),
            this, SLOT(showParserTracerouteResult(QTreeWidgetItem*,int)));
    connect(m_ui->m_monitor, SIGNAL(hostFinisced(QStringList,QByteArray,QByteArray,int)),
            this, SLOT(startParser(QStringList,QByteArray,QByteArray,int)));
}

ParserManager::~ParserManager()
{
    memory::freelist<PObject*>::itemDeleteAll(m_parserObjList);
    memory::freelist<PObjectLookup*>::itemDeleteAll(m_parserObjUtilList);
}

void ParserManager::syncSettings()
{
    QSettings settings("nmapsi4", "nmapsi4");
    settings.setValue("rawlogHorizontalSplitter", m_rawlogHorizontalSplitter->saveState());
}

void ParserManager::clearParserItems()
{
    memory::freelist<PObject*>::itemDeleteAll(m_parserObjList);
    memory::freelist<PObjectLookup*>::itemDeleteAll(m_parserObjUtilList);
    memory::freelist<QTreeWidgetItem*>::itemDeleteAll(m_objectItems);
    memory::freelist<QTreeWidgetItem*>::itemDeleteAll(m_itemListScan);
    memory::freelist<QTreeWidgetItem*>::itemDeleteAll(m_treeItems);

    // clear combo Vulnerabilities
    m_ui->m_vulnerability->m_vulnerabilityWidget->comboVuln->clear();
    m_ui->m_vulnerability->m_vulnerabilityWidget->comboVuln->insertItem(0, "Services");
}

void ParserManager::addUtilObject(PObjectLookup* object)
{
    m_parserObjUtilList.append(object);
}

void ParserManager::startParser(const QStringList parList, QByteArray dataBuffer, QByteArray errorBuffer, int id)
{
    /*
     * TODO: remove this check with QT5 QStandardPaths::findExecutable.
     *
     */
    if (!dataBuffer.size() && errorBuffer.size()) {
        QMessageBox::critical(m_ui, "NmapSI4", tr("Error: check nmap Installation.\n")
                              + "\n\n"
                              + QString(errorBuffer), tr("Close"));
        return;
    }

    QString StdoutStr(dataBuffer);
    QString StderrorStr(errorBuffer);

    // create a scan host item.
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(m_ui->m_scanWidget->treeMain);
    m_treeItems.push_front(treeItem);
    treeItem->setSizeHint(0, QSize(32, 32));

    // call real parser
    PObject* elemObj = parserCore(parList, StdoutStr, StderrorStr, treeItem);

    elemObj->setParameters(parList.join(" "));
    elemObj->setId(id);

    // clear output and error thread buffer.
    dataBuffer.clear();
    errorBuffer.clear();

    QString message(tr("Scan completed"));
    // NOTE: It works only with kdelibs
    Notify::notificationMessage(parList[parList.size()-1], message);

#if defined(USE_KDELIBS)
    // limit Widget lines to 5
    int lineNumber = m_ui->m_kWidgetNotification->text().split('\n').size();
    if (lineNumber == 5) {
        m_ui->m_kWidgetNotification->setText(QString(""));
    }

    if (!m_ui->m_kWidgetNotification->text().isEmpty()) {
        message.prepend(m_ui->m_kWidgetNotification->text() + "\n> " + parList[parList.size()-1] + " - ");
    } else {
        message.prepend("> " + parList[parList.size()-1] + " - ");
    }

    m_ui->m_kWidgetNotification->setText(message);
    m_ui->m_kWidgetNotification->show();
#endif

    if (!m_ui->m_monitor->monitorHostNumber()) {
        m_ui->m_monitor->m_monitorWidget->scanProgressBar->setMaximum(100);
        m_ui->m_monitor->m_monitorWidget->monitorStopAllScanButt->setEnabled(false);
        m_ui->m_monitor->m_monitorWidget->monitorStopCurrentScanButt->setEnabled(false);
        m_ui->m_monitor->m_monitorWidget->monitorDetailsScanButt->setEnabled(false);
        m_ui->m_mainTabWidget->setTabIcon(m_ui->m_mainTabWidget->indexOf(m_ui->m_monitor->m_monitorWidget),
                                QIcon(QString::fromUtf8(":/images/images/utilities-system-monitor.png")));
        Notify::clearButtonNotify(m_ui->m_collections->m_collectionsButton.value("scan-sez"));
    }

    m_ui->m_collections->m_collectionsScanSection.value("scan-action")->setEnabled(true);
    m_ui->m_collections->enableSaveActions();

    if (!m_ui->m_monitor->monitorHostNumber()) {
        m_ui->m_monitor->clearHostMonitor();
    }

    m_parserObjList.append(elemObj);
}

void ParserManager::showParserResult(QTreeWidgetItem *item, int column)
{
    // SLOT
    Q_UNUSED(column);

    QString hostName_ = item->text(0);
    hostName_  = hostName_.left(hostName_.indexOf("\n"));

    if (m_ui->m_scanWidget->hostEdit->itemText(0).isEmpty() && item->parent() == NULL) {
        m_ui->m_scanWidget->hostEdit->addItem(hostName_);
    } else if (item->parent() == NULL) {
        m_ui->m_scanWidget->hostEdit->setItemText(0, hostName_);
    }

    int indexObj = m_ui->m_scanWidget->treeMain->indexOfTopLevelItem(item);

    if (indexObj != -1) {
        showParserObj(indexObj);
        showParserObjPlugins(indexObj);
    }
}

void ParserManager::showParserTracerouteResult(QTreeWidgetItem *item, int column)
{
    // SLOT
    Q_UNUSED(column);

    if (m_ui->m_scanWidget->hostEdit->itemText(0).isEmpty() && !item->parent() && !item->text(2).isEmpty()) {
        if (!item->text(3).contains("DNS")) {
            m_ui->m_scanWidget->hostEdit->addItem(item->text(3));
        } else {
            m_ui->m_scanWidget->hostEdit->addItem(item->text(2));
        }
    } else if (!item->parent() && !item->text(2).isEmpty()) {
        if (!item->text(3).contains("DNS")) {
            m_ui->m_scanWidget->hostEdit->setItemText(0, item->text(3));
        } else {
            m_ui->m_scanWidget->hostEdit->setItemText(0, item->text(2));
        }
    }
}

PObject* ParserManager::parserCore(const QStringList parList, QString StdoutStr,
                                   QString StderrorStr, QTreeWidgetItem* mainScanTreeElem)
{
    // Create parser Obect
    PObject *parserObjectElem = new PObject();
    QString hostCheck = parList[parList.size() - 1];
    parserObjectElem->setHostName(hostCheck);

    QRegExp portRx(matchPorts);
    QRegExp tracerouteRx(matchTraceroute);
    QString generalBuffer_(hostCheck);
    QString scanBuffer;
    QString bufferInfo;
    QString bufferTraceroot;
    QString nseBuffer;
    QString tmp;

    QTextStream stream(&StdoutStr);

    while (!stream.atEnd()) {
        tmp = stream.readLine();

        if ((portRx.indexIn(tmp) != -1)
                && (tmp.contains("open")
                    || tmp.contains("closed")
                    || tmp.contains("filtered")
                    || tmp.contains("unfiltered"))
                && !tmp.contains("Not shown:")
                && !tmp.contains("Discovered")) {

            scanBuffer.append(tmp);
            scanBuffer.append("\n");

            // Insert new elem to nse buffer result
            nseBuffer.append("|--" + tmp + '\n');
        }

        if (tmp.startsWith(QLatin1String("Host script results:"))) {
            nseBuffer.append("|--" + tmp + '\n');
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
                && !tmp.startsWith(QLatin1String("|"))) {
            bufferInfo.append(tmp);
            bufferInfo.append("\n");
        }

        // pars for subtree service
        if (tmp.startsWith(QLatin1String("|")) && !nseBuffer.isEmpty()) {
            QString tmpClean(tmp);
            if (tmpClean.startsWith(QLatin1String("|"))) {
                tmpClean.remove('|');
            }

            if (tmpClean.startsWith(QLatin1String("_"))) {
                tmpClean.remove('_');
            }

            int pos;
            while (tmpClean.startsWith(QLatin1String(" "))) {
                pos = tmpClean.indexOf(" ");
                if (pos == 0) {
                    tmpClean.remove(pos, 1);
                }
            }

            nseBuffer.append(tmpClean + '\n');
        }

        if ((tracerouteRx.indexIn(tmp) != -1) && (!tmp.contains("/"))) {
            bufferTraceroot.append(tmp);
            bufferTraceroot.append("\n");
        }

    } // End first While

    QString tmp_host;

    if (!generalBuffer_.isEmpty()) {
        //QFont rootFont = root->font(0);
        //rootFont.setWeight(QFont::Normal);
        tmp_host.append(generalBuffer_ + '\n' + QDateTime::currentDateTime().toString("M/d/yyyy - hh:mm:ss"));
        mainScanTreeElem->setText(0, tmp_host);
    } else {
        tmp_host.append(hostCheck + '\n' + QDateTime::currentDateTime().toString("M/d/yyyy - hh:mm:ss"));
        mainScanTreeElem->setText(0, tmp_host);
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
                    parserObjectElem->setPortFiltered(scanBufferToStream_line);
                } else {
                    parserObjectElem->setPortOpen(scanBufferToStream_line);
                }
            } else {
                parserObjectElem->setPortClose(scanBufferToStream_line);
            }

            if (!scanBufferToStream_line.isEmpty()) {
                QString tmpStr = scanBufferToStream_line;
                QStringList lStr = tmpStr.split(' ', QString::SkipEmptyParts);
                parserObjectElem->setServices(lStr[2]); // Obj Services
                parserObjectElem->setPortServices(lStr[0]);
            }
        } // end while

        parserObjectElem->setValidity(true);
    } else {
        mainScanTreeElem->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit_noresult.png")));
        parserObjectElem->setValidity(false);
    }

    QTextStream bufferInfoStream(&bufferInfo); // QString to QtextStrem (scan Tree)
    QString bufferInfoStream_line;

    // check for Host information
    // OS not detected
    bool isOsFound = false;
    while (!bufferInfoStream.atEnd()) {
        bufferInfoStream_line = bufferInfoStream.readLine();
        // Check OS String
        if (bufferInfoStream_line.contains("OS") && !isOsFound) {
            // OS was found ?
            isOsFound = HostTools::checkViewOS(bufferInfoStream_line, mainScanTreeElem);
        }

        parserObjectElem->setMainInfo(bufferInfoStream_line);
    }

    if (mainScanTreeElem->icon(0).isNull()) {
        mainScanTreeElem->setTextAlignment(1, Qt::AlignHCenter | Qt::AlignVCenter);
        mainScanTreeElem->setIcon(0, QIcon(QString::fromUtf8(":/images/images/network_local.png")));
        mainScanTreeElem->setText(1, "Undiscovered");
    }

    QTextStream bufferTraceStream(&bufferTraceroot); // Traceroute buffer
    QString bufferTraceStream_line;

    // check for traceroute scan information
    while (!bufferTraceStream.atEnd()) {
        bufferTraceStream_line = bufferTraceStream.readLine();
        if (!bufferTraceStream_line.isEmpty() && !bufferTraceStream_line.contains("guessing hop")) {
            parserObjectElem->setTraceRouteInfo(bufferTraceStream_line);
        }
    }

    QTextStream nseStream(&nseBuffer);
    QString nseStreamLine = nseStream.readLine();
    QHash<QString, QStringList> nseResult;

    while (!nseStream.atEnd()) {
        QString service;
        QStringList serviceNseResult;

        if (nseStreamLine.startsWith(QLatin1String("|--"))) {
            service = nseStreamLine.remove("|--");
            nseStreamLine = nseStream.readLine();
        }

        while (!nseStreamLine.startsWith(QLatin1String("|--")) && !nseStream.atEnd()) {
            if (!nseStreamLine.isEmpty()) {
                serviceNseResult.append(nseStreamLine);
            }
            nseStreamLine = nseStream.readLine();

            // Save nse vulnerabilies url discovered
            if ((nseStreamLine.startsWith(QLatin1String("http://"))
                    || nseStreamLine.startsWith(QLatin1String("https://")))
                    && !nseStreamLine.contains(hostCheck)
                    && !nseStreamLine.contains("localhost")) {
                parserObjectElem->setVulnDiscoverd(nseStreamLine);
            }
        }

        if (!service.isEmpty() && serviceNseResult.size()) {
            nseResult.insert(service, serviceNseResult);
        }
    }

    // save nse result with QHash
    parserObjectElem->setNseResult(nseResult);
    m_ui->m_collections->m_collectionsScanSection.value("clearHistory-action")->setEnabled(true);

    QTextStream bufferLogStream(&StdoutStr);
    QString bufferLogStream_line;

    // check for full log scan
    while (!bufferLogStream.atEnd()) {
        bufferLogStream_line = bufferLogStream.readLine();
        if (!bufferLogStream_line.isEmpty()) {
            parserObjectElem->setFullScanLog(bufferLogStream_line);
        }
    }

    QTextStream bufferErrorStream(&StderrorStr);
    QString bufferErrorStream_line;

    // check for scan error
    while (!bufferErrorStream.atEnd()) {
        bufferErrorStream_line = bufferErrorStream.readLine();
        parserObjectElem->setErrorScan(bufferErrorStream_line);
    }

    return parserObjectElem;
}

void ParserManager::showParserObj(int hostIndex)
{
    // Clear widget
    memory::freelist<QTreeWidgetItem*>::itemDeleteAll(m_itemListScan);
    memory::freelist<QTreeWidgetItem*>::itemDeleteAll(m_objectItems);
    m_ui->m_scanWidget->listWscan->clear();
    m_ui->m_scanWidget->GItree->clear();
    m_ui->m_scanWidget->treeNSS->clear();
    m_ui->m_vulnerability->m_vulnerabilityWidget->treeVulnNseRecovered->clear();

    // set combo scan parameters
    m_ui->m_scanWidget->comboScanLog->clear();
    m_ui->m_scanWidget->comboScanLog->insertItem(0, m_parserObjList[hostIndex]->getParameters());

    QString noInfo("not Discovered");

    foreach(const QString & token, m_parserObjList[hostIndex]->getMainInfo()) {
        QTreeWidgetItem *root = new QTreeWidgetItem(m_ui->m_scanWidget->treeHostDet);
        m_itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/messagebox_info.png")));
        root->setText(0, token);
        root->setToolTip(0, token);
    }

    QString noDes = tr("No description");
    // clear combo Vulnerabilities
    m_ui->m_vulnerability->m_vulnerabilityWidget->comboVuln->clear();
    m_ui->m_vulnerability->m_vulnerabilityWidget->comboVuln->insertItem(0, "Services");

    // Show open ports
    foreach(const QString & token, m_parserObjList[hostIndex]->getPortOpen()) {
        QTreeWidgetItem *root = new QTreeWidgetItem(m_ui->m_scanWidget->listWscan);
        m_itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/flag_green.png")));
        root->setForeground(0, QBrush(QColor(0, 0, 255, 127)));
        QStringList split = token.split(' ', QString::SkipEmptyParts);
        root->setText(0, split[0]);
        root->setText(1, split[1]);
        root->setText(2, split[2]);
        if (split.size() == 4) {
            root->setText(3, split[3]);
            root->setToolTip(3, split[3]);
            if (!split[3].isEmpty()) {
                m_ui->m_vulnerability->m_vulnerabilityWidget->comboVuln->addItem(split[3]);
            }
        } else if (split.size() > 4) {
            QString lineDescription_("");
            for (int index = 3; index < split.size(); index++) {
                lineDescription_.append(split[index]);
                lineDescription_.append(" ");
            }
            root->setText(3, lineDescription_);
            root->setToolTip(3, lineDescription_);
            //load comboVuln
            if (!lineDescription_.isEmpty()) {
                m_ui->m_vulnerability->m_vulnerabilityWidget->comboVuln->addItem(lineDescription_);
            }
        } else {
            root->setText(3, noDes);
        }
    }

    // Show Close ports
    foreach(const QString & token, m_parserObjList[hostIndex]->getPortClose()) {
        QTreeWidgetItem *root = new QTreeWidgetItem(m_ui->m_scanWidget->listWscan);
        m_itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/flag_red.png")));
        QStringList split = token.split(' ', QString::SkipEmptyParts);
        root->setText(0, split[0]);
        root->setText(1, split[1]);
        root->setText(2, split[2]);
        if (split.size() == 4) {
            root->setText(3, split[3]);
            root->setToolTip(3, split[3]);
            if (!split[3].isEmpty()) {
                m_ui->m_vulnerability->m_vulnerabilityWidget->comboVuln->addItem(split[3]);
            }
        } else if (split.size() > 4) {
            QString lineDescription_("");
            for (int index = 3; index < split.size(); index++) {
                lineDescription_.append(split[index]);
                lineDescription_.append(" ");
            }
            root->setText(3, lineDescription_);
            root->setToolTip(3, lineDescription_);
            //load comboVuln
            if (!lineDescription_.isEmpty()) {
                m_ui->m_vulnerability->m_vulnerabilityWidget->comboVuln->addItem(lineDescription_);
            }
        } else {
            root->setText(3, noDes);
        }
    }

    // Show Filtered ports
    foreach(const QString & token, m_parserObjList[hostIndex]->getPortFiltered()) {
        QTreeWidgetItem *root = new QTreeWidgetItem(m_ui->m_scanWidget->listWscan);
        m_itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/flag_yellow.png")));
        root->setForeground(0, QBrush(QColor(255, 0, 0, 127)));
        QStringList split = token.split(' ', QString::SkipEmptyParts);
        root->setText(0, split[0]);
        root->setText(1, split[1]);
        root->setText(2, split[2]);
        if (split.size() == 4) {
            root->setText(3, split[3]);
            root->setToolTip(3, split[3]);
            if (!split[3].isEmpty()) {
                m_ui->m_vulnerability->m_vulnerabilityWidget->comboVuln->addItem(split[3]);
            }
        } else if (split.size() > 4) {
            QString lineDescription_("");
            for (int index = 3; index < split.size(); index++) {
                lineDescription_.append(split[index]);
                lineDescription_.append(" ");
            }
            root->setText(3, lineDescription_);
            root->setToolTip(3, lineDescription_);
            //load comboVuln
            if (!lineDescription_.isEmpty()) {
                m_ui->m_vulnerability->m_vulnerabilityWidget->comboVuln->addItem(lineDescription_);
            }
        } else {
            root->setText(3, noDes);
        }
    }

    // show services
    int servicesWithDetails = 0;
    foreach(const QString & token, m_parserObjList[hostIndex]->getServices()) {
        if (!m_ui->m_scanWidget->listWscan->findItems(token, Qt::MatchExactly, 2)[0]->text(3).contains(noDes)) {
            QTreeWidgetItem *objItem = new QTreeWidgetItem(m_ui->m_scanWidget->GItree);
            objItem->setSizeHint(0, QSize(22, 22));
            objItem->setIcon(0, QIcon(QString::fromUtf8(":/images/images/network_local.png")));
            m_objectItems.push_front(objItem);
            objItem->setText(0, token);
            servicesWithDetails++;
        }
    }

    if (servicesWithDetails) {
        Notify::startButtonNotify(m_ui->m_collections->m_collectionsButton.value("vuln-sez"));
    } else {
        Notify::clearButtonNotify(m_ui->m_collections->m_collectionsButton.value("vuln-sez"));
    }

    // Show Nse Info
    QHash<QString, QStringList> nseResult = m_parserObjList[hostIndex]->getNseResult();
    QHash<QString, QStringList>::const_iterator i;
    m_ui->m_scanWidget->treeNSS->setRootIsDecorated(true);

    for (i = nseResult.constBegin(); i != nseResult.constEnd(); ++i) {
        QTreeWidgetItem *root = new QTreeWidgetItem(m_ui->m_scanWidget->treeNSS);
        m_itemListScan.push_front(root);
        root->setSizeHint(0, QSize(32, 32));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/traceroute.png")));

        QStringList rootValue = i.key().split(' ', QString::SkipEmptyParts);

        if (rootValue.size() >= 3) {
            root->setText(0, rootValue[0] + ' ' + rootValue[2]);
        } else {
            root->setText(0, rootValue[0]);
        }

        foreach(const QString & value, i.value()) {
            QTreeWidgetItem *item = new QTreeWidgetItem(root);
            m_itemListScan.push_front(item);

            if (value.contains(":") && !value.contains("//")
                    && !value.contains("ERROR")) {
                item->setForeground(0, QBrush(QColor(0, 0, 255, 127)));
            }

            if (value.contains("ERROR")) {
                item->setForeground(0, QBrush(QColor(255, 0, 0, 127)));
            }

            item->setText(0, value);
            item->setToolTip(0, value);
            item->setIcon(0, QIcon(QString::fromUtf8(":/images/images/messagebox_info.png")));
        }
    }

    // Show nse url discovered
    foreach(const QString & url, m_parserObjList[hostIndex]->getVulnDiscoverd()) {
        QTreeWidgetItem *root = new QTreeWidgetItem(m_ui->m_vulnerability->m_vulnerabilityWidget->treeVulnNseRecovered);
        m_itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmag+.png")));
        root->setText(0, url);
        root->setToolTip(0, url);
    }

    // Show full scan log
    foreach(const QString & token, m_parserObjList[hostIndex]->getFullScanLog()) {
        QTreeWidgetItem *root = new QTreeWidgetItem(m_ui->m_scanWidget->listScan);
        m_itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/book.png")));
        root->setText(0, token);
        root->setToolTip(0, token);
    }

    // Show scan error
    foreach(const QString & token, m_parserObjList[hostIndex]->getErrorScan()) {
        QTreeWidgetItem *root = new QTreeWidgetItem(m_ui->m_scanWidget->listScanError);
        m_itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/messagebox_critical.png")));
        root->setText(0, token);
        root->setToolTip(0, token);
    }
}

void ParserManager::showParserObjPlugins(int hostIndex)
{
    // show traceroute
    foreach(const QString & token, m_parserObjList[hostIndex]->getTraceRouteInfo()) {
        QTreeWidgetItem *root = new QTreeWidgetItem(m_ui->m_scanWidget->treeTraceroot);
        m_itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/traceroute.png")));
        QStringList tmpToken = token.split(' ', QString::SkipEmptyParts);

        // MS windows check for ms string
        if (tmpToken.size() == 5) {
            if (tmpToken[2].size() < 4) {
                // minimum dns length
                tmpToken.removeAt(2);
            }
        }

        if (tmpToken.size() == 4) {
            if (tmpToken[2].size() < 4) {
                // minimum dns length
                tmpToken.removeAt(2);
            } else {
                tmpToken[3].remove('(');
                tmpToken[3].remove(')');
            }
        }

        if (tmpToken.size() == 4) {
            root->setText(0, tmpToken[0]);
            root->setText(1, tmpToken[1]);
            root->setText(3, tmpToken[2]);
            root->setText(2, tmpToken[3]);

        } else if (tmpToken.size() == 3) {
            root->setText(0, tmpToken[0]);
            root->setText(1, tmpToken[1]);
            root->setText(2, tmpToken[2]);
            root->setText(3, "no DNS");
            root->setForeground(3, QBrush(QColor(255, 0, 0, 127)));
        } else {
            root->setText(0, token);
            root->setToolTip(0, token);
        }
    }

    // show lookUp info
    foreach(PObjectLookup * elem, m_parserObjUtilList) {
        if (m_parserObjList[hostIndex]->getId() == elem->getId()) {
            foreach(const QString & token, elem->getInfoLookup()) {
                QTreeWidgetItem *root = new QTreeWidgetItem(m_ui->m_scanWidget->treeLookup);
                m_itemListScan.push_front(root);
                root->setSizeHint(0, QSize(22, 22));
                root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit.png")));
                root->setText(0, token);
            }
            break;
        }
    }
}

void ParserManager::callSaveSingleLogWriter()
{
    if (!m_ui->m_scanWidget->treeMain->selectedItems().size()) {
        return;
    }

    int selectedItemsIndex = m_ui->m_scanWidget->treeMain->indexOfTopLevelItem(m_ui->m_scanWidget->treeMain->selectedItems()[0]);
    PObject *object = m_parserObjList[selectedItemsIndex];

    if (!object->isValidObject()) {
        return;
    }

    QSettings settings("nmapsi4", "nmapsi4");
    int logType = settings.value("logType", 0).toInt();

    QString filter;
    if (logType == LogWriter::HtmlLog) {
        filter.append("Html (*.html *.htm)");
    } else {
        filter.append("Log (*.log)");
    }

    const QString& path = QFileDialog::getSaveFileName(
                              m_ui,
                              tr("Save Log"),
                              QDir::homePath() + QDir::toNativeSeparators("/") + "untitled",
                              filter
                          );

    if (!path.isEmpty()) {
        LogWriter *writer = new LogWriter();
        writer->writeSingleLogFile(object, path);
        delete writer;
    }
}

void ParserManager::callSaveAllLogWriter()
{
    if (!m_parserObjList.size()) {
        return;
    }

    const QString& directoryPath = QFileDialog::getExistingDirectory(
                                       m_ui,
                                       tr("Open Directory"),
                                       QDir::homePath(),
                                       QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
                                   );

    if (!directoryPath.isEmpty()) {
        LogWriter *writer = new LogWriter();
        writer->writeAllLogFile(m_parserObjList, directoryPath);
        delete writer;
    }
}
