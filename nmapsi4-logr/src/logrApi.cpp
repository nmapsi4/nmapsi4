#include "mainwin.h"


QString mwClass::showBrowser() {

     QString url = QFileDialog::getOpenFileName(this, tr("Open Log"),
						QDir::homePath(), tr("Log (*.log)"));
     
     return url;
}

