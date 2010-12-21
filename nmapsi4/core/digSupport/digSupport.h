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

#include <QObject>
#include <QProcess>
#include <QTextStream>
#include <QDebug>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTextStream>
#include <QList>
#include "digThread.h"
#include "../nmapsi4Debug.h"


class digSupport : public QObject
{
    Q_OBJECT
    friend class nmapClass;

    public:
        digSupport();
        ~digSupport();
        void checkDigSupport();
        bool getDigSupport();
        void digProcess(const QString hostname, QTreeWidget* view);

    private slots:
        void checkDig();
        void digReturn(const QStringList hostname, QByteArray buffer1);

    signals:
        void killScan();

    protected:
        QProcess *digProc;
        bool state;
        QTreeWidget *Wview;
        digThread *th;
        QList<QTreeWidgetItem*> digList;

};

#endif
