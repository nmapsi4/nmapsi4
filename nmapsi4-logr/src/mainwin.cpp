#include "mainwin.h"


mwClass::mwClass() {
     setupUi(this);

     connect( actionOpen, SIGNAL(triggered()), 
	      this, SLOT(logReader()));
}

mwClass::~mwClass() {
}

void mwClass::logReader() {
     // TODO
     // Read log
     if((url = showBrowser()).isEmpty())
	  return;

     qDebug() << "nmapsi4-logr:: --> url::" << url;
}
