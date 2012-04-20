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


#ifndef PROFILERDIALOG_H
#define PROFILERDIALOG_H

#include <QtGui/QDialog>
#include <QtGui/QListWidgetItem>
#include <QtGui/QMessageBox>
#include <QtCore/QDebug>
#include <QtCore/QHash>
#include <QtCore/QPair>

#include "ui_profilerdialog.h"
#include "nsemanager.h"

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

    friend class NseManager;

public:
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
    NseManager* m_nseManager;
    int m_uid;
    QHash<QString,QPair<QComboBox*,int> > preLoadComboList;
    QHash<QString,QPair<QCheckBox*,QString> > preLoadCheckBoxList;
    QHash<QString,QLineEdit*> lineEditList;
    QHash<QString,QSpinBox*> spinBoxList;

    void initObject();
    void loadDefaultHash();
    QStringList buildExtensions();
    void loadDefaultComboValues();
    void setNormalProfile();
    void setFullVersionProfile();
    void setDefaultNseScripts();
    void resetOptions();
    void createQList();
    void reloadScanParameters();
    void preLoadOptionsCombo();
    void preLoadOptionsCheckBox();
    void restoreValuesFromProfile(const QStringList parameters);

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

#endif // PROFILEDIALOG_H
