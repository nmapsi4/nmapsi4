/*
Copyright 2013  Francesco Cecconi <francesco.cecconi@gmail.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef QMLWELCOME_H
#define QMLWELCOME_H

#include <QtCore/QObject>
#include <QtCore/QString>

class MainWindow;

class QmlWelcome : public QObject
{
    Q_OBJECT

public:
    QmlWelcome(MainWindow* parent);
    Q_INVOKABLE void scanSection();
    Q_INVOKABLE void vulnerabilitySection();
    Q_INVOKABLE void discoverSection();
    Q_INVOKABLE void callScan(const QString hostName);

private:
    MainWindow* m_ui;
};

#endif // QMLWELCOME_H
