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

#ifndef SCANTHREAD_H
#define SCANTHREAD_H

#include <QtCore/QThread>
#include <QtCore/QByteArray>
#include <QtCore/QStringList>
#include <QtCore/QProcess>
#include <QtCore/QObject>
#include <QtCore/QMetaType>
#include <QtCore/QtDebug>
#include "../nmapsi4Debug.h"

namespace scanning {
    class scanThread : public QThread
    {
	Q_OBJECT

    public:	
	scanThread(QByteArray& ProcB1, QByteArray& ProcB2, const QStringList parametri, QObject *parent = 0);

    signals:
	void threadEnd(const QStringList, QByteArray, QByteArray);
	void flowFromThread(const QString, const QString);
	void upgradePR();

    private:
	QByteArray pout;
	QByteArray perr;
	QStringList ParList;

    private slots:
	void setValue();
	void stopProcess();
	void realtimeData();

    protected:
	QProcess *proc;
	void run();
	QObject* par;
    };
}

#endif
