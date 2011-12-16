/***************************************************************************
 *   Copyright (C) 2011 by Francesco Cecconi                               *
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

#ifndef ADDPARAMETERSTOBOOKMARK_H
#define ADDPARAMETERSTOBOOKMARK_H

#include <QtGui/QDialog>
#include "ui_addparameterstobookmark.h"

class addParametersToBookmark : public QDialog, private Ui::addParBook
{
    Q_OBJECT

public:
    addParametersToBookmark(const QString parameters);
    ~addParametersToBookmark();

private slots:
    void exit();

signals:
    void doneParBook(const QString profileName, const QString profilePar);
};
#endif
