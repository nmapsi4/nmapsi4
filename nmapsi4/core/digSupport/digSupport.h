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

#ifndef DIGSUPPORT_H
#define DIGSUPPORT_H

#include <QtCore/QObject>
#include <QtCore/QProcess>
#include <QtCore/QDebug>
#include <QtCore/QTextStream>
#include <QtCore/QList>

// local include
#include "digThread.h"
#include "nmapsi4Debug.h"
#include "parserObjUtil.h"

using namespace digLookup;

namespace digInterface {
    /*!
     * dig interface, main method for dig lookup.
     */
    class digSupport : public QObject
    {
	Q_OBJECT
    public:
	/*!
	 * Create a object for dig lookup Class.
	 */
	digSupport();
	~digSupport();
	/*!
	 * Check for dig support (if installed) with QProcess.
	 */
	void checkDigSupport(bool& digState);
	/*!
	 * Start QThread dig for hostname.
	 */
	void digProcess(const QString hostname, parserObjUtil* objElem);

    private slots:
	void checkDig();
	/*!
	 * Set dig result on parser Object utils (objElem)
	 */
	void digReturn(const QStringList hostname, QByteArray buffer1);

    signals:
	/*!
	 * Stop QProcess immediately.
	 */
	void killScan();

    protected:
	QProcess* m_digProc;
	bool m_state;
	parserObjUtil* m_elemObjUtil;
	QString m_hostNameLocal;
	QList<digThread*> threadList;

    };
}
#endif
