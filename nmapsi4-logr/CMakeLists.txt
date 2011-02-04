PROJECT(nmapsi4-logr)

configure_file(config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h )

if (WIN32)
SET(SOURCES_CPP	mainwin.cpp 
                src/logrApi.cpp
		../lib/history/loghistory.cpp
		../lib/history/coreHistory.cpp
		../lib/about/about.cpp
		src/main.cpp)
#MESSAGE( STATUS ${SOURCES_CPP} )
else (WIN32)
SET(SOURCES_CPP mainwin.cpp 
                src/logrApi.cpp	
		src/main.cpp)
#MESSAGE( STATUS ${SOURCES_CPP} )

endif(WIN32)

if(NOT WIN32)
	SET(SOURCES_H mainwin.h)
	#MESSAGE( STATUS ${SOURCES_H} )
else(WIN32)
	SET(SOURCES_H mainwin.h ../lib/about/about.h)
	#MESSAGE( STATUS ${SOURCES_H} )
endif(NOT WIN32)

if(NOT WIN32)
	SET(SOURCES_UI src/mainwin.ui )
	#MESSAGE( STATUS ${SOURCES_UI} )
else(WIN32)
	SET(SOURCES_UI src/mainwin.ui ../lib/about/about.ui )
	#MESSAGE( STATUS ${SOURCES_UI} )
endif(NOT WIN32)

SET(RESOURCES_QRC ../mainwin.qrc)
#MESSAGE( STATUS ${RESOURCES_QRC} )

qt4_add_RESOURCES(RESOURCES_QRC_SOURCES  ${RESOURCES_QRC} )
QT4_WRAP_UI(SOURCES_UI_H ${SOURCES_UI})
QT4_WRAP_CPP( SOURCES_CPP ${SOURCES_H} )

if (RELOAD_TRANSLATIONS)
  QT4_CREATE_TRANSLATION(qms ${SOURCES_CPP} ${SOURCES_UI_H} ts/nmapsi4-logr_it.ts 
    ts/nmapsi4-logr_fr.ts ts/nmapsi4-logr_es.ts ts/nmapsi4-logr_de.ts ts/nmapsi4-logr_pl.ts
    ts/nmapsi4-logr_cs.ts)
  message( "nmapsi4-logr:: update and rebuild translation " )  
else (NOT RELOAD_TRANSLATIONS)
  QT4_ADD_TRANSLATION(qms ts/nmapsi4-logr_it.ts	ts/nmapsi4-logr_fr.ts 
    ts/nmapsi4-logr_es.ts ts/nmapsi4-logr_de.ts ts/nmapsi4-logr_pl.ts ts/nmapsi4-logr_cs.ts)
  message( "nmapsi4-logr:: update translation " )
endif (RELOAD_TRANSLATIONS)

ADD_EXECUTABLE(nmapsi4-logr ${SOURCES_H} ${SOURCES_CPP} ${SOURCES_UI_H} ${RESOURCES_QRC_SOURCES} ${qms})

if (WIN32)
  INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR})
  INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR} )
  TARGET_LINK_LIBRARIES( nmapsi4-logr ${QT_LIBRARIES})
else (NOT WIN32)
  INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/../lib/history/)
  INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR} ${CMAKE_BINARY_DIR}/lib/about/)
  TARGET_LINK_LIBRARIES( nmapsi4-logr ${QT_LIBRARIES} nmapsi4_history nmapsi4_about)
endif (WIN32)


if (Q_WS_X11)
  install(FILES ${qms} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/nmapsi4/locale)
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/nmapsi4-logr" DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
  install(FILES "src/nmapsi4-logr.desktop" DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications/kde4/)
endif (Q_WS_X11)

