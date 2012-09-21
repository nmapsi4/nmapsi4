/***************************************************************************
 *   Copyright (C) 2011-2012 by Francesco Cecconi                          *
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

#ifndef DISCOVERMANAGER_H
#define DISCOVERMANAGER_H

#include "ui_discoverwidget.h"

#include <QtCore/QObject>
#include <QtCore/QDateTime>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QSplitter>

#include "discover.h"
#include "regularexpression.h"
#include "notify.h"
#include "selectprofiledialog.h"
#include "logwriterxml.h"
#include "style.h"

// system
#ifndef Q_WS_WIN
#include <unistd.h>
#endif

class MainWindow;

class DiscoverWidget : public QWidget, public Ui::DiscoverWidgetForm
{
    Q_OBJECT
public:
    explicit DiscoverWidget(QWidget* parent = 0);
};

class DiscoverManager : public QObject
{
    Q_OBJECT

public:
    DiscoverManager(MainWindow* parent);
    ~DiscoverManager();
    void defaultDiscoverProbes();
    void syncSettings();

    DiscoverWidget* m_discoverWidget;

private:
    void clearDiscover();
    void startSelectProfilesDialog();
    void resetDiscoverfromRangeValues();
    bool activeIpContains(const QString ipAddress);

    MainWindow* m_ui;
    QList<Discover*> m_listDiscover;
    QList<QTreeWidgetItem*> m_listTreeItemDiscover;
    QList<QTreeWidgetItem*> m_listTreePackets;
    int m_ipCounter;
    int m_userid;
    bool m_discoverIsActive;
    QSplitter *m_discoverHorizontalSplitter;
    QSplitter *m_discoverVerticalSplitter;

signals:
    void killDiscoverFromIpsRange();
    void killDiscoverFromCIDR();

public slots:
    void loadFoundInterfaces();
    void scanSingleDiscoveredIp();
    void scanAllDiscoveredIps();
    void saveXmlIpsList();
    void loadXmlIpsList();

private slots:
    void endDiscoverIpsFromRange(const QStringList hostname, bool state, const QByteArray callBuff);
    void endDiscoverIpsFromCIDR();
    void currentDiscoverIpsFromCIDR(const QString parameters, const QString data);
    void discoverIp(const QString& interface);
    void runtimeScanDiscover();
    void stopDiscoverFromIpsRange();
    void stopDiscoverFromCIDR();
    void startDiscoverIpsFromCIDR();
    void startDiscoverIpsFromRange();
    void calculateAddressFromCIDR();
    void splitCIDRAddressPasted();
};

#endif // DISCOVERMANAGER_H
