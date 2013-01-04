/*
Copyright 2012-2013  Francesco Cecconi <francesco.cecconi@gmail.com>

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

#ifndef PROFILERMANAGER_H
#define PROFILERMANAGER_H

#include <QDialog>
#include <QListWidgetItem>
#include <QMessageBox>
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

class MainWindow;

class ProfilerManager : public QDialog
{
    Q_OBJECT

public:
    explicit ProfilerManager(MainWindow* parent);
    ProfilerManager(const QString profileName, const QString parameters, MainWindow* parent);
    virtual ~ProfilerManager();

    NseManager* m_nseManager;
    Ui::profilerDialog *m_dialogUi;

private:
    void initObject();
    void loadDefaultComboValues();
    void createQList();
    void reloadScanParameters();
    void loadDefaultBaseProfile();

    QListWidgetItem *m_profileW;
    QListWidgetItem *m_scanW;
    QListWidgetItem *m_toolW;
    QListWidgetItem *m_discoverW;
    QListWidgetItem *m_timingW;
    QListWidgetItem *m_nseW;
    QListWidgetItem *m_targetW;
    int m_userId;
    Profiler *m_profiler;
    MainWindow *m_ui;

signals:
    void doneParBook(const QString profileName, const QString profilePar);

private slots:
    void optionListUpdate();
    void updatePortCombo();
    void updateOptions();
    void updateComboVerbosity();
    void exit();
    void updateBaseOptions();
};

#endif // PROFILEMANAGER_H
