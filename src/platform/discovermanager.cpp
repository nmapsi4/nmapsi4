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

#include "discovermanager.h"
#include "mainwindow.h"

#include <QRegExp>

DiscoverWidget::DiscoverWidget(QWidget* parent): QWidget(parent)
{
    setupUi(this);
}

DiscoverManager::DiscoverManager(MainWindow* parent)
    : QObject(parent), m_ui(parent), m_ipCounter(0), m_userid(0), m_discoverIsActive(false)
{

#if !defined(Q_OS_WIN32)
    m_userid = getuid();
#endif

    m_discoverWidget = new DiscoverWidget(m_ui);
    m_discoverWidget->treeDiscover->setColumnWidth(0, 300);

    m_discoverHorizontalSplitter = new QSplitter(m_discoverWidget);
    m_discoverHorizontalSplitter->setOrientation(Qt::Horizontal);
    m_discoverHorizontalSplitter->addWidget(m_discoverWidget->frameDiscoverTree);
    m_discoverHorizontalSplitter->addWidget(m_discoverWidget->frameDiscoverOptions);

    // call discover startup, NPING is REQUIRED
    loadFoundInterfaces();
    defaultDiscoverProbes();

    m_discoverWidget->layout()->addWidget(m_discoverHorizontalSplitter);

    QSettings settings("nmapsi4", "nmapsi4");

    if (!settings.value("discoverHorizontalSplitter").toByteArray().isEmpty()) {
        m_discoverHorizontalSplitter->restoreState(settings.value("discoverHorizontalSplitter").toByteArray());
    }

    m_discoverWidget->treeTracePackets->setIconSize(QSize(22, 22));
    m_discoverWidget->treeDiscover->setIconSize(QSize(22, 22));

    connect(m_discoverWidget->comboDiscover, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::textActivated),
            this, &DiscoverManager::discoverIp);

    connect(m_discoverWidget->startDiscoverButt, &QPushButton::clicked,
            this, &DiscoverManager::startDiscoverIpsFromRange);
    connect(m_discoverWidget->cidrButton, &QPushButton::clicked,
            this, &DiscoverManager::startDiscoverIpsFromCIDR);
    connect(m_discoverWidget->stopDiscoverButt, &QPushButton::clicked,
            this, &DiscoverManager::stopDiscoverFromIpsRange);
    connect(m_discoverWidget->stopDiscoverCidrButton, &QPushButton::clicked,
            this, &DiscoverManager::stopDiscoverFromCIDR);
    connect(m_discoverWidget->treeDiscover, &QTreeWidget::itemClicked,
            this, &DiscoverManager::runtimeScanDiscover);
    connect(m_discoverWidget->reloadComboDiscover, &QPushButton::clicked,
            this, &DiscoverManager::loadFoundInterfaces);
    connect(m_discoverWidget->discoverCIDRPrefixSizeSpin, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &DiscoverManager::calculateAddressFromCIDR);
    connect(m_discoverWidget->discoverCIDRPasteCombo->lineEdit(), &QLineEdit::textChanged,
            this, &DiscoverManager::splitCIDRAddressPasted);

    calculateAddressFromCIDR();
}

DiscoverManager::~DiscoverManager()
{
    memory::freelist<Discover*>::itemDeleteAll(m_listDiscover);
}

void DiscoverManager::syncSettings()
{
    QSettings settings("nmapsi4", "nmapsi4");
    settings.setValue("discoverHorizontalSplitter", m_discoverHorizontalSplitter->saveState());
}

bool DiscoverManager::activeIpContains(const QString ipAddress)
{
    for (QTreeWidgetItem * item : std::as_const(m_listTreeItemDiscover)) {
        if (item->text(0) == ipAddress) {
            return true;
        }
    }

    return false;
}

void DiscoverManager::loadFoundInterfaces()
{
    // take local interfaces
    m_discoverWidget->comboDiscover->clear();
    m_discoverWidget->comboDiscover->insertItem(0, "Select Interface");

    Discover *discoverPtr = new Discover(m_userid);

    auto interfaces = discoverPtr->getAllInterfaces(Discover::AllInterfaceWithAddress);

    for (const QNetworkInterface & interface : std::as_const(interfaces)) {
        if (!interface.flags().testFlag(QNetworkInterface::IsLoopBack)) {
            //TODO: readable name for windows - humanReadableName()
            //ID is configurable on MS windows.
            m_discoverWidget->comboDiscover->insertItem(1, interface.name());
        }
    }

    delete discoverPtr;
}

void DiscoverManager::discoverIp(const QString& interface)
{
    // ip from interface and discover ip range
    Discover *discover_ = new Discover(m_userid);

    QList<QNetworkAddressEntry> entryList_ = discover_->getAddressEntries(interface);

    if (!entryList_.isEmpty()) {
        QNetworkAddressEntry entry_ = discover_->getAddressEntries(interface).constFirst();
        QString ipAdressString = entry_.ip().toString();

        QHostAddress address(ipAdressString);

        // NOTE:: this range mode is not usable with ipv6 at moment.
        if (!ipAdressString.contains("127.0.0.1") && address.protocol() != QAbstractSocket::IPv6Protocol) {
            // active discover buttton
            m_discoverWidget->startDiscoverButt->setEnabled(true);
            QStringList ipAdressSplitted = ipAdressString.split('.');
            int ipStart = ipAdressSplitted[3].toInt();
            ipAdressSplitted.removeLast();
            m_discoverWidget->discoverIpFirstSpin->setValue(ipAdressSplitted[0].toInt());
            m_discoverWidget->discoverIpSecondSpin->setValue(ipAdressSplitted[1].toInt());
            m_discoverWidget->discoverIpThreeSpin->setValue(ipAdressSplitted[2].toInt());
            m_discoverWidget->spinBeginDiscover->setValue(ipStart);
            m_discoverWidget->spinEndDiscover->setValue(ipStart + 10);
        } else {
            // reset discover value
            resetDiscoverfromRangeValues();
        }
    } else {
        // reset discover value
        resetDiscoverfromRangeValues();;
    }

    delete discover_;
}

void DiscoverManager::resetDiscoverfromRangeValues()
{
    m_discoverWidget->discoverIpFirstSpin->setValue(192);
    m_discoverWidget->discoverIpSecondSpin->setValue(168);
    m_discoverWidget->discoverIpThreeSpin->setValue(1);
    m_discoverWidget->spinBeginDiscover->setValue(1);
    m_discoverWidget->spinEndDiscover->setValue(15);
    m_discoverWidget->startDiscoverButt->setEnabled(true);
}

void DiscoverManager::startDiscoverIpsFromRange()
{
    Notify::startButtonNotify(m_ui->m_collections->m_collectionsButton.value("discover-sez"));
    // disable start discover button
    m_discoverWidget->startDiscoverButt->setEnabled(false);
    m_discoverWidget->stopDiscoverButt->setEnabled(true);
    m_discoverWidget->cidrButton->setEnabled(false);
    // clear tree discover
    clearDiscover();

    QStringList addressList;
    for (int index = m_discoverWidget->spinBeginDiscover->value(); index <= m_discoverWidget->spinEndDiscover->value(); ++index) {
        QString tmpIpAddress = QString::number(m_discoverWidget->discoverIpFirstSpin->value())
                               + '.'
                               + QString::number(m_discoverWidget->discoverIpSecondSpin->value())
                               + '.'
                               + QString::number(m_discoverWidget->discoverIpThreeSpin->value())
                               + '.'
                               + QString::number(index);

        addressList.append(tmpIpAddress);
    }

    QStringList parameters;
    parameters << m_discoverWidget->discoverProbesCombo->currentText();

    Discover *discoverPtr = new Discover(m_userid);
    m_listDiscover.push_back(discoverPtr);

    connect(discoverPtr, &Discover::fromListFinisched,
            this, &DiscoverManager::endDiscoverIpsFromRange);

    m_discoverIsActive = true;
    m_ui->m_collections->m_collectionsDiscover.value("load-ips")->setEnabled(false);
    /*
     * TODO: check nping with QT5 QStandardPaths::findExecutable.
     *
     */
    discoverPtr->fromList(addressList, this, parameters);
    m_ipCounter = addressList.size();
    m_discoverWidget->discoverProgressBar->setMaximum(0);
}

void DiscoverManager::endDiscoverIpsFromRange(const QStringList hostname, bool state, const QByteArray callBuff)
{
    // decrement ping ip counter
    if (m_discoverIsActive) {
        --m_ipCounter;
    }

    // set values in treeDiscover widget
    m_discoverWidget->treeDiscover->setIconSize(QSize(24, 24));
    QTextStream stream(callBuff);

    if (state) {
        // active scan all action
        m_ui->m_collections->m_collectionsDiscover.value("scan-all")->setEnabled(true);
        m_ui->m_collections->m_collectionsDiscover.value("save-ips")->setEnabled(true);
        m_ui->m_collections->m_collectionsDiscover.value("load-ips")->setEnabled(true);

        QTreeWidgetItem *item = new QTreeWidgetItem(m_discoverWidget->treeDiscover);
        item->setIcon(0, QIcon(QString::fromUtf8(":/images/images/flag_green.png")));
        m_listTreeItemDiscover.push_back(item);
        item->setText(0, hostname[hostname.size() - 1]);
        item->setText(1, tr("is Up"));

        while (!stream.atEnd()) {
            QString line = stream.readLine();
            if (((line.startsWith(u"RECV") && line.contains("completed")) || line.startsWith(u"RCVD"))
                    && line.contains(hostname[hostname.size() - 1]) && !line.contains(u"No route to host")) {
                QTreeWidgetItem *packet = new QTreeWidgetItem(m_discoverWidget->treeTracePackets);
                packet->setText(0, line);
                //packet->setBackground(0, QBrush(QColor(163, 224, 163)));  QBrush(QColor(27, 117, 9)
                packet->setBackground(0, QBrush(QColor(27, 117, 9)));
                packet->setToolTip(0, startRichTextTags + line + endRichTextTags);
                packet->setIcon(0, QIcon(QString::fromUtf8(":/images/images/document-preview-archive.png")));
                m_listTreePackets.push_back(packet);
            } else if (line.startsWith(u"SENT") && line.contains(hostname[hostname.size() - 1])) {
                QTreeWidgetItem *packet = new QTreeWidgetItem(m_discoverWidget->treeTracePackets);
                packet->setText(0, line);
                packet->setToolTip(0, startRichTextTags + line + endRichTextTags);
                packet->setIcon(0, QIcon(QString::fromUtf8(":/images/images/document-preview-archive.png")));
                m_listTreePackets.push_back(packet);
            }
        }
    } else {
        // NOTE: host is down
        //qDebug() << "DEBUG:: " << hostname[1] << " Ip is Up:: " << state;
    }

    if (!m_ipCounter) {
        memory::freelist<Discover*>::itemDeleteAll(m_listDiscover);
        m_discoverWidget->startDiscoverButt->setEnabled(true);
        m_discoverWidget->stopDiscoverButt->setEnabled(false);
        m_discoverWidget->cidrButton->setEnabled(true);
        m_discoverWidget->discoverProgressBar->setMaximum(100);
        Notify::clearButtonNotify(m_ui->m_collections->m_collectionsButton.value("discover-sez"));

        //QString message("> " + tr("Discover completed"));

        // NOTE: no action
        //Notify::notificationMessage("Discover (RANGE)", message);
    }
}

void DiscoverManager::clearDiscover()
{
    memory::freelist<QTreeWidgetItem*>::itemDeleteAll(m_listTreeItemDiscover);
    memory::freelist<QTreeWidgetItem*>::itemDeleteAll(m_listTreePackets);
    memory::freelist<Discover*>::itemDeleteAll(m_listDiscover);

    m_ui->m_collections->m_collectionsDiscover.value("scan-single")->setEnabled(false);
    m_ui->m_collections->m_collectionsDiscover.value("scan-all")->setEnabled(false);
    m_ui->m_collections->m_collectionsDiscover.value("save-ips")->setEnabled(false);
    m_ui->m_collections->m_collectionsDiscover.value("load-ips")->setEnabled(true);
}

void DiscoverManager::stopDiscoverFromIpsRange()
{
    m_discoverWidget->startDiscoverButt->setEnabled(true);
    m_discoverWidget->stopDiscoverButt->setEnabled(false);
    m_discoverWidget->discoverProgressBar->setMaximum(100);
    m_discoverWidget->cidrButton->setEnabled(true);
    m_ipCounter = 0;
    m_discoverIsActive = false;
    Notify::clearButtonNotify(m_ui->m_collections->m_collectionsButton.value("discover-sez"));
    //emit signal
    emit killDiscoverFromIpsRange();
}

void DiscoverManager::scanSingleDiscoveredIp()
{
    if (m_discoverWidget->treeDiscover->currentItem()) {
        startSelectProfilesDialog();
        Notify::startButtonNotify(m_ui->m_collections->m_collectionsButton.value("scan-sez"));

        auto items = m_discoverWidget->treeDiscover->selectedItems();

        for (QTreeWidgetItem * item : std::as_const(items)) {
            m_ui->updateComboHostnameProperties();
            m_ui->m_scanWidget->hostEdit->insertItem(0, item->text(0));
            m_ui->startScan();
        }
    }
}

void DiscoverManager::scanAllDiscoveredIps()
{
    if (m_listTreeItemDiscover.size()) {
        startSelectProfilesDialog();
        Notify::startButtonNotify(m_ui->m_collections->m_collectionsButton.value("scan-sez"));

        for (QTreeWidgetItem * item : std::as_const(m_listTreeItemDiscover)) {
            m_ui->updateComboHostnameProperties();
            m_ui->m_scanWidget->hostEdit->insertItem(0, item->text(0));
            m_ui->startScan();
        }
    }
}

void DiscoverManager::startSelectProfilesDialog()
{
    SelectProfileDialog* profileDialg = new SelectProfileDialog(m_ui);

    profileDialg->exec();

    if (profileDialg) {
        delete profileDialg;
    }
}

void DiscoverManager::runtimeScanDiscover()
{
    if (!m_ui->m_collections->m_collectionsDiscover.value("scan-single")->isEnabled()) {
        m_ui->m_collections->m_collectionsDiscover.value("scan-single")->setEnabled(true);
    }
}

void DiscoverManager::defaultDiscoverProbes()
{
    /* Modes Probe
    * --tcp,--udp,--arp,--tr
    */
    if (!m_userid) {
        m_discoverWidget->discoverProbesCombo->insertItem(0, "--icmp");
        m_discoverWidget->discoverProbesCombo->insertItem(1, "--tcp");
        m_discoverWidget->discoverProbesCombo->insertItem(2, "--udp");
        m_discoverWidget->discoverProbesCombo->insertItem(3, "--arp");
        m_discoverWidget->discoverProbesCombo->insertItem(4, "--tr");
        m_discoverWidget->discoverProbesCombo->insertItem(5, "--tcp-connect");
    } else {
        m_discoverWidget->discoverProbesCombo->insertItem(0, "--tcp-connect");
    }
}

void DiscoverManager::startDiscoverIpsFromCIDR()
{
    Notify::startButtonNotify(m_ui->m_collections->m_collectionsButton.value("discover-sez"));
    // disable CIDR button but also ip range discover
    m_discoverWidget->cidrButton->setEnabled(false);
    m_discoverWidget->startDiscoverButt->setEnabled(false);
    m_discoverWidget->stopDiscoverCidrButton->setEnabled(true);
    // clear tree discover
    clearDiscover();

    QStringList parameters;
    if (!m_userid) {
        parameters.append(m_discoverWidget->discoverProbesCombo->currentText());
    } else {
        parameters.append("--tcp-connect");
    }

    Discover *discoverPtr = new Discover(m_userid);
    m_listDiscover.push_back(discoverPtr);

    connect(discoverPtr, &Discover::cidrCurrentValue,
            this, &DiscoverManager::currentDiscoverIpsFromCIDR);
    connect(discoverPtr, &Discover::cidrFinisced,
            this, &DiscoverManager::endDiscoverIpsFromCIDR);

    const QString& cidrAddress = QString::number(m_discoverWidget->discoverCIDRFirstSpin->value())
                                 + '.'
                                 + QString::number(m_discoverWidget->discoverCIDRSecondSpin->value())
                                 + '.'
                                 + QString::number(m_discoverWidget->discoverCIDRThirdSpin->value())
                                 + '.'
                                 + QString::number(m_discoverWidget->discoverCIDRFourthSpin->value())
                                 + '/'
                                 + QString::number(m_discoverWidget->discoverCIDRPrefixSizeSpin->value());

    m_ui->m_collections->m_collectionsDiscover.value("load-ips")->setEnabled(false);

    m_discoverWidget->discoverProgressBar->setMaximum(0);
    // TODO: check nping with QT5 QStandardPaths::findExecutable.
    discoverPtr->fromCIDR(cidrAddress, parameters, this, Discover::IPv4);
}

void DiscoverManager::endDiscoverIpsFromCIDR()
{
    // restore default button state.
    m_ui->m_collections->m_collectionsDiscover.value("scan-all")->setEnabled(true);
    m_ui->m_collections->m_collectionsDiscover.value("save-ips")->setEnabled(true);
    m_ui->m_collections->m_collectionsDiscover.value("load-ips")->setEnabled(true);
    m_discoverWidget->cidrButton->setEnabled(true);
    m_discoverWidget->stopDiscoverCidrButton->setEnabled(false);
    m_discoverWidget->startDiscoverButt->setEnabled(true);
    m_discoverWidget->discoverProgressBar->setMaximum(100);
    Notify::clearButtonNotify(m_ui->m_collections->m_collectionsButton.value("discover-sez"));

    //QString message("> " + tr("Discover completed"));

    // NOTE: no action
    //Notify::notificationMessage("Discover (CIDR)", message);
}

void DiscoverManager::currentDiscoverIpsFromCIDR(const QString parameters, const QString data)
{
    Q_UNUSED(parameters);

    QRegExp ipv4(matchIpv4);

    if ((data.startsWith(u"RECV") && data.contains("completed")) || (data.startsWith(u"RCVD") && !data.contains(u"No route to host"))) {
        qDebug() << "CIDR::Discover::CIDR::Current::UP:: " << data;
        QTreeWidgetItem *packet = new QTreeWidgetItem(m_discoverWidget->treeTracePackets);
        m_listTreePackets.push_back(packet);
        //packet->setBackground(0, QBrush(QColor(163, 224, 163)));
        packet->setBackground(0, QBrush(QColor(27, 117, 9)));
        packet->setText(0, data);
        packet->setToolTip(0, startRichTextTags + data + endRichTextTags);
        packet->setIcon(0, QIcon(QString::fromUtf8(":/images/images/document-preview-archive.png")));

        int positionMatched = 0;

        QStringList matched;
        while ((positionMatched = ipv4.indexIn(data, positionMatched)) != -1) {
            matched << ipv4.cap(0);
            positionMatched += ipv4.matchedLength();
        }

        if (matched.size() && !activeIpContains(matched.first())) {
            QTreeWidgetItem *ipItem = new QTreeWidgetItem(m_discoverWidget->treeDiscover);
            m_listTreeItemDiscover.push_back(ipItem);
            ipItem->setIcon(0, QIcon(QString::fromUtf8(":/images/images/flag_green.png")));
            ipItem->setText(0, matched.first());
            ipItem->setText(1, QDateTime::currentDateTime().toString("MMMM d yyyy - hh:mm:ss"));
        }
    } else {
        QTreeWidgetItem *packet = new QTreeWidgetItem(m_discoverWidget->treeTracePackets);
        m_listTreePackets.push_back(packet);

        if (data.startsWith(u"SENT")) {
            //packet->setBackground(0, QBrush(QColor(221, 224, 163)));
            packet->setBackground(0, QBrush(QColor(255, 134, 12)));
        }

        packet->setText(0, data);
        packet->setToolTip(0, startRichTextTags + data + endRichTextTags);
        packet->setIcon(0, QIcon(QString::fromUtf8(":/images/images/document-preview-archive.png")));
    }
}

void DiscoverManager::stopDiscoverFromCIDR()
{
    emit killDiscoverFromCIDR();
    Notify::clearButtonNotify(m_ui->m_collections->m_collectionsButton.value("discover-sez"));
}

void DiscoverManager::calculateAddressFromCIDR()
{
    int exp = 32 - m_discoverWidget->discoverCIDRPrefixSizeSpin->value();
    int numberOfIps = 2;

    while (exp >= 2) {
        numberOfIps *= 2;
        exp--;
    }

    if (!exp) {
        numberOfIps = 1;
    }

    if (m_discoverWidget->discoverCIDRPrefixSizeSpin->value() >= 23) {
        m_discoverWidget->lineAddressNumber->setStyleSheet(positiveBackground);
    } else if (m_discoverWidget->discoverCIDRPrefixSizeSpin->value() >= 19) {
        m_discoverWidget->lineAddressNumber->setStyleSheet(highIpNumberBackground);
    } else {
        m_discoverWidget->lineAddressNumber->setStyleSheet(negativeBackground);
    }

    m_discoverWidget->lineAddressNumber->setText(QString::number(numberOfIps));
}

void DiscoverManager::splitCIDRAddressPasted()
{
    const QString& cidrAddress = m_discoverWidget->discoverCIDRPasteCombo->lineEdit()->text();

    if (cidrAddress.isEmpty()) {
        m_discoverWidget->discoverCIDRPasteCombo->lineEdit()->setStyleSheet(neutralBackground);
        return;
    }

    QStringList addressPart = cidrAddress.split(QChar('/'), Qt::SkipEmptyParts);

    if (addressPart.size() != 2) {
        m_discoverWidget->discoverCIDRPasteCombo->lineEdit()->setStyleSheet(negativeBackground);
        return;
    }

    QStringList network = addressPart[0].split(QChar('.'), Qt::SkipEmptyParts);

    if (network.size() != 4 || network[0].toInt() > 255
            || network[1].toInt() > 255
            || network[2].toInt() > 255
            || network[3].toInt() > 255) {
        m_discoverWidget->discoverCIDRPasteCombo->lineEdit()->setStyleSheet(negativeBackground);
        return;
    }

    bool ok;
    addressPart[1].toInt(&ok);
    if (!ok ||  addressPart[1].toInt() > 32) {
        // not an integer
        m_discoverWidget->discoverCIDRPasteCombo->lineEdit()->setStyleSheet(negativeBackground);
        return;
    }

    m_discoverWidget->discoverCIDRPasteCombo->lineEdit()->setStyleSheet(positiveBackground);

    m_discoverWidget->discoverCIDRFirstSpin->setValue(network[0].toInt());
    m_discoverWidget->discoverCIDRSecondSpin->setValue(network[1].toInt());
    m_discoverWidget->discoverCIDRThirdSpin->setValue(network[2].toInt());
    m_discoverWidget->discoverCIDRFourthSpin->setValue(network[3].toInt());
    m_discoverWidget->discoverCIDRPrefixSizeSpin->setValue(addressPart[1].toInt());

}

void DiscoverManager::saveXmlIpsList()
{
    const QString& path = QFileDialog::getSaveFileName(
                              m_discoverWidget,
                              tr("Save IP list"),
                              QDir::homePath() + QDir::toNativeSeparators("/") + "untitled.xml",
                              "xml (*.xml)"
                          );

    if (!path.isEmpty()) {
        LogWriterXml *xmlWriter = new LogWriterXml();
        if (!xmlWriter->writeXmlDiscoverLog(path, m_discoverWidget->treeDiscover)) {
            qWarning() << "LOG:Writer:Xml:: file not writable.";
        }
        delete xmlWriter;
    }
}

void DiscoverManager::loadXmlIpsList()
{
    const QString& path = QFileDialog::getOpenFileName(
                              m_discoverWidget,
                              tr("Load IP list"),
                              QDir::homePath() + QDir::toNativeSeparators("/"),
                              "xml (*.xml)"
                          );

    if (!path.isEmpty()) {
        clearDiscover();
        LogWriterXml *xmlWriter = new LogWriterXml();
        m_listTreeItemDiscover = xmlWriter->readXmlDiscoverLog(path, m_discoverWidget->treeDiscover);
        delete xmlWriter;

        if (!m_listTreeItemDiscover.isEmpty()) {
            m_ui->m_collections->m_collectionsDiscover.value("scan-all")->setEnabled(true);
            m_ui->m_collections->m_collectionsDiscover.value("save-ips")->setEnabled(true);
        }
    }
}
