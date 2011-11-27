PROJECT(nmapsi4-logr)

configure_file(config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h )

SET(SOURCES_CPP src/mainwin.cpp 
                src/logrApi.cpp	
		main.cpp)

SET(SOURCES_H src/mainwin.h)
SET(SOURCES_UI src/mainwin.ui)
SET(RESOURCES_QRC ../mainwin.qrc)

qt4_add_RESOURCES(RESOURCES_QRC_SOURCES  ${RESOURCES_QRC} )
QT4_WRAP_UI(SOURCES_UI_H ${SOURCES_UI})
QT4_WRAP_CPP( SOURCES_CPP ${SOURCES_H} )

if (RELOAD_TRANSLATIONS)
  QT4_CREATE_TRANSLATION(qms ${SOURCES_CPP} ${SOURCES_UI_H} ts/nmapsi4-logr_it.ts 
    ts/nmapsi4-logr_fr.ts ts/nmapsi4-logr_es.ts ts/nmapsi4-logr_de.ts ts/nmapsi4-logr_pl.ts
    ts/nmapsi4-logr_cs.ts ts/template/nmapsi4-logr.ts OPTIONS -no-obsolete)
  message( "nmapsi4-logr:: update and rebuild translation " )  
else (NOT RELOAD_TRANSLATIONS)
  QT4_ADD_TRANSLATION(qms ts/nmapsi4-logr_it.ts	ts/nmapsi4-logr_fr.ts 
    ts/nmapsi4-logr_es.ts ts/nmapsi4-logr_de.ts ts/nmapsi4-logr_pl.ts ts/nmapsi4-logr_cs.ts)
  message( "nmapsi4-logr:: update translation " )
endif (RELOAD_TRANSLATIONS)

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR})

if (WIN32)
  ADD_EXECUTABLE(nmapsi4-logr WIN32 ${SOURCES_H} ${SOURCES_CPP} ${SOURCES_UI_H} ${RESOURCES_QRC_SOURCES} ${qms})
  INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR} ${CMAKE_BINARY_DIR}/lib/about/ ${CMAKE_BINARY_DIR}/lib/history/)
  TARGET_LINK_LIBRARIES( nmapsi4-logr ${QT_LIBRARIES} nmapsi4_history nmapsi4_about)
else (WIN32)
  ADD_EXECUTABLE(nmapsi4-logr ${SOURCES_H} ${SOURCES_CPP} ${SOURCES_UI_H} ${RESOURCES_QRC_SOURCES} ${qms})
  INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR} ${CMAKE_BINARY_DIR}/lib/about/ ${CMAKE_BINARY_DIR}/lib/history/)
  TARGET_LINK_LIBRARIES( nmapsi4-logr ${QT_LIBRARIES} nmapsi4_history nmapsi4_about)
endif (WIN32)


if (Q_WS_X11)
  install(FILES ${qms} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/nmapsi4/locale)
  install(TARGETS nmapsi4-logr DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
  install(FILES "src/nmapsi4-logr.desktop" DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications/kde4/)
endif (Q_WS_X11)

