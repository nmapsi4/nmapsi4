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

#ifndef SELECTPROFILEDIALOG_H
#define SELECTPROFILEDIALOG_H

#include <QtGui/QDialog>
#include <QtGui/QComboBox>

#include "ui_selectprofiledialog.h"
#include "mainwindow.h"

class SelectProfileDialog : public QDialog, private Ui::selectProfileDialog
{
    Q_OBJECT

public:
    SelectProfileDialog(MainWindow* ui);
    ~SelectProfileDialog();

private:
    void loadComboValues();

    MainWindow* m_ui;

private slots:
    void closeDialog();

};

#endif // SELECTPROFILEDIALOG_H
