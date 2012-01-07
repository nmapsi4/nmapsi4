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


#ifndef PROFILEDIALOG_H
#define PROFILEDIALOG_H

#include <QtGui/QDialog>
#include <QtGui/QListWidgetItem>
#include <QtGui/QMessageBox>
#include <QtCore/QDebug>

#include "ui_profilerdialog.h"

class profilerManager : public QDialog, private Ui::profilerDialog
{
    Q_OBJECT
public:
    profilerManager(QWidget* parent = 0);
    ~profilerManager();

private:
    QListWidgetItem *scanW;
    QListWidgetItem *toolW;
    QListWidgetItem *discoverW;
    QListWidgetItem *timingW;
    int uid;

    QStringList buildExtensions();
    void loadDefaultComboValues();
    void loadDefaultBaseParameters();
    void setNormalProfile();
    void setFullVersionProfile();
    void resetOptions();
    void createQList();

signals:
    void doneParBook(const QString profileName, const QString profilePar);

private slots:
    void optionListUpdate();
    void update_portCombo();
    void update_discover();
    void update_timing();
    void update_options();
    void update_comboVerbosity();
    void exit();

};

#endif // PROFILEDIALOG_H
