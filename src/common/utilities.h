/*
Copyright 2011-2012  Francesco Cecconi <francesco.cecconi@gmail.com>

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

#ifndef UTILITIES_H
#define UTILITIES_H

#include <QtGui/QFileDialog>
#include <QtGui/QLineEdit>
#include <QtGui/QDesktopServices>
#include <QtCore/QObject>
#include <QtCore/QDir>
#include <QtCore/QUrl>

// local include
#include "about.h"

class MainWindow;

class Utilities : public QObject
{
    Q_OBJECT

public:
    Utilities(MainWindow* parent);
    ~Utilities();
    void openDirectoryDialog(QLineEdit *destination);
    void openFileBrowser(QLineEdit *destination);

private:
    void showBrowser(QLineEdit *destination);
    MainWindow* m_ui;

public slots:
    void about();
    void aboutQt();
    void showBugUrl();
    void showHomepageUrl();
    void showDocumentationUrl();
    void showDonateUrl();
};

#endif // UTILITIES_H
