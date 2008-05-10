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
HEADERS += src/mainwin.h \
	src/preference/profilemain.h 

FORMS += src/mainwin.ui \
	src/preference/profilemain.ui

SOURCES += 	src/core/nmapsi4Slot.cpp \
		src/core/resizeNmapsi4.cpp \
		src/preference/profilemain.cpp \
		src/preference/profilemainupdate.cpp \
		src/core/nmapParser.cpp \
		src/core/extensions.cpp \
		src/mainwin.cpp \
		src/core/updateUI.cpp \
		src/core/toolsUI.cpp \
		src/core/profileSession.cpp \
		src/core/saveLog.cpp \
		src/main.cpp

RESOURCES = mainwin.qrc
TRANSLATIONS = ts/nmapsi4_it.ts \
		ts/nmapsi4_NONE.ts \
		ts/nmapsi4_de.ts \
		ts/nmapsi4_fr.ts \
		ts/nmapsi4_es.ts \
		ts/nmapsi4_pl.ts 
		
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
