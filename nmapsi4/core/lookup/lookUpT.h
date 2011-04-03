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

#ifndef LOOKUPT_H
#define LOOKUPT_H

#include <QtCore/QThread>
#include <QtCore/QStringList>
#include <QtCore/QString>
#include <QtCore/QMetaType>
#include <QtNetwork/QHostInfo>
#include "../nmapsi4Debug.h"

namespace internalLookup {
    /*!
     * internal lookup thread class, return QHostInfo with 
     * address resolv.
     */
    class lookUpT : public QThread
    {
	Q_OBJECT

	public:
	    /*!
	     * Create a QThread and start static QHostInfo resolv.
	     */
	    lookUpT(const QString hostname, QObject *parent);

	signals:
	    /*!
	     * Return QHostInfo with a signal with address resolv.
	     */
	    void threadEnd(QHostInfo, int, const QString);

	private:
	    QString m_host;
	    QHostInfo m_info;

	private slots:
	    /*!
	     * Stop static QHostInfo resolv immediately.
	     */
	    void killLookup();

	protected:
	    void run();
	    QObject* m_par;
    };
}
#endif
