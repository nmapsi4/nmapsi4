PROJECT(nmapsi4-logr)
#cmake_minimum_required(VERSION 2.6.0)
#ADD_DEFINITIONS(-O2 -Wall -Wextra)

#FIND_PACKAGE(Qt4 REQUIRED)
#include(${QT_USE_FILE})

#configure_file(config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h )

SET(SOURCES_CPP	src/mainwin.cpp 
                src/logrApi.cpp
		src/loghistory.cpp
		../lib/loghistory.cpp
		src/main.cpp )
MESSAGE( STATUS ${SOURCES_CPP} )

SET(SOURCES_H src/mainwin.h)
MESSAGE( STATUS ${SOURCES_H} )

SET(SOURCES_UI src/mainwin.ui )
MESSAGE( STATUS ${SOURCES_UI} )

SET(RESOURCES_QRC ../mainwin.qrc)
MESSAGE( STATUS ${RESOURCES_QRC} )

qt4_add_RESOURCES(RESOURCES_QRC_SOURCES  ${RESOURCES_QRC} )
QT4_WRAP_UI(SOURCES_UI_H ${SOURCES_UI})
QT4_WRAP_CPP( SOURCES_CPP ${SOURCES_H} )

QT4_CREATE_TRANSLATION(qms ${SOURCES_CPP} ${SOURCES_UI_H} ts/nmapsi4-logr_it.ts 
	ts/nmapsi4-logr_fr.ts ts/nmapsi4-logr_es.ts ts/nmapsi4-logr_de.ts ts/nmapsi4-logr_pl.ts)

#QT4_ADD_TRANSLATION(qms ts/nmapsi4-logr_it.ts ts/nmapsi4-logr_fr.ts ts/nmapsi4-logr_es.ts nmapsi4-logr_de.ts ts/nmapsi4-logr_pl.ts)

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} )
#ADD_EXECUTABLE(nmapsi4 ${SOURCES_H} ${SOURCES_CPP} ${SOURCES_UI_H} ${RESOURCES_QRC_SOURCES} ${qms})
ADD_EXECUTABLE(nmapsi4-logr ${SOURCES_H} ${SOURCES_CPP} ${SOURCES_UI_H} ${RESOURCES_QRC_SOURCES} ${qms})
INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR} )
TARGET_LINK_LIBRARIES( nmapsi4-logr ${QT_LIBRARIES} )


#if (Q_WS_X11)
#  install(FILES ${qms} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/nmapsi4/locale)
#  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/nmapsi4" DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
#  install(FILES "src/kde/nmapsi4.desktop" DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications/kde/)
#  install(FILES "src/kde/nmapsi4-admin.desktop" DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications/kde/)
#  install(FILES "icons/16x16/nmapsi4.png" DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/16x16/apps/)
#  install(FILES "icons/32x32/nmapsi4.png" DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/32x32/apps/)
#  install(FILES "icons/64x64/nmapsi4.png" DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/64x64/apps/)
#  install(FILES "icons/128x128/nmapsi4.png" DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/128x128/apps/)
#endif (Q_WS_X11)

