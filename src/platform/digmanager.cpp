/***************************************************************************
 *   Copyright (C) 2009-2011 by Francesco Cecconi                          *
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

#include "digmanager.h"
#include "memorytools.h"

DigManager::DigManager() //: m_state(false)
{
}

DigManager::~DigManager()
{
    qDebug() << "DEBUG:: ~dig()";
    memory::freelist<ProcessThread*>::itemDeleteAllWithWait(m_threadList);
}

void DigManager::digProcess(const QString hostname, parserObjUtil* objElem)
{
    QStringList command;
    m_hostNameLocal = hostname;
    command << hostname;
    m_elemObjUtil = objElem;

    QWeakPointer<ProcessThread> m_th = new ProcessThread("dig",command);
    m_threadList.push_back(m_th.data());

    connect(m_th.data(), SIGNAL(threadEnd(QStringList,QByteArray,QByteArray)),
      this, SLOT(digReturn(QStringList,QByteArray,QByteArray)));

    m_th.data()->start();
}

void DigManager::digReturn(const QStringList hostname, QByteArray bufferData, QByteArray bufferError)
{
    Q_UNUSED(hostname);

    QString buff1(bufferData);
    QTextStream stream1(&buff1);
    QString line;

    m_elemObjUtil->setHostName(m_hostNameLocal);

    while(!stream1.atEnd())
    {
        line = stream1.readLine();
        if(!line.startsWith(QLatin1String(";;"))
            && !line.startsWith(QLatin1String(";"))
            && !line.isEmpty())
        {
            m_elemObjUtil->setInfoLookup(line);
        }
    }

    // clear thread
    bufferData.clear();
    bufferError.clear();
}
