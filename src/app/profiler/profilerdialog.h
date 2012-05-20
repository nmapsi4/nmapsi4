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


#ifndef PROFILERMANAGER_H
#define PROFILERMANAGER_H

#include <QtGui/QDialog>
#include <QtGui/QListWidgetItem>
#include <QtGui/QMessageBox>
#include <QtCore/QDebug>

#include "ui_profilerdialog.h"
#include "nsemanager.h"
#include "profiler.h"

// system
#ifndef Q_WS_WIN
#include <unistd.h>
#endif

namespace Ui
{
    class profilerDialog;
}

class ProfilerManager : public QDialog, public Ui::profilerDialog
{
    Q_OBJECT

public:
    NseManager* m_nseManager;

    ProfilerManager(QWidget* parent = 0);
    ProfilerManager(const QString profileName, const QString parameters, QWidget* parent = 0);
    ~ProfilerManager();

private:
    QListWidgetItem *m_profileW;
    QListWidgetItem *m_scanW;
    QListWidgetItem *m_toolW;
    QListWidgetItem *m_discoverW;
    QListWidgetItem *m_timingW;
    QListWidgetItem *m_nseW;
    int m_uid;
    Profiler *m_profiler;

    void initObject();
    void loadDefaultComboValues();
    void setNormalProfile();
    void setFullVersionProfile();
    void setDefaultNseScripts();
    void createQList();
    void reloadScanParameters();

signals:
    void doneParBook(const QString profileName, const QString profilePar);

private slots:
    void optionListUpdate();
    void update_portCombo();
    void update_options();
    void update_comboVerbosity();
    void exit();
    void updateBaseOptions();
};

#endif // PROFILEMANAGER_H
