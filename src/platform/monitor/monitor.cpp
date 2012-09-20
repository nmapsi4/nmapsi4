/***************************************************************************
 *   Copyright (C) 2011-2012 by Francesco Cecconi                          *
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

#include "monitor.h"
#include "mainwindow.h"

#ifdef Q_WS_X11
#include "nmapsi4adaptor.h"
#endif

MonitorWidget::MonitorWidget(QWidget* parent): QWidget(parent)
{
    setupUi(this);
}

Monitor::Monitor(MainWindow* parent)
    : QObject(parent), m_ui(parent), m_idCounter(0)
{
#ifdef Q_WS_X11
    new Nmapsi4Adaptor(this);
    // FIXME: with full mode It is registrered into root dbus session
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
    m_monitorWidget->scanMonitor->header()->setResizeMode(0, QHeaderView::Interactive);

    updateMaxParallelScan();

    m_isHostcached = false;
    m_timer = new QTimer(this);

    connect(m_monitorWidget->monitorStopCurrentScanButt, SIGNAL(clicked()),
            this, SLOT(stopSelectedScan()));
    connect(m_monitorWidget->monitorDetailsScanButt, SIGNAL(clicked()),
            this, SLOT(showSelectedScanDetails()));
    connect(m_monitorWidget->monitorStopAllScanButt, SIGNAL(clicked()),
            this, SLOT(stopAllScan()));
    connect(m_monitorWidget->scanMonitor, SIGNAL(itemSelectionChanged()),
            this, SLOT(monitorRuntimeEvent()));
}

Monitor::~Monitor()
{
    freemap<QString, ProcessThread*>::itemDeleteAllWithWait(m_scanHashList);
    freelist<LookupManager*>::itemDeleteAllWithWait(m_internealLookupList);
    freelist<DigManager*>::itemDeleteAll(m_digLookupList);

    if (m_timer->isActive()) {
        m_timer->stop();
    }

    delete m_timer;
}

bool Monitor::isHostOnMonitor(const QString hostname)
{
    QList<QTreeWidgetItem*>::const_iterator i;
    for (i = m_monitorElem.constBegin(); i != m_monitorElem.constEnd(); ++i) {
        if ((*i)->text(0) == hostname) {
            return true;
        }
    }

    return false;
}

int Monitor::monitorHostNumber()
{
    return m_monitorElem.size();
}

void Monitor::addMonitorHost(const QString hostName, const QStringList parameters, LookupType option)
{
    QTreeWidgetItem *hostThread = new QTreeWidgetItem(m_monitorWidget->scanMonitor);
    hostThread->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit.png")));
    hostThread->setText(0, hostName);
    hostThread->setText(1, parameters.join(" "));
    hostThread->setIcon(2, QIcon::fromTheme("media-playback-start", QIcon(":/images/images/media-playback-start.png")));
    hostThread->setText(2, "Scanning");
    m_monitorElem.push_front(hostThread);
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
    if (m_parallelThreadLimit) {
        m_parallelThreadLimit--;

        startScan(hostname, parameters);
        startLookup(hostname, option);
    } else {
        /**
         * Create a cache Item
         */
        QPair<QString, QStringList> scanPair(hostname, parameters);
        QPair<LookupType, QTreeWidgetItem*> lookupPair(option, item);

        m_hostScanCacheList.append(scanPair);
        m_lookupScanCacheList.append(lookupPair);

        item->setText(2, tr("Waiting"));
        item->setIcon(2, QIcon::fromTheme("media-playback-pause", QIcon(":/images/images/media-playback-pause.png")));
        qDebug() << "Monitor::cached:: " << hostname;
    }

    if (m_hostScanCacheList.size() && !m_isHostcached) {
        m_isHostcached = true;
        connect(m_timer, SIGNAL(timeout()), this, SLOT(cacheRepeat()));

        if (!m_timer->isActive()) {
            m_timer->start(5000);
        }
    }
}

void Monitor::cacheRepeat()
{
    if (!m_parallelThreadLimit) {
        return;
    }

    disconnect(this, SLOT(cacheRepeat()));

    m_isHostcached = false;

    m_timer->stop();
    int freeThreadSpace = 1;

    while (freeThreadSpace <= m_parallelThreadLimit && freeThreadSpace <= m_hostScanCacheList.size()) {
        QPair<QString, QStringList> scanPair = m_hostScanCacheList.takeFirst();
        QPair<LookupType, QTreeWidgetItem*> lookupPair = m_lookupScanCacheList.takeFirst();
        lookupPair.second->setText(2, tr("Scanning"));
        lookupPair.second->setIcon(2, QIcon::fromTheme("media-playback-start", QIcon(":/images/images/media-playback-start.png")));

        qDebug() << "Monitor::restored:: " << scanPair.first;

        cacheScan(scanPair.first, scanPair.second, lookupPair.first, lookupPair.second);
        freeThreadSpace++;
    }

}

void Monitor::startScan(const QString hostname, QStringList parameters)
{
    parameters.append(hostname); // add hostname

    // start scan Thread
    QWeakPointer<ProcessThread> thread = new ProcessThread("nmap", parameters);
    m_scanHashList.insert(hostname, thread.data());
    // read current data scan from the thread
    connect(thread.data(), SIGNAL(flowFromThread(QString,QString)),
            this, SLOT(readFlowFromThread(QString,QString)));
    // read scan data return
    connect(thread.data(), SIGNAL(threadEnd(QStringList,QByteArray,QByteArray)),
            this, SLOT(scanFinisced(QStringList,QByteArray,QByteArray)));
    // start scan
    thread.data()->start();
}

void Monitor::startLookup(const QString hostname, LookupType option)
{
    if (option == DisabledLookup || !HostTools::isDns(hostname)) {
        return;
    }

    if (option == InternalLookup) {
        LookupManager *internalLookupPtr = new LookupManager(hostname);
        m_internealLookupList.push_back(internalLookupPtr);

        connect(internalLookupPtr, SIGNAL(threadEnd(QHostInfo,int,QString)),
                this, SLOT(lookupFinisced(QHostInfo,int,QString)));

        internalLookupPtr->start();
    } else {
        PObjectLookup* tmpParserObj_ = new PObjectLookup();

        DigManager *digManager = new DigManager();
        m_digLookupList.push_back(digManager);

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

void Monitor::scanFinisced(const QStringList parametersList, QByteArray dataBuffer, QByteArray errorBuffer)
{
    /*
     * Remove host scan finisced from the monitor list.
     */
    delMonitorHost(parametersList[parametersList.size() - 1]);

    m_parallelThreadLimit++;
    /*
     * Return scan result with a signal.
     */
    emit hostFinisced(parametersList, dataBuffer, errorBuffer, m_hostIdList.value(parametersList[parametersList.size() - 1]));
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
    for (int i = 0; i < m_monitorElem.size(); i++) {
        if (m_monitorElem[i]->text(0) == hostName) {
            // remove host from monitor and list.
            delete m_monitorElem.takeAt(i);
            break;
        }
    }

    emit monitorUpdated(monitorHostNumber());
}

void Monitor::updateMonitorHost(const QString hostName, int valueIndex, const QString newData)
{
    Q_ASSERT(valueIndex < m_monitor->columnCount());

    QList<QTreeWidgetItem*>::const_iterator i;
    for (i = m_monitorElem.constBegin(); i != m_monitorElem.constEnd(); ++i) {
        if ((*i)->text(0) == hostName) {
            (*i)->setText(valueIndex, newData);
        }

        break;
    }
}

void Monitor::clearHostMonitor()
{
    freemap<QString, ProcessThread*>::itemDeleteAllWithWait(m_scanHashList);
    freelist<LookupManager*>::itemDeleteAllWithWait(m_internealLookupList);
    freelist<DigManager*>::itemDeleteAll(m_digLookupList);

    if (m_timer->isActive()) {
        m_timer->stop();
    }

    m_hostScanCacheList.clear();
    m_lookupScanCacheList.clear();

    m_isHostcached = false;
    updateMaxParallelScan();

    freelist<QTreeWidgetItem*>::itemDeleteAll(m_monitorElem);
    m_hostIdList.clear();
}

void Monitor::updateMaxParallelScan()
{
    QSettings settings("nmapsi4", "nmapsi4");
    m_parallelThreadLimit = settings.value("maxParallelScan", 5).toInt();
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
    m_scanHashListFlow.clear();
}

ProcessThread* Monitor::takeMonitorElem(const QString hostName)
{
    return m_scanHashList.take(hostName);
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
        m_scanHashListFlow.take(hostname);
    } else {
        for (int i = 0; i < m_hostScanCacheList.size(); ++i) {
            if (m_hostScanCacheList[i].first == hostname) {
                // Remove stopped host from cache
                m_hostScanCacheList.removeAt(i);
                m_lookupScanCacheList.removeAt(i);
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
    QWeakPointer<MonitorDetails> details = new MonitorDetails(m_scanHashListFlow.operator[](m_monitorWidget->scanMonitor->selectedItems()[0]->text(0)),
            m_monitorWidget->scanMonitor->selectedItems()[0]->text(0), m_ui);
    details.data()->exec();

    if (!details.isNull()) {
        delete details.data();
    }
}

void Monitor::readFlowFromThread(const QString hostname, QString lineData)
{
    /*
     * read data line form thread
     */
    QHash<QString, QStringList>::const_iterator i = m_scanHashListFlow.find(hostname);
    QTextStream stream(&lineData);

    if (i == m_scanHashListFlow.constEnd()) {
        QStringList flowHistory;

        while (!stream.atEnd()) {
            flowHistory.append(stream.readLine());
        }

        m_scanHashListFlow.insert(hostname, flowHistory);
    } else {
        // append scan flow values
        while (i != m_scanHashListFlow.constEnd() && i.key() == hostname) {
            QStringList flowHistory = i.value();

            while (!stream.atEnd()) {
                flowHistory.append(stream.readLine());
            }

            m_scanHashListFlow.insert(i.key(), flowHistory);
            ++i;
        }
    }

    // search hostname on treeWidget and update data rows (index = 2)
    // take only remaining time and remove character unused, only [remaining || ETA]
    if (lineData.contains("remaining") || lineData.contains("ETC")) {
        QString infoTmp_ = lineData.mid(lineData.indexOf("("), lineData.indexOf(")"));
        infoTmp_ = infoTmp_.remove('(');
        infoTmp_ = infoTmp_.remove(')');
        infoTmp_.remove('\n');
        // insert new information into monitor
        updateMonitorHost(hostname, 2, infoTmp_);
    }
}
