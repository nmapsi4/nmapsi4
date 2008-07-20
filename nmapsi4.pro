######################################################################
# Nmapsi4 qmake project 
######################################################################

TEMPLATE = app
TARGET = nmapsi4
DEPENDPATH += .
INCLUDEPATH += .

unix {
	system(cp tools/config.h .):HAS_BIN=FALSE
}

win32 {
	system(xcopy tools\config.h /e /Y):HAS_BIN=FALSE
}

# Input
HEADERS += nmapsi4/mainwin.h \
	nmapsi4/preference/profilemain.h 

FORMS += nmapsi4/mainwin.ui \
	nmapsi4/preference/profilemain.ui

SOURCES += 	nmapsi4/core/nmapsi4Slot.cpp \
		nmapsi4/core/resizeNmapsi4.cpp \
		nmapsi4/preference/profilemain.cpp \
		nmapsi4/preference/profilemainupdate.cpp \
		nmapsi4/core/nmapParser.cpp \
		nmapsi4/core/extensions.cpp \
		nmapsi4/mainwin.cpp \
		nmapsi4/core/updateUI.cpp \
		nmapsi4/core/toolsUI.cpp \
		nmapsi4/core/profileSession.cpp \
		nmapsi4/core/saveLog.cpp \
		nmapsi4/main.cpp

RESOURCES = mainwin.qrc
TRANSLATIONS = nmapsi4/ts/nmapsi4_it.ts \
		nmapsi4/ts/nmapsi4_NONE.ts \
		nmapsi4/ts/nmapsi4_de.ts \
		nmapsi4/ts/nmapsi4_fr.ts \
		nmapsi4/ts/nmapsi4_es.ts \
		nmapsi4/ts/nmapsi4_pl.ts 
		
QMAKE_CLEAN = config.h Makefile nmapsi4

unix {
	#install#
		target.path = /usr/bin
		menuico16.files = icons/16x16/nmapsi4.png
		menuico16.path = /usr/share/icons/hicolor/16x16/apps/
		menuico32.files = icons/32x32/nmapsi4.png
		menuico32.path = /usr/share/icons/hicolor/32x32/apps/
		menuico64.files = icons/64x64/nmapsi4.png
		menuico64.path = /usr/share/icons/hicolor/64x64/apps/
		menuico128.files = icons/128x128/nmapsi4.png
		menuico128.path = /usr/share/icons/hicolor/128x128/apps/
		translation.files = nmapsi4.qm
		translation.path = /usr/share/nmapsi4/locale
		INSTALLS += target menuico16 menuico32 menuico64 menuico128 translation
}
