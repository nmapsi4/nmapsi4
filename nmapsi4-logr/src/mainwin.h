#ifndef MAINWIN_H
#define MAINWIN_H

#include <QtGui>
#include "ui_mainwin.h"


class mwClass : public QMainWindow , private Ui::mWindow
{
     Q_OBJECT

private:
     QString showBrowser();
     QTreeWidgetItem *root;
     QTreeWidgetItem *item;
     QList<QTreeWidgetItem*> ItemList;
     void itemDeleteAll(QList<QTreeWidgetItem*> itemsList);

public:
     mwClass();
     ~mwClass();

protected:
     QString url;

private slots:
     void logReader();


};

#endif
