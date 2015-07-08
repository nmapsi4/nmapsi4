/*
Copyright 2009-2012  Francesco Cecconi <francesco.cecconi@gmail.com>

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

#include "digmanager.h"
#include "memorytools.h"

DigManager::DigManager(QObject* parent)
    : QObject(parent), m_elemObjUtil(0)
{
}

DigManager::~DigManager()
{
    memory::freelist<ProcessThread*>::itemDeleteAllWithWait(m_threadList);
}

void DigManager::digRequest(const QString hostname, PObjectLookup* objElem, DigRequestType type)
{
    QStringList command;

    if (type == Verbose) {
        command << hostname;
    } else {
        command << "+short";
        command << hostname;
    }

    m_hostNameLocal = hostname;
    m_elemObjUtil = objElem;

    QPointer<ProcessThread> m_th = new ProcessThread("dig", command);
    m_threadList.push_back(m_th);

    if (type == Verbose) {
        connect(m_th, &ProcessThread::threadEnd,
                this, &DigManager::longDigAnswer);
    } else {
        connect(m_th, &ProcessThread::threadEnd,
                this, &DigManager::shortDigAnswer);
    }

    m_th->start();
}

void DigManager::longDigAnswer(const QStringList hostname, QByteArray bufferData, QByteArray bufferError)
{
    Q_UNUSED(hostname);

    /*
     * TODO: remove this check with QT5 QStandardPaths::findExecutable.
     *
     */
    if (!bufferData.size() && bufferError.size()) {
        qWarning() << "Error: Dig is not installed.";
        return;
    }

    QString buff1(bufferData);
    QTextStream stream1(&buff1);
    QString line;

    m_elemObjUtil->setHostName(m_hostNameLocal);

    while (!stream1.atEnd()) {
        line = stream1.readLine();
        if (!line.startsWith(QLatin1String(";;"))
                && !line.startsWith(QLatin1String(";"))
                && !line.isEmpty()) {
            m_elemObjUtil->setInfoLookup(line);
        }
    }

    // clear thread
    bufferData.clear();
    bufferError.clear();
}

void DigManager::shortDigAnswer(const QStringList hostname, QByteArray bufferData, QByteArray bufferError)
{
    Q_UNUSED(hostname);

    /*
     * TODO: remove this check with QT5 QStandardPaths::findExecutable.
     *
     */
    if (!bufferData.size() && bufferError.size()) {
        qWarning() << "Error: Dig is not installed.";
        return;
    }

    QString buff1(bufferData);
    QTextStream stream1(&buff1);
    QString line;

    m_elemObjUtil->setHostName(m_hostNameLocal);

    while (!stream1.atEnd()) {
        line = stream1.readLine();
        m_elemObjUtil->setInfoLookup(line);
    }

    // clear thread
    bufferData.clear();
    bufferError.clear();
}
