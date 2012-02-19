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

#ifndef UTILITIES_H
#define UTILITIES_H

#include <QtGui/QFileDialog>
#include <QtGui/QLineEdit>
#include <QtCore/QDir>
#include <QtGui/QDesktopServices>
#include <QtCore/QUrl>
#include <QtGui/QWidget>

// local include
#include "about.h"


class Utilities : public QWidget
{
    Q_OBJECT

public:
    Utilities(QWidget* parent=0);
    ~Utilities();
    void openDirectoryDialog(QLineEdit *destination);
    void openFileBrowser(QLineEdit *destination);

private:
    void showBrowser(QLineEdit *destination);
    QWidget* m_parent;

public slots:
    void about();
    void aboutQt();
    void showBugUrl();
    void showHomepageUrl();
    void showDocumentationUrl();
    void showDonateUrl();
};

#endif // UTILITIES_H
