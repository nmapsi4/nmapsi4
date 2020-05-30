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

#include "monitor.h"
#include "mainwindow.h"

#if !defined(Q_OS_WIN32) && !defined(Q_OS_MAC)
#include "nmapsi4adaptor.h"
#endif

MonitorWidget::MonitorWidget(QWidget* parent): QWidget(parent)
{
    setupUi(this);
}

Monitor::Monitor(MainWindow* parent)
    : QObject(parent), m_ui(parent), m_idCounter(0)
{
#if !defined(Q_OS_WIN32) && !defined(Q_OS_MAC)
    new Nmapsi4Adaptor(this);
    // NOTE: with full mode It is registrered into root dbus session
    QDBusConnection dbus = QDBusConnection::sessionBus();
//     if (!dbus.isConnected())
//     {
//         dbus = QDBusConnection::systemBus();
//     }
    dbus.registerObject("/Nmapsi4", this);
    bool value = dbus.registerService("org.nmapsi4.Nmapsi4");

    if (!value) {
        qDebug() << "DBUS:: error with dbus connection";
    }
#endif

    m_monitorWidget = new MonitorWidget(m_ui);
    m_monitorWidget->scanMonitor->setColumnWidth(0, 300);
    m_monitorWidget->scanMonitor->setColumnWidth(1, 350);
    m_monitorWidget->scanMonitor->setIconSize(QSize(22, 22));
    m_monitorWidget->scanMonitor->header()->setSectionResizeMode(QHeaderView::Interactive);

    updateMaxParallelScan();

    m_isHostcached = false;
    m_timer = new QTimer(this);

    connect(m_monitorWidget->monitorStopCurrentScanButt, &QPushButton::clicked,
            this, &Monitor::stopSelectedScan);
    connect(m_monitorWidget->monitorDetailsScanButt, &QPushButton::clicked,
            this, &Monitor::showSelectedScanDetails);
    connect(m_monitorWidget->monitorStopAllScanButt, &QPushButton::clicked,
            this, &Monitor::stopAllScan);
    connect(m_monitorWidget->scanMonitor, &QTreeWidget::itemSelectionChanged,
            this, &Monitor::monitorRuntimeEvent);
}

Monitor::~Monitor()
{
    memory::freemap<QString, ProcessThread*>::itemDeleteAllWithWait(m_scanThreadHashList);
    memory::freelist<LookupManager*>::itemDeleteAllWithWait(m_internealLookupList);
    memory::freelist<DigManager*>::itemDeleteAll(m_digLookupPointersList);

    if (m_timer->isActive()) {
        m_timer->stop();
    }

    delete m_timer;
}

bool Monitor::isHostOnMonitor(const QString hostname)
{
    QList<QTreeWidgetItem*>::const_iterator i;
    for (i = m_monitorTreeWidgetItemsList.constBegin(); i != m_monitorTreeWidgetItemsList.constEnd(); ++i) {
        if ((*i)->text(0) == hostname) {
            return true;
        }
    }

    return false;
}

int Monitor::monitorHostNumber()
{
    return m_monitorTreeWidgetItemsList.size();
}

void Monitor::addMonitorHost(const QString hostName, const QStringList parameters, LookupType option)
{
    QTreeWidgetItem *hostThread = new QTreeWidgetItem(m_monitorWidget->scanMonitor);
    hostThread->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit.png")));
    hostThread->setText(0, hostName);
    hostThread->setText(1, parameters.join(" "));
    hostThread->setIcon(2, QIcon::fromTheme("media-playback-start",
                                            QIcon(":/images/images/media-playback-start.png")));
    hostThread->setText(2, "Scanning");
    m_monitorTreeWidgetItemsList.push_front(hostThread);
    // start indeterminate progress bar
    m_monitorWidget->scanProgressBar->setMaximum(0);

    emit monitorUpdated(monitorHostNumber());

    // Start Scan for host
    m_hostIdList.insert(hostName, m_idCounter);
    ++m_idCounter;

    cacheScan(hostName, parameters, option, hostThread);
}

void Monitor::cacheScan(const QString& hostname, const QStringList& parameters, LookupType option, QTreeWidgetItem *item)
{
    if (m_parallelThreadLimitValue) {
        m_parallelThreadLimitValue--;

        startScan(hostname, parameters);
        startLookup(hostname, option);
    } else {
        /**
         * Create a cache Item
         */
        QPair<QString, QStringList> firstScanPair(hostname, parameters);
        QPair<LookupType, QTreeWidgetItem*> seconScanPair(option, item);

        m_firstScanCacheList.append(firstScanPair);
        m_secondScanCacheList.append(seconScanPair);

        item->setText(2, tr("Waiting"));
        item->setIcon(2, QIcon::fromTheme("media-playback-pause",
                                          QIcon(":/images/images/media-playback-pause.png")));
        qDebug() << "Monitor::cached:: " << hostname;
    }

    if (m_firstScanCacheList.size() && !m_isHostcached) {
        m_isHostcached = true;
        connect(m_timer, &QTimer::timeout, this, &Monitor::cacheRepeat);

        if (!m_timer->isActive()) {
            m_timer->start(5000);
        }
    }
}

void Monitor::cacheRepeat()
{
    if (!m_parallelThreadLimitValue) {
        return;
    }

    disconnect(this, SLOT(cacheRepeat()));

    m_isHostcached = false;

    m_timer->stop();
    int freeThreadSpace = 1;

    while (freeThreadSpace <= m_parallelThreadLimitValue && freeThreadSpace <= m_firstScanCacheList.size()) {
        QPair<QString, QStringList> firstScanPair = m_firstScanCacheList.takeFirst();
        QPair<LookupType, QTreeWidgetItem*> secondScanPair = m_secondScanCacheList.takeFirst();
        secondScanPair.second->setText(2, tr("Scanning"));
        secondScanPair.second->setIcon(2, QIcon::fromTheme("media-playback-start",
                                   QIcon(":/images/images/media-playback-start.png")));

        qDebug() << "Monitor::restored:: " << firstScanPair.first;

        cacheScan(firstScanPair.first, firstScanPair.second, secondScanPair.first, secondScanPair.second);
        freeThreadSpace++;
    }

}

void Monitor::startScan(const QString hostname, QStringList parameters)
{
    parameters.append(hostname); // add hostname

    // start scan Thread
    QPointer<ProcessThread> thread = new ProcessThread("nmap", parameters);
    m_scanThreadHashList.insert(hostname, thread);
    // read current data scan from the thread
    connect(thread, &ProcessThread::flowFromThread,
            this, &Monitor::readFlowFromThread);
    // read scan data return
    connect(thread, &ProcessThread::dataIsReady,
            this, &Monitor::scanFinisced);
    // start scan
    thread->start();
}

void Monitor::startLookup(const QString hostname, LookupType option)
{
    if (option == DisabledLookup || !HostTools::isDns(hostname)) {
        return;
    }

    if (option == InternalLookup) {
        LookupManager *internalLookupPtr = new LookupManager(hostname);
        m_internealLookupList.push_back(internalLookupPtr);

        connect(internalLookupPtr, &LookupManager::threadEnd,
                this, &Monitor::lookupFinisced);

        internalLookupPtr->start();
    } else {
        PObjectLookup* tmpParserObj_ = new PObjectLookup();

        DigManager *digManager = new DigManager();
        m_digLookupPointersList.push_back(digManager);

        QSettings settings("nmapsi4", "nmapsi4");

        switch (settings.value("digVerbosityLevel", 0).toInt()) {
        case 0:
            digManager->digRequest(hostname, tmpParserObj_, DigManager::Short);
            break;
        case 1:
            digManager->digRequest(hostname, tmpParserObj_, DigManager::Verbose);
            break;
        }

        tmpParserObj_->setId(m_hostIdList.value(hostname));
        m_ui->m_parser->addUtilObject(tmpParserObj_);
    }
}

void Monitor::scanFinisced(const QStringList parameters, QByteArray errorBuffer)
{
    QString hostName(parameters[parameters.size() - 1]);

    /*
     * Remove host scan finisced from the monitor list.
     */
    delMonitorHost(hostName);

    m_parallelThreadLimitValue++;

    /*
     * Start Scan parser
     */
    m_ui->m_parser->startParser(parameters,
                                m_scanHashListRealtime.value(hostName).first,
                                errorBuffer,
                                m_hostIdList.value(hostName));
}

void Monitor::lookupFinisced(QHostInfo info, int state, const QString hostname)
{
    if (state == -1) {
        qWarning() << "Monitor:: Wrong Address for lookUp";
        return;
    }

    PObjectLookup* elemObjUtil = new PObjectLookup();

    elemObjUtil->setHostName(hostname);
    const int infoSize_ = info.addresses().size();
    for (int index = 0; index < infoSize_; index++) {
        elemObjUtil->setInfoLookup(info.addresses()[index].toString());
    }

    elemObjUtil->setId(m_hostIdList.value(hostname));
    m_ui->m_parser->addUtilObject(elemObjUtil);
}

void Monitor::delMonitorHost(const QString hostName)
{
    for (int i = 0; i < m_monitorTreeWidgetItemsList.size(); i++) {
        if (m_monitorTreeWidgetItemsList[i]->text(0) == hostName) {
            // remove host from monitor and list.
            delete m_monitorTreeWidgetItemsList.takeAt(i);
            break;
        }
    }

    emit monitorUpdated(monitorHostNumber());
}

void Monitor::updateMonitorHost(const QString hostName, int valueIndex, const QString newData)
{
    Q_ASSERT(valueIndex < m_monitorWidget->scanMonitor->columnCount());

    QList<QTreeWidgetItem*>::const_iterator i;
    for (i = m_monitorTreeWidgetItemsList.constBegin(); i != m_monitorTreeWidgetItemsList.constEnd(); ++i) {
        if ((*i)->text(0) == hostName) {
            (*i)->setText(valueIndex, newData);
        }

        break;
    }
}

void Monitor::clearHostMonitor()
{
    memory::freemap<QString, ProcessThread*>::itemDeleteAllWithWait(m_scanThreadHashList);
    memory::freelist<LookupManager*>::itemDeleteAllWithWait(m_internealLookupList);
    memory::freelist<DigManager*>::itemDeleteAll(m_digLookupPointersList);

    if (m_timer->isActive()) {
        m_timer->stop();
    }

    m_firstScanCacheList.clear();
    m_secondScanCacheList.clear();

    m_isHostcached = false;
    updateMaxParallelScan();

    memory::freelist<QTreeWidgetItem*>::itemDeleteAll(m_monitorTreeWidgetItemsList);
    m_hostIdList.clear();
}

void Monitor::updateMaxParallelScan()
{
    QSettings settings("nmapsi4", "nmapsi4");
    m_parallelThreadLimitValue = settings.value("maxParallelScan", 5).toInt();
}

void Monitor::monitorRuntimeEvent()
{
    if (!m_monitorWidget->monitorStopCurrentScanButt->isEnabled()) {
        m_monitorWidget->monitorStopCurrentScanButt->setEnabled(true);
    }

    if (!m_monitorWidget->monitorDetailsScanButt->isEnabled()) {
        m_monitorWidget->monitorDetailsScanButt->setEnabled(true);
    }
}

void Monitor::clearHostMonitorDetails()
{
    m_scanHashListRealtime.clear();
}

ProcessThread* Monitor::takeMonitorElem(const QString hostName)
{
    return m_scanThreadHashList.take(hostName);
}

void Monitor::stopSelectedScan()
{
    // Stop and wait thread from QHash table
    if (m_monitorWidget->scanMonitor->selectedItems().isEmpty()) {
        return;
    }

    const QString& hostname = m_monitorWidget->scanMonitor->selectedItems()[0]->text(0);

    ProcessThread *ptrTmp = takeMonitorElem(hostname);

    qDebug() << "Monitor:: pointer not in list:: " << ptrTmp;

    if (ptrTmp) {
        ptrTmp->quit();
        ptrTmp->wait();
        delete ptrTmp;

        // Remove Qhash entry for stopped scan
        m_scanHashListRealtime.take(hostname);
    } else {
        for (int i = 0; i < m_firstScanCacheList.size(); ++i) {
            if (m_firstScanCacheList[i].first == hostname) {
                // Remove stopped host from cache
                m_firstScanCacheList.removeAt(i);
                m_secondScanCacheList.removeAt(i);
                break;
            }
        }

        // delete QTreeWidgetItem for removed host
        delMonitorHost(hostname);
    }
}

void Monitor::stopAllScan()
{
    clearHostMonitor();
}

void Monitor::showSelectedScanDetails()
{
    if (m_monitorWidget->scanMonitor->selectedItems().isEmpty()) {
        return;
    }
    // start details UI
    MonitorDetails* details = new MonitorDetails(
        m_scanHashListRealtime.operator[](m_monitorWidget->scanMonitor->selectedItems()[0]->text(0)).second,
        m_monitorWidget->scanMonitor->selectedItems()[0]->text(0), m_ui);

    details->exec();

    if (details) {
        delete details;
    }
}

void Monitor::readFlowFromThread(const QString hostname, QByteArray lineData)
{
    /*
     * read data line form thread
     */
    QHash<QString, QPair<QByteArray,QStringList> >::const_iterator i = m_scanHashListRealtime.find(hostname);
    QTextStream stream(lineData);

    if (i == m_scanHashListRealtime.constEnd()) {
        QPair<QByteArray, QStringList> dataElement;
        dataElement.first.append(lineData);

        while (!stream.atEnd()) {
            QString currentLine(stream.readLine());
            findRemainingTime(currentLine,hostname);
            dataElement.second.append(currentLine);
        }

        m_scanHashListRealtime.insert(hostname, dataElement);
    } else {
        // append scan flow values
        while (i != m_scanHashListRealtime.constEnd() && i.key() == hostname) {

            QPair<QByteArray, QStringList> dataElement = i.value();
            dataElement.first.append(lineData);

            while (!stream.atEnd()) {
                QString currentLine(stream.readLine());
                findRemainingTime(currentLine,hostname);
                dataElement.second.append(currentLine);
            }

            m_scanHashListRealtime.insert(i.key(), dataElement);
            ++i;
        }
    }
}

void Monitor::findRemainingTime(const QString& textLine, const QString& hostName)
{
    if (textLine.contains("remaining") || textLine.contains("ETC")) {
        QString cleanLine = textLine.mid(textLine.indexOf("("), textLine.indexOf(")"));
        cleanLine = cleanLine.remove('(');
        cleanLine = cleanLine.remove(')');
        cleanLine.remove('\n');
        // insert new information into monitor
        updateMonitorHost(hostName, 2, cleanLine);
    }
}
