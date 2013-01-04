/*
Copyright 2011-2012  Francesco Cecconi <francesco.cecconi@gmail.com>

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

#ifndef PROFILEHANDLER_H
#define PROFILEHANDLER_H

#include <QtCore/QObject>
#include <QtCore/QPair>
#include <QtCore/QStringList>

class MainWindow;

class ProfileHandler : public QObject
{
    Q_OBJECT
public:
    ProfileHandler(MainWindow* ui, int userId, int defaultProfile);
    ~ProfileHandler() {};
    QList< QPair<QString, QString> > defaultScanProfile() const;
    QStringList getParameters() const;
    bool containsParameter(const QString& parameter) const;

private:
    int m_userId;
    int m_savedProfileIndex;
    MainWindow* m_ui;

public slots:
    void loadDefaultProfile();
    void clearParametersCombo();
    void updateComboParametersFromList(const QStringList& parameters);
};

#endif
