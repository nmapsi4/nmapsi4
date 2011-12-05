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

#include "digSupport.h"
#include "memorytools.h"

digInterface::digSupport::digSupport() : m_state(false)
{  
}

digInterface::digSupport::~digSupport()
{
    qDebug() << "DEBUG:: ~digSupport()";
    memory::freelist<QProcessThread*>::itemDeleteAllWithWait(threadList);
}

void digInterface::digSupport::checkDigSupport(bool& digState) 
{
    m_digProc = new QProcess();
    m_state = digState;
    QStringList parametri;
    parametri << "-v";
    m_digProc->start("dig", parametri);

    connect(m_digProc, SIGNAL(finished(int,QProcess::ExitStatus)),
            this, SLOT(checkDig()));
}

void digInterface::digSupport::checkDig() 
{
    QString output(m_digProc->readAllStandardOutput());
    QString error(m_digProc->readAllStandardError());

    QTextStream stream(&output);
    QTextStream stream2(&error);

    QString line(stream.readLine());
    QString line2(stream2.readLine());

    if (line2.startsWith(QLatin1String("DiG")) || line.startsWith(QLatin1String("DiG"))) 
    {
        m_state = true;
    } 
    else 
    {
        m_state = false;
    }

    delete m_digProc;
}

void digInterface::digSupport::digProcess(const QString hostname, parserObjUtil* objElem) 
{
    //QByteArray buffData;
    //QByteArray buffError;
    QStringList command;
    m_hostNameLocal = hostname;
    command << hostname;
    m_elemObjUtil = objElem;
    QPointer<QProcessThread> m_th = new QProcessThread("dig",command);
    threadList.push_back(m_th);
    m_th->start();
    connect(m_th, SIGNAL(threadEnd(QStringList,QByteArray,QByteArray)),
      this, SLOT(digReturn(QStringList,QByteArray,QByteArray)));
}

void digInterface::digSupport::digReturn(const QStringList hostname, QByteArray bufferData, QByteArray bufferError) 
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
