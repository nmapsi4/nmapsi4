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

#ifndef MAINWIN_H
#define MAINWIN_H

#include <QProcess>
#include <QtGui>
#include <QtWebKit/QWebView>
#include "ui_mainwin.h"
#include "preference/profilemain.h"
#include "../lib/loghistory.h"
#include "../lib/staticDefine.h"



class nmapClass : public QMainWindow , private Ui::MainWindow
{
  Q_OBJECT

private:
	void rootMode(int uid);						
  	void show_browser(QLineEdit *location);				
	void isEmptyLog();						
	void setNmapsiSlot();						
	QFile* create_logFile(QString Path);				
	QStringList check_extensions(QStringList parametri,QString *winTitle);
	void resetOptions();
	void checkProfile();
	void init();
	void itemDeleteAll(QList<QTreeWidgetItem*> items);

public:
	nmapClass();
	~nmapClass();
	void show_log_browserUrl(QString url, QLineEdit *location);			
	void setQuickProfile();
	void setNormalProfile();
	void setFullVersionProfile();
	void setQuickVersionProfile();

protected:
	QProcess *proc;
	QProcess *versionProc;
	QTreeWidgetItem *root,*item2, *infoItem, *infoItemObj;
	QTreeWidgetItem *root2,*item_root2;
	QTreeWidgetItem *error,*item_error;
	QFile *PFile;
	QString FileName;
	QString firstPath;
	QString Profile;
	QString logPath;
	bool checkLog;
	bool flag_state;
	bool savePos; 
	bool saveSize;
	bool listClearFlag;
	bool verboseLog;
	QList<QTreeWidgetItem*> itemList;
	QString logSessionFile;
	QLabel *labelVersion;
	QLabel *userMode;
	mainProfile *dialog;
	QProgressBar *progressScan;


public slots:
	void scan();
	void about();
	void about_qt();
	void stop_scan();
	void exit();


private slots:
	void nmapParser();
	void update_portCombo();
	void update_scanCombo();
	void update_inputcheck();
	void update_discover();
	void update_timing();
	void update_options();
	void update_comboVerbosity();
	void input_browser();
	void fileSession();
	void listClear();
	void closeTree();
	void checkFullScreen();
	void updateMenuBar();
	void showMainToolBar();
	void showActionToolBar();
	void showStatusBar();
	void updateIconsBox();
	void callScanH();

	// Check nmap version
	void checkNmapVersion();
	void setNmapVersion();

	//Resize UI
	void resize_tab_Options_Scan();
	void resize_tab_Options();
	void resize_tab_MiscOptions();
	void resize_tab_Discover();
	void resize_tab_Files();
	void resize_tab_Timing();
	void startProfile_ui();
	void readProfile();
	void saveAsLog();
	void saveLog();
	void searchVuln();
	virtual void updateFontHost();
	virtual void updateFontHost(QString hostName); 
};

#endif
