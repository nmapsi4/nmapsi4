/**************************************************************************
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

#ifndef DIGMANAGER_H
#define DIGMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QProcess>
#include <QtCore/QDebug>
#include <QtCore/QTextStream>
#include <QtCore/QList>
#include <QtCore/QPointer>

// local include
#include "qprocessthread.h"
#include "nmapsi4Debug.h"
#include "parserObjects.h"

class digManager : public QObject
{
    /*!
    * dig interface, main method for dig lookup.
    */
    Q_OBJECT
public:
    /*!
     * Create a object for dig lookup Class.
     */
    digManager();
    ~digManager();
    /*!
     * Start QThread dig for hostname.
     */
    void digProcess(const QString hostname, parserObjUtil* objElem);

private:
    parserObjUtil* m_elemObjUtil;
    QString m_hostNameLocal;
    QList<QProcessThread*> m_threadList;

private slots:
    /*!
     * Set dig result on parser Object utils (objElem)
     */
    void digReturn(const QStringList hostname, QByteArray bufferData, QByteArray bufferError);

signals:
    /*!
     * Stop QProcess immediately.
     */
    void killScan();
};
#endif
