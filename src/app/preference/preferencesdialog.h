/***************************************************************************
 *   Copyright (C) 2007-2012 by Francesco Cecconi                          *
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

#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QtGui/QDialog>
#include <QtCore/QSettings>

// local include
#include "ui_preferencesdialog.h"

class preferencesDialog : public QDialog, private Ui::ProfileMain
{
    Q_OBJECT

public:
    preferencesDialog(QWidget *parent = 0);
    ~preferencesDialog();

private:
    QListWidgetItem* m_generalItem;
    QListWidgetItem* m_sizeItem;
    QListWidgetItem* m_lookItem;

public slots:
    void saveValues();

private slots:
    void updateListWidgetItem();
    void quit();
    void setDefaults();
};

#endif
