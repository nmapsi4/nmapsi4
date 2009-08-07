/***************************************************************************
 *   Copyright (C) 2009 by Francesco Cecconi                               *
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

#ifndef DIGTHREAD_H
#define DIGTHREAD_H

#include <QThread>
#include <QByteArray>
#include <QStringList>
#include <QProcess>
#include <QObject>
#include <QMetaType>
#include <QtDebug>


class digThread : public QThread
{
 Q_OBJECT

 public:
     digThread(QByteArray& ProcB1, const QStringList hostname, QObject *parent = 0);

signals:
     void threadEnd(const QStringList, QByteArray);
     // TODO create a start scan signal

private:
     QByteArray pout;
     QStringList host;

private slots:
     void setValue();
     void stopProcess();

 protected:
     QProcess *proc;
     void run();
     QObject* par;
};


#endif
