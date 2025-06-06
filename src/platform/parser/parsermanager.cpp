/*
Copyright 2011-2014  Francesco Cecconi <francesco.cecconi@gmail.com>

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

    connect(m_ui->m_scanWidget->treeMain, &QTreeWidget::itemActivated,
            this, &ParserManager::showParserResult);
    connect(m_ui->m_scanWidget->treeTraceroot, &QTreeWidget::itemActivated,
            this, &ParserManager::showParserTracerouteResult);
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
                              + QString(errorBuffer));
        return;
    }

    // create a scan host item.
    QTreeWidgetItem *scanTreeItem = new QTreeWidgetItem(m_ui->m_scanWidget->treeMain);
    m_treeItems.push_front(scanTreeItem);
    scanTreeItem->setSizeHint(0, QSize(32, 32));

    // call real parser
    PObject* elemObj = parserCore(parList, dataBuffer, errorBuffer, scanTreeItem);

    elemObj->setParameters(parList.join(" "));
    elemObj->setId(id);

    //QString message(tr("Scan completed"));

    // TODO: no action
    //Notify::notificationMessage(parList[parList.size()-1], message);

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

PObject* ParserManager::parserCore(const QStringList parList, QByteArray StdoutStr,
                                   QByteArray StderrorStr, QTreeWidgetItem* mainScanTreeElem)
{
    // Create parser Object
    PObject *parserObjectElem = new PObject();
    QString hostName(parList[parList.size() - 1]);
    parserObjectElem->setHostName(hostName);

    QRegExp portRx(matchPorts);
    QRegExp tracerouteRx(matchTraceroute);

    QStringList infoParserStringList;
    infoParserStringList << "MAC" << "Running" << "OS details:" << "Aggressive OS guesses:"
                     << "OS CPE:" << "Device type:" << "Uptime:" << "Uptime guess:" << "TCP Sequence Prediction:"
                     << "IPID Sequence Generation:" << "IP ID Sequence Generation:" << "Service Info:"
                     << "Initiating Ping " << "Completed Ping " << "Network Distance:" << "Note:"
                     << "Nmap done:" << "Hosts";

    QTextStream stream(&StdoutStr);
    QString tmpBufferLine;
    QString nseBuffer;
    QString bufferInfo;

    while (!stream.atEnd()) {
        tmpBufferLine = stream.readLine();

        if (portRx.indexIn(tmpBufferLine) != -1) {
            if (tmpBufferLine.contains("open") || tmpBufferLine.contains("filtered")
                    || tmpBufferLine.contains("unfiltered")) {

                if (tmpBufferLine.contains("open")) {
                    parserObjectElem->setPortOpen(tmpBufferLine);
                } else {
                    parserObjectElem->setPortFiltered(tmpBufferLine);
                }

            } else {
                parserObjectElem->setPortClose(tmpBufferLine);
            }

            // Insert new elem to nse buffer result
            nseBuffer.append("|--" + tmpBufferLine + '\n');
        }

        if (tmpBufferLine.startsWith(u"Host script results:")) {
            nseBuffer.append("|--" + tmpBufferLine + '\n');
        }

        bool isInfoStringFounded = false;
        // check for specific info
        for (const QString& infoString : std::as_const(infoParserStringList)) {
            if (tmpBufferLine.startsWith(infoString)) {
                bufferInfo.append(tmpBufferLine);
                bufferInfo.append("\n");
                isInfoStringFounded = true;
                break;
            }
        }

        if (!isInfoStringFounded
                && tmpBufferLine.startsWith(u"Host")
                && !tmpBufferLine.contains("Host script results:")
                && !tmpBufferLine.contains("Probes")) {
            bufferInfo.append(tmpBufferLine);
            bufferInfo.append("\n");
        }

        // check for nse subtree service
        if (tmpBufferLine.startsWith(u"|") && !nseBuffer.isEmpty()) {
            QString tmpClean(tmpBufferLine);
            if (tmpClean.startsWith(u"|")) {
                tmpClean.remove('|');
            }

            if (tmpClean.startsWith(u"_")) {
                tmpClean.remove('_');
            }

            int pos;
            while (tmpClean.startsWith(u" ")) {
                // remove space at begin of string
                pos = tmpClean.indexOf(" ");
                if (pos == 0) {
                    tmpClean.remove(pos, 1);
                }
            }

            nseBuffer.append(tmpClean + '\n');
        }

        // collect trace route information
        if ((tracerouteRx.indexIn(tmpBufferLine) != -1) && (!tmpBufferLine.contains("/"))) {
            if (!tmpBufferLine.isEmpty() && !tmpBufferLine.contains("guessing hop")) {
                parserObjectElem->setTraceRouteInfo(tmpBufferLine);
            }
        }

    } // End first While

    parserObjectElem->setScanDate(QDateTime::currentDateTime().toString("M/d/yyyy - hh:mm:ss"));

    mainScanTreeElem->setText(0, hostName + " (" + parserObjectElem->scanDate() + ')');
    mainScanTreeElem->setToolTip(0, startRichTextTags + hostName
                                 + " (" + parserObjectElem->scanDate() + ')' + endRichTextTags);

    QTextStream bufferInfoStream(&bufferInfo); // Host info
    QString bufferInfoStream_line;

    // check for Host information
    bool isOsFound = false;
    bool osGuessesFound = false;
    while (!bufferInfoStream.atEnd()) {
        bufferInfoStream_line = bufferInfoStream.readLine();

        // check for specific device type
        if (bufferInfoStream_line.startsWith(u"Device type:") && bufferInfoStream_line.contains("switch")) {
            mainScanTreeElem->setIcon(0, QIcon(QString::fromUtf8(":/images/images/hub.png")));
        }

        // check for Os guesses string (more specific)
        if (bufferInfoStream_line.startsWith(u"Aggressive OS guesses:")) {

            QString osInfo(bufferInfoStream_line.left(bufferInfoStream_line.indexOf("%")));
            osInfo = osInfo.remove("Aggressive OS guesses:");
            osInfo = osInfo.remove('(');
            osInfo = osInfo.remove(')');
            isOsFound = HostTools::checkViewOS(osInfo, mainScanTreeElem);

            if (!isOsFound && (osInfo.contains("router") || osInfo.contains("switch") || osInfo.contains("hub"))) {
                mainScanTreeElem->setIcon(0, QIcon(QString::fromUtf8(":/images/images/hub.png")));
                osGuessesFound = true;
            }

            if (!osGuessesFound && osInfo.contains("WAP")) {
                mainScanTreeElem->setIcon(0, QIcon(QString::fromUtf8(":/images/images/wap.png")));
                osGuessesFound = true;
            }

            if (!osGuessesFound && osInfo.contains("printer")) {
                mainScanTreeElem->setIcon(0, QIcon(QString::fromUtf8(":/images/images/printer.png")));
                osGuessesFound = true;
            }
            mainScanTreeElem->setToolTip(0, mainScanTreeElem->toolTip(0) + "<br/>" + "Os: " + osInfo + '%');
        }

        // Check OS String (more generic information)
        if (bufferInfoStream_line.contains("OS") && !isOsFound && !osGuessesFound) {
            // OS was found ?
            isOsFound = HostTools::checkViewOS(bufferInfoStream_line, mainScanTreeElem);
        }

        parserObjectElem->setHostInfo(bufferInfoStream_line);
    } // end while

    if (mainScanTreeElem->icon(0).isNull() && !bufferInfo.isEmpty()) {
        mainScanTreeElem->setIcon(0, QIcon(QString::fromUtf8(":/images/images/no-os.png")));
    }

    // set validity of parser object
    if (!bufferInfo.isEmpty()) {
        parserObjectElem->setValidity(true);
    } else {
        parserObjectElem->setValidity(false);
    }

    QTextStream nseStream(&nseBuffer);
    QString nseStreamLine = nseStream.readLine();
    QHash<QString, QStringList> nseResult;

    while (!nseStream.atEnd()) {
        QString service;
        QStringList serviceNseResult;

        if (nseStreamLine.startsWith(u"|--")) {
            service = nseStreamLine.remove("|--");
            nseStreamLine = nseStream.readLine();
        }

        while (!nseStreamLine.startsWith(u"|--") && !nseStream.atEnd()) {
            if (!nseStreamLine.isEmpty()) {
                serviceNseResult.append(nseStreamLine);
            }
            nseStreamLine = nseStream.readLine();

            // Save nse vulnerabilies url discovered
            if ((nseStreamLine.startsWith(u"http://")
                    || nseStreamLine.startsWith(u"https://"))
                    && !nseStreamLine.contains(hostName)
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

    // no result for scan and ip is down
    if (bufferInfo.isEmpty() || (!bufferInfo.isEmpty() && !bufferInfo.contains("Host is up"))) {
        mainScanTreeElem->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit_noresult.png")));
    }

    return parserObjectElem;
}

void ParserManager::showParserResult(QTreeWidgetItem *item, int column)
{
    // SLOT
    Q_UNUSED(column);

    QString hostName_ = item->text(0);
    hostName_  = hostName_.left(hostName_.indexOf(' '));

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

void ParserManager::setPortItem(QTreeWidgetItem* item, const QStringList& details, bool& isPortDescriptionPresent)
{
    Q_ASSERT(details.size() >= 3);

    item->setText(0, details[0]);
    item->setText(1, details[1]);
    item->setText(2, details[2]);

    if (details.size() == 4) {
        item->setText(3, details[3]);
        item->setToolTip(3, details[3]);
        if (!details[3].isEmpty()) {
            m_ui->m_vulnerability->m_vulnerabilityWidget->comboVuln->addItem(details[3]);
        }
    } else if (details.size() > 4) {
        QString lineDescription_("");
        for (int index = 3; index < details.size(); index++) {
            lineDescription_.append(details[index]);
            lineDescription_.append(" ");
        }
        item->setText(3, lineDescription_);
        item->setToolTip(3, lineDescription_);
        //load comboVuln
        if (!lineDescription_.isEmpty()) {
            m_ui->m_vulnerability->m_vulnerabilityWidget->comboVuln->addItem(lineDescription_);
            isPortDescriptionPresent = true;
        }
    }
}

void ParserManager::showParserObj(int hostIndex)
{
    // Clear widget
    memory::freelist<QTreeWidgetItem*>::itemDeleteAll(m_itemListScan);
    m_ui->m_scanWidget->listWscan->clear();
    m_ui->m_scanWidget->treeNSS->clear();
    m_ui->m_vulnerability->m_vulnerabilityWidget->treeVulnNseRecovered->clear();

    // set combo scan parameters
    m_ui->m_scanWidget->comboScanLog->clear();
    m_ui->m_scanWidget->comboScanLog->insertItem(0, m_parserObjList[hostIndex]->getParameters());

    for (const QString& token : m_parserObjList[hostIndex]->getHostInfo()) {
        QTreeWidgetItem *root = new QTreeWidgetItem(m_ui->m_scanWidget->treeHostDet);
        m_itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/messagebox_info.png")));

        if (token.contains("OS") || token.startsWith(u"Device type:")) {
            QFont font = root->font(0);
            font.setBold(true);
            root->setFont(0, font);
            root->setText(0, token);
        } else {
            root->setText(0, token);
        }

        root->setToolTip(0, startRichTextTags + token + endRichTextTags);
    }

    // clear combo Vulnerabilities
    m_ui->m_vulnerability->m_vulnerabilityWidget->comboVuln->clear();
    m_ui->m_vulnerability->m_vulnerabilityWidget->comboVuln->insertItem(0, "Services");

    bool isPortDescriptionPresent = false;
    // Show open ports
    for (const QString& token : m_parserObjList[hostIndex]->getPortOpen()) {
        QTreeWidgetItem *root = new QTreeWidgetItem(m_ui->m_scanWidget->listWscan);
        m_itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/flag_green.png")));
        root->setForeground(0, QBrush(QColor(27, 117, 9)));
        QStringList split = token.split(QChar(' '), Qt::SkipEmptyParts);
        setPortItem(root, split, isPortDescriptionPresent);
    }

    // Show Close ports
    for (const QString& token : m_parserObjList[hostIndex]->getPortClose()) {
        QTreeWidgetItem *root = new QTreeWidgetItem(m_ui->m_scanWidget->listWscan);
        m_itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/flag_red.png")));
        QStringList split = token.split(QChar(' '), Qt::SkipEmptyParts);
        setPortItem(root, split, isPortDescriptionPresent);
    }

    // Show Filtered ports
    for (const QString& token : m_parserObjList[hostIndex]->getPortFiltered()) {
        QTreeWidgetItem *root = new QTreeWidgetItem(m_ui->m_scanWidget->listWscan);
        m_itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/flag_yellow.png")));
        root->setForeground(0, QBrush(QColor(255, 0, 0)));
        QStringList split = token.split(QChar(' '), Qt::SkipEmptyParts);
        setPortItem(root, split, isPortDescriptionPresent);
    }

    if (isPortDescriptionPresent) {
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
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/traceroute.png")));

        QStringList rootValue = i.key().split(QChar(' '), Qt::SkipEmptyParts);

        if (rootValue.size() >= 3) {
            root->setText(0, rootValue[0] + ' ' + rootValue[2]);
        } else {
            root->setText(0, rootValue[0]);
        }

        for (const QString& value : i.value()) {
            QTreeWidgetItem *item = new QTreeWidgetItem(root);
            m_itemListScan.push_front(item);

            if (value.contains(":") && !value.contains("//")
                    && !value.contains("ERROR")) {
                item->setForeground(0, QBrush(QColor(27, 117, 9)));
            }

            if (value.contains("ERROR")) {
                item->setForeground(0, QBrush(QColor(255, 0, 0)));
            }

            item->setText(0, value);
            item->setToolTip(0, startRichTextTags + value + endRichTextTags);
            item->setIcon(0, QIcon(QString::fromUtf8(":/images/images/code-function.png")));
        }
    }

    // Show nse url discovered
    const QStringList& vulnUrlList(m_parserObjList[hostIndex]->getVulnDiscoverd());
    if (!vulnUrlList.size()) {
        m_ui->m_vulnerability->m_vulnerabilityWidget->treeVulnNseRecovered->setVisible(false);
    } else {
        m_ui->m_vulnerability->m_vulnerabilityWidget->treeVulnNseRecovered->setVisible(true);

        QList<int> size = m_ui->m_vulnerability->m_mainHorizontalLeftSplitter->sizes();
        if (!size[0]) {
            size[0] = 100; // ratio
            m_ui->m_vulnerability->m_mainHorizontalLeftSplitter->setSizes(size);
        }
    }

    for (const QString& url : vulnUrlList) {
        QTreeWidgetItem *root = new QTreeWidgetItem(m_ui->m_vulnerability->m_vulnerabilityWidget->treeVulnNseRecovered);
        m_itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmag+.png")));
        root->setText(0, url);
        root->setToolTip(0, url);
    }

    // Show full scan log
    for (const QString& token : m_parserObjList[hostIndex]->getFullScanLog()) {
        QTreeWidgetItem *root = new QTreeWidgetItem(m_ui->m_scanWidget->listScan);
        m_itemListScan.push_front(root);

        if (token.contains("open")) {
            root->setForeground(0, QBrush(QColor(27, 117, 9)));
        } else if (token.contains("closed")) {
            root->setForeground(0, QBrush(QColor(255, 0, 0)));
        } else if (token.contains("filtered") || token.contains("unfiltered")) {
            root->setForeground(0, QBrush(QColor(255, 134, 12)));
        }

        root->setText(0, token);
        root->setToolTip(0, startRichTextTags + token + endRichTextTags);
    }

    // Show scan error
    for (const QString& token : m_parserObjList[hostIndex]->getErrorScan()) {
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
    for (const QString& token : m_parserObjList[hostIndex]->getTraceRouteInfo()) {
        QTreeWidgetItem *root = new QTreeWidgetItem(m_ui->m_scanWidget->treeTraceroot);
        m_itemListScan.push_front(root);
        root->setSizeHint(0, QSize(22, 22));
        root->setIcon(0, QIcon(QString::fromUtf8(":/images/images/traceroute.png")));
        QStringList tmpToken = token.split(QChar(' '), Qt::SkipEmptyParts);

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
            root->setForeground(3, QBrush(QColor(255, 0, 0)));
        } else {
            root->setText(0, token);
            root->setToolTip(0, token);
        }
    }

    // show lookUp info
    for (PObjectLookup * elem : std::as_const(m_parserObjUtilList)) {
        if (m_parserObjList[hostIndex]->getId() == elem->getId()) {
            for (const QString& token : elem->getInfoLookup()) {
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
