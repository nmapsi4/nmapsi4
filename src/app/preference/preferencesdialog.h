/*
Copyright 2007-2012  Francesco Cecconi <francesco.cecconi@gmail.com>

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

#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QtGui/QDialog>
#include <QtCore/QSettings>

// local include
#include "ui_preferencesdialog.h"

class PreferencesDialog : public QDialog, private Ui::ProfileMain
{
    Q_OBJECT

public:
    PreferencesDialog(QWidget *parent = 0);
    ~PreferencesDialog();

private:
    QListWidgetItem* m_generalItem;
    QListWidgetItem* m_lookItem;

public slots:
    void saveValues();

private slots:
    void updateListWidgetItem();
    void quit();
    void setDefaults();
    void updateLookupState();
};

#endif
