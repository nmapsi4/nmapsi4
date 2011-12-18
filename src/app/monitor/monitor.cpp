/***************************************************************************
 *   Copyright (C) 2008-2011 by Francesco Cecconi                          *
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
#include "mainwin.h"

#ifdef Q_WS_X11
#include "nmapsi4adaptor.h"
#endif

monitor::monitor(QTreeWidget* monitor, nmapClass* parent) : _monitor(monitor), _ui(parent)
{
#ifdef Q_WS_X11
    new Nmapsi4Adaptor(this);
    // FIXME: with full mode It is registrered into root dbus session
    QDBusConnection dbus = QDBusConnection::sessionBus();
//     if (!dbus.isConnected())
//     {
//         dbus = QDBusConnection::systemBus();
//     }
    dbus.registerObject("/Nmapsi4",this);
    bool value = dbus.registerService("org.nmapsi4.Nmapsi4");

    if (!value)
    {
        qDebug() << "DBUS:: error with dbus connection";
    }
#endif
}

monitor::~monitor()
{
    freelist<QTreeWidgetItem*>::itemDeleteAll(monitorElem);
    freelist<lookupManager*>::itemDeleteAllWithWait(internealLookupList);
    freelist<digManager*>::itemDeleteAll(digLookupList);
    _scanHashListFlow.clear();
}

bool monitor::isHostOnMonitor(const QString hostname)
{
    QList<QTreeWidgetItem*>::const_iterator i;
    for (i = monitorElem.constBegin(); i != monitorElem.constEnd(); ++i)
    {
        if ((*i)->text(0) == hostname)
        {
            return true;
        }
    }

    return false;
}

int monitor::monitorHostNumber()
{
    return monitorElem.size();
}

void monitor::addMonitorHost(const QString hostName, const QStringList parameters, LookupType option)
{
    QTreeWidgetItem *hostThread = new QTreeWidgetItem(_monitor);
    hostThread->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit.png")));
    hostThread->setText(0, hostName);
    hostThread->setText(1, parameters.join(" "));
    hostThread->setIcon(2, QIcon(QString::fromUtf8(":/images/images/reload.png")));
    hostThread->setText(2, "Scanning");
    monitorElem.push_front(hostThread);

    emit monitorUpdated(monitorHostNumber());

    // Start Scan for host
    startScan(hostName,parameters);
    startLookup(hostName,option);
}

void monitor::updateMonitorHost(const QString hostName, int valueIndex, const QString newData)
{
    Q_ASSERT(valueIndex < _monitor->columnCount());

    QList<QTreeWidgetItem*>::const_iterator i;
    for (i = monitorElem.constBegin(); i != monitorElem.constEnd(); ++i)
    {
        if ((*i)->text(0) == hostName)
        {
            (*i)->setText(valueIndex,newData);
        }

        break;
    }
}

void monitor::startScan(const QString hostname, QStringList parameters)
{
    parameters.append(hostname); // add hostname

    // start scan Thread
    QPointer<QProcessThread> thread = new QProcessThread("nmap",parameters);
    _scanHashList.insert(hostname,thread);
    // read current data scan from the thread
    connect(thread, SIGNAL(flowFromThread(QString,QString)),
            this, SLOT(readFlowFromThread(QString,QString)));
    // read scan data return
    connect(thread, SIGNAL(threadEnd(QStringList,QByteArray,QByteArray)),
            this, SLOT(scanFinisced(QStringList,QByteArray,QByteArray)));
    // start scan
    thread->start();
}

void monitor::startLookup(const QString hostname, LookupType option)
{
    if (option == DisabledLookup)
    {
        return;
    }

    if (option == InternalLookup)
    {
        lookupManager *internalLookupPtr = new lookupManager(hostname,this);
        internealLookupList.push_back(internalLookupPtr);

        connect(internalLookupPtr, SIGNAL(threadEnd(QHostInfo,int,QString)),
                this, SLOT(lookupFinisced(QHostInfo,int,QString)));

        internalLookupPtr->start();
    }
    else
    {
        parserObjUtil* tmpParserObj_ = new parserObjUtil();

        digManager *digC = new digManager();
        digLookupList.push_back(digC);

        digC->digProcess(hostname,tmpParserObj_);

        _ui->_parser->addUtilObject(tmpParserObj_);
    }
}

void monitor::scanFinisced(const QStringList parametersList, QByteArray dataBuffer, QByteArray errorBuffer)
{
    /*
     * Remove host scan finisced from the monitor list.
     */
    delMonitorHost(parametersList[parametersList.size()-1]);
    /*
     * Return scan result with a signal.
     */
    emit hostFinisced(parametersList,dataBuffer,errorBuffer);
}

void monitor::lookupFinisced(QHostInfo info, int state, const QString hostname)
{
    if(state == -1)
    {
        //QMessageBox::warning(this, "NmapSI4", tr("Wrong Address\n"), tr("Close"));
        return;
    }

    parserObjUtil* elemObjUtil = new parserObjUtil();

    elemObjUtil->setHostName(hostname);
    const int infoSize_ = info.addresses().size();
    for(int index=0; index < infoSize_; index++)
    {
        elemObjUtil->setInfoLookup(info.addresses()[index].toString());
    }

    _ui->_parser->addUtilObject(elemObjUtil);
}

void monitor::delMonitorHost(const QString hostName)
{
     for(int i=0; i < monitorElem.size(); i++)
     {
          if(monitorElem[i]->text(0) == hostName)
          {
              // remove host from monitor and list.
              delete monitorElem.takeAt(i);
              break;
           }
     }

     emit monitorUpdated(monitorHostNumber());
}

void monitor::clearHostMonitor()
{
    freemap<QString,QProcessThread*>::itemDeleteAllWithWait(_scanHashList);
    freelist<lookupManager*>::itemDeleteAllWithWait(internealLookupList);
    freelist<digManager*>::itemDeleteAll(digLookupList);
}

void monitor::clearHostMonitorDetails()
{
    _scanHashListFlow.clear();
}

QProcessThread* monitor::takeMonitorElem(const QString hostName)
{
    return _scanHashList.take(hostName);
}

void monitor::stopSelectedScan()
{
        // Stop and wait thread from QHash table
    if (_monitor->selectedItems().isEmpty())
    {
        return;
    }

    QProcessThread *ptrTmp = takeMonitorElem(_monitor->selectedItems()[0]->text(0));

    if (ptrTmp)
    {
        ptrTmp->quit();
        ptrTmp->wait();
        delete ptrTmp;
    }

    // Remove Qhash entry for stopped scan
    _scanHashListFlow.take(_monitor->selectedItems()[0]->text(0));
}

void monitor::showSelectedScanDetails()
{
    if (_monitor->selectedItems().isEmpty())
    {
        return;
    }
    // start details UI
    classDetails details(_scanHashListFlow.operator[](_monitor->selectedItems()[0]->text(0)),
                         _monitor->selectedItems()[0]->text(0));
    details.exec();
}

void monitor::readFlowFromThread(const QString hostname, QString lineData)
{
    /*
     * read data line form thread
     */
    QHash<QString, QStringList>::const_iterator i = _scanHashListFlow.find(hostname);
    QTextStream stream(&lineData);

    if (i == _scanHashListFlow.constEnd())
    {
        QStringList flowHistory;

        while (!stream.atEnd())
        {
            flowHistory.append(stream.readLine());
        }

        _scanHashListFlow.insert(hostname,flowHistory);
    }
    else
    {
        // append scan flow values
        while (i != _scanHashListFlow.constEnd() && i.key() == hostname)
        {
            QStringList flowHistory = i.value();

            while (!stream.atEnd())
            {
                flowHistory.append(stream.readLine());
            }

            _scanHashListFlow.insert(i.key(),flowHistory);
            ++i;
        }
    }

    // search hostname on treeWidget and update data rows (index = 2)
    // take only remaining time and remove character unused, only [remaining || ETA]
    if (lineData.contains("remaining") || lineData.contains("ETC"))
    {
        QString infoTmp_ = lineData.mid(lineData.indexOf("("),lineData.indexOf(")"));
        infoTmp_ = infoTmp_.remove('(');
        infoTmp_ = infoTmp_.remove(')');
        infoTmp_.remove('\n');
        // insert new information into monitor
        updateMonitorHost(hostname,2,infoTmp_);
    }
}
