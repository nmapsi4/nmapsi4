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

digSupport::digSupport() 
    : m_digProc(NULL),
      m_state(false),
      m_Wview(NULL),
      m_th(NULL),
      m_elemObjUtil(NULL) 
{
    
}


void digSupport::checkDigSupport(bool& digState) 
{
    m_digProc = new QProcess();
    m_state = digState;
    QStringList parametri;
    parametri << "-v";
    m_digProc->start("dig", parametri);

    connect(m_digProc, SIGNAL(finished(int, QProcess::ExitStatus)),
            this, SLOT(checkDig()));
}

void digSupport::checkDig() 
{
    QString *output, *error;
    output = new QString(m_digProc->readAllStandardOutput());
    error = new QString(m_digProc->readAllStandardError());

    QTextStream stream(output);
    QTextStream stream2(error);
    QString line, line2;

    line = stream.readLine();
    line2 = stream2.readLine();
#ifndef DIG_NO_DEBUG
    qDebug() << "Dig::line:: " << line;
    qDebug() << "Dig::error:: " << line2;
#endif
    if (line2.startsWith(QLatin1String("DiG")) || line.startsWith(QLatin1String("DiG"))) {
        m_state = true;
#ifndef DIG_NO_DEBUG
        qDebug() << "Dig support enable";
#endif
    } else {
	m_state = false;
    }

    delete output;
    delete error;
    delete m_digProc;
}

void digSupport::digProcess(const QString hostname, QTreeWidget* view, parserObjUtil* objElem) 
{
    m_Wview = view;
    QByteArray buff1;
    QStringList command;
    m_hostNameLocal = hostname;
    command << hostname;
    m_elemObjUtil = objElem;
    m_th = new digThread(buff1, command, this);
    m_th->start();
    connect(m_th, SIGNAL(threadEnd(const QStringList, QByteArray)),
      this, SLOT(digReturn(const QStringList, QByteArray)));
}

void digSupport::digReturn(const QStringList hostname, QByteArray buffer1) 
{
#ifndef DIG_NO_DEBUG
    qDebug() << "############## digSupport():: start pars ######################";
#endif
    Q_UNUSED(hostname);
    QString buff1(buffer1);
    QTextStream stream1(&buff1);
    QString line;
    
    m_elemObjUtil->setHostName(m_hostNameLocal);

    while(!stream1.atEnd()) {
        line = stream1.readLine();
        if(!line.startsWith(QLatin1String(";;")) && !line.startsWith(QLatin1String(";")) && !line.isEmpty()) {
#ifndef DIG_NO_DEBUG
            qDebug() << "digSupport():: " << line;
#endif
	    m_elemObjUtil->setInfoLookup(line);
        }
    }

    delete m_th;
    buffer1.clear();
}

digSupport::~digSupport() {
}
