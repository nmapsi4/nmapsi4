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

#include "../mainwin.h"

void nmapClass::scanLookup(QHostInfo info, int state, QString hostname) {
    // TODO
    // create a user box for scan (lookup or not) type
    // create a function for check user preference

    qDebug() << "scanLookup::flag:: " << state;

    this->isDns(hostname);

    if(state == -1) {
        QMessageBox::warning(this, "NmapSI4", tr("Wrong Address\n"), tr("Close"));
        this->delMonitorHost(scanMonitor,hostname);
        return;
    }
    treeLookup->setIconSize(QSize::QSize(32, 32));
    treeLookup->header()->setResizeMode(0, QHeaderView::Interactive);

    rootLook = new QTreeWidgetItem(treeLookup);
    itemListLook.push_front(rootLook);
    rootLook->setBackground(0, QColor::fromRgb(164, 164, 164));
    rootLook->setForeground(0, Qt::white);
    rootLook->setIcon(0, QIcon(QString::fromUtf8(":/images/images/viewmagfit.png")));

    qDebug() << "scanLookup::Found address DNS:: " << info.hostName();

    if(isDns(hostname)) {
        rootLook->setText(0, hostname);
    } else {
        rootLook->setText(0, info.hostName());
    }

    foreach (QHostAddress address, info.addresses()) {
        qDebug() << "scanLookup::Found address:: " << address.toString();
        itemLook = new QTreeWidgetItem(rootLook);
        itemListLook.push_front(itemLook);
        itemLook->setText(0,address.toString());
    }

    this->scan(hostname);
}

bool nmapClass::isDns(QString hostname) {
    bool state_;
    QRegExp rx_("*.*.*.*");
    rx_.setPatternSyntax(QRegExp::Wildcard);
    (rx_.exactMatch(hostname) == true) ? state_=false : state_=true;
    qDebug() << "nmapsi4/isDns():: " << hostname << state_;
    return state_;
}
