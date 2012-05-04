/***************************************************************************
 *   Copyright (C) 2012 by Francesco Cecconi                               *
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


#ifndef PROFILER_H
#define PROFILER_H

#include <QtCore/QStringList>
#include <QtCore/QPair>
#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>
#include <QtGui/QComboBox>
#include <QtGui/QCheckBox>

class ProfilerManager;

class Profiler
{

public:
    Profiler(ProfilerManager *parent);
    ~Profiler() { };
    void restoreValuesFromProfile(const QStringList parameters);
    void loadDefaultHash();
    QStringList buildExtensions();

private:
    ProfilerManager *m_ui;
    QHash<QString,QPair<QComboBox*,int> > preLoadComboList;
    QHash<QString,QPair<QCheckBox*,QString> > preLoadCheckBoxList;
    QHash<QString,QLineEdit*> lineEditList;
    QHash<QString,QSpinBox*> spinBoxList;

    void preLoadOptionsCombo();
    void preLoadOptionsCheckBox();
};

#endif