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

#ifndef DIGTHREAD_H
#define DIGTHREAD_H

#include <QtCore/QThread>
#include <QtCore/QByteArray>
#include <QtCore/QStringList>
#include <QtCore/QProcess>
#include <QtCore/QObject>
#include <QtCore/QMetaType>
#include <QtCore/QtDebug>
#include <QtCore/QPointer>

// local include
#include "nmapsi4Debug.h"

namespace digLookup {
    /*!
     * dig thread class, start dig with a QProcess and return
     * QByteArray result with a signal.
     */
    class digThread : public QThread
    {
    Q_OBJECT

    public:
	/*!
	 * Create a dig QThread and start QProcess for dig
	 * with hostname parameters.
	 */
	digThread(QByteArray& ProcB1, const QStringList hostname, QObject *parent = 0);

    signals:
	/*!
	 * Return dig QThread output with a Signal.
	 */
	void threadEnd(const QStringList, QByteArray);

    private:
	QByteArray m_pout;
	QStringList m_host;

    private slots:
	/*!
	 * Load QProcess buffer return on QByteArray
	 */
	void setValue();
	/*!
	 * Stop QProcess immediately.
	 */
	void stopProcess();

    protected:
	QPointer<QProcess> m_proc;
	void run();
	QObject* m_par;
    };
}

#endif
