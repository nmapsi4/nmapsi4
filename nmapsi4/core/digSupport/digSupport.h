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
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>
#include <QtCore/QTextStream>
#include <QtCore/QList>
#include "digThread.h"
#include "../nmapsi4Debug.h"
#include "../parserObj/parserObjUtil.h"


class digSupport : public QObject
{
    Q_OBJECT
    public:
        digSupport();
        ~digSupport();
        void checkDigSupport(bool& digState);
        void digProcess(const QString hostname, QTreeWidget* view, parserObjUtil* objElem);

    private slots:
        void checkDig();
        void digReturn(const QStringList hostname, QByteArray buffer1);

    signals:
        void killScan();

    protected:
        QProcess* m_digProc;
        bool m_state;
        QTreeWidget* m_Wview;
        digThread* m_th;
	parserObjUtil* m_elemObjUtil;
	QString m_hostNameLocal;

};

#endif
