/***************************************************************************
 *   Copyright (C) 2011 by Francesco Cecconi                               *
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

#ifndef MONITORHOSTSCANDETAILS_H
#define MONITORHOSTSCANDETAILS_H

#include <QtGui/QDialog>
#include <QtCore/QList>
#include <QtCore/QStringList>
#include <QtCore/QObject>
#include <QtCore/QTimer>

// local include
#include "ui_monitorhostscandetails.h"
#include "memorytools.h"

using namespace memory;

class MonitorDetails : public QDialog, private Ui::monitorDetails
{
    Q_OBJECT

public:
    MonitorDetails(QStringList& processFlow, const QString hostname);
    ~MonitorDetails();

private:
    /*
     * Load scan realtime line to QListWidget
     */
    void loadFlow();

    QStringList& m_scanLines;
    QList<QListWidgetItem*> m_itemsList;
    int m_itemsSize;
    QTimer* m_timer;

private slots:
    /*
     * Reload scan realtime line, append to QListWidget
     * only the new line.
     */
    void reloadFlow();
};
#endif
