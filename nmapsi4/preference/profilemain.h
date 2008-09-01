/***************************************************************************
 *   Copyright (C) 2007-2008 by Francesco Cecconi                          *
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

#ifndef PROFILEMAIN_H
#define PROFILEMAIN_H

#include <QtGui>
#include "ui_profilemain.h"

class mainProfile : public QDialog, private Ui::ProfileMain
{
    Q_OBJECT

private:
    void saveProfile(QString ProfileType); // Create a enum for the profile
    QString readProfile();
    void setProfile();

public:
    mainProfile();
    ~mainProfile();

protected:
    QString ScanActive;
    QListWidgetItem *profileItem, *logItem, *sizeItem;

public slots:
    void setScan();

private slots:
    void updateNormalCheck();
    void updateQuickCheck();
    void updateFullVersionCheck();
    void updateQuickVersionCheck();
    void updateItem();
    void log_browser();
    void update_saveButton();
    void quit();
    void setDefaults();

};

#endif

