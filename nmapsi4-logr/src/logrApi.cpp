#include "mainwin.h"


QString mwClass::showBrowser() {

     QString url = QFileDialog::getOpenFileName(this, tr("Open Log"),
						QDir::homePath(), tr("Log (*.log)"));
     
     return url;
}

void mwClass::itemDeleteAll(QList<QTreeWidgetItem*> items) {
     qDebug() << "nwClass/itemDeleteAll() -> Free List";
     foreach(QTreeWidgetItem *item, items) {
	  delete item;
     }
}

void mwClass::exit() {
     QSettings settings("nmapsi4","nmapsi4");
     settings.setValue("window/pos", pos());
     settings.setValue("window/size", size());
     close();
}
