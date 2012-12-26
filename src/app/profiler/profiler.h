/*
Copyright 2012  Francesco Cecconi <francesco.cecconi@gmail.com>

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

#ifndef PROFILER_H
#define PROFILER_H

#include <QtCore/QStringList>
#include <QtCore/QPair>
#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>
#include <QtGui/QComboBox>
#include <QtGui/QCheckBox>

class ProfilerManager;

class Profiler : public QObject
{
    Q_OBJECT

public:
    explicit Profiler(ProfilerManager *parent);
    ~Profiler() { };
    void restoreValuesFromProfile(const QStringList parameters);
    void loadDefaultHash();
    void resetOptions();
    QStringList buildExtensions();

private:
    void preLoadOptionsCombo();
    void preLoadOptionsCheckBox();

    ProfilerManager *m_ui;
    QHash<QString, QPair<QComboBox*, int> > preLoadComboList;
    QHash<QString, QPair<QCheckBox*, QString> > preLoadCheckBoxList;
    QHash<QString, QLineEdit*> lineEditList;
    QHash<QString, QSpinBox*> spinBoxList;

};

#endif
