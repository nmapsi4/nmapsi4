PROJECT(nmapsi4)

configure_file(config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h )

include_directories(${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/nmapsi4/app/
    ${CMAKE_SOURCE_DIR}/nmapsi4/app/parser/
    ${CMAKE_SOURCE_DIR}/nmapsi4/app/monitor/
    ${CMAKE_SOURCE_DIR}/nmapsi4/app/preference/
    ${CMAKE_SOURCE_DIR}/nmapsi4/common/
    ${CMAKE_SOURCE_DIR}/nmapsi4/platform/digSupport/
    ${CMAKE_SOURCE_DIR}/nmapsi4/platform/discover/
    ${CMAKE_SOURCE_DIR}/nmapsi4/platform/bookmarks/
    ${CMAKE_SOURCE_DIR}/nmapsi4/platform/vulnerability/
    ${CMAKE_SOURCE_DIR}/nmapsi4/platform/lookup/
    ${CMAKE_SOURCE_DIR}/lib/about/
    ${CMAKE_SOURCE_DIR}/lib/history/
)


if (Q_WS_X11)
QT4_ADD_DBUS_ADAPTOR(nmapsi4adaptor_SRCS
                     dbus/org.nmapsi4.Nmapsi4.xml
                     nmapsi4/app/monitor/monitor.h
                     monitor)
endif (Q_WS_X11)

SET(SOURCES_CPP 
		app/parser/parser.cpp
                app/parser/parserSlotsDefinition.cpp
		app/parser/parserObjects.cpp
		app/monitor/monitor.cpp
		app/monitor/details.cpp
		app/preference/profilemain.cpp
		app/preference/profilemainupdate.cpp
		common/utilities.cpp
                common/qpushbuttonorientated.cpp
		common/qprocessthread.cpp
		platform/extensions.cpp
		platform/updateUI.cpp
                platform/slots.cpp
		platform/resizeNmapsi4.cpp
		platform/mainWinTools.cpp
		platform/profile/globalProfile.cpp
		platform/profile/profileSession.cpp
		platform/profile/defaultProfile.cpp
		platform/logUtils.cpp
		platform/vulnerability/searchVuln.cpp
		platform/vulnerability/defaultUrlVuln.cpp
		platform/vulnerability/tabVuln.cpp
		platform/vulnerability/utilsVuln.cpp
		platform/vulnerability/addurl.cpp
		platform/lookup/lookUpT.cpp
		platform/digSupport/digSupport.cpp
		platform/actions.cpp
                platform/bookmarks/bookCalls.cpp
                platform/saveUiSettings.cpp
		platform/nse/nseCalls.cpp
		platform/nse/nseUpdateCalls.cpp
		platform/startupTools.cpp
		platform/discover/maindiscover.cpp
		platform/discover/discoverCalls.cpp
		platform/bookmarks/addparbook.cpp
		platform/mainScan.cpp
                app/mainwin.cpp
		app/main.cpp)


if (Q_WS_X11)
    SET(SOURCES_CPP ${SOURCES_CPP} ${nmapsi4adaptor_SRCS})
endif (Q_WS_X11)

SET(SOURCES_H app/mainwin.h 
	platform/lookup/lookUpT.h 
	platform/digSupport/digSupport.h 
	platform/vulnerability/addurl.h 
	platform/bookmarks/addparbook.h
	platform/discover/maindiscover.h
        common/utilities.h
	common/qprocessthread.h
	app/preference/profilemain.h 
	app/monitor/details.h
        app/monitor/monitor.h
	app/parser/parser.h)

SET(SOURCES_UI app/mainwin.ui 
	platform/vulnerability/addurl.ui 
	platform/bookmarks/addparbook.ui
	app/preference/profilemain.ui 
	app/monitor/details.ui)

SET(RESOURCES_QRC ../mainwin.qrc)

qt4_add_RESOURCES(RESOURCES_QRC_SOURCES  ${RESOURCES_QRC} )
QT4_WRAP_UI(SOURCES_UI_H ${SOURCES_UI})
QT4_WRAP_CPP( SOURCES_CPP ${SOURCES_H} )

if (RELOAD_TRANSLATIONS)
  QT4_CREATE_TRANSLATION(qms ${SOURCES_CPP} ${SOURCES_UI_H} ../lib/history/loghistory.cpp ts/nmapsi4_it.ts 
    ts/nmapsi4_fr.ts ts/nmapsi4_es.ts ts/nmapsi4_de.ts 
    ts/nmapsi4_pl.ts ts/nmapsi4_pt_BR.ts ts/nmapsi4_cs.ts 
    ts/template/nmapsi4.ts ts/nmapsi4_ru.ts OPTIONS -no-obsolete)
  message( "nmapsi4:: update and rebuild translation " )  
else (NOT RELOAD_TRANSLATIONS)
  QT4_ADD_TRANSLATION(qms ts/nmapsi4_it.ts ts/nmapsi4_fr.ts 
    ts/nmapsi4_es.ts ts/nmapsi4_de.ts ts/nmapsi4_pl.ts ts/nmapsi4_pt_BR.ts ts/nmapsi4_cs.ts ts/nmapsi4_ru.ts)
  message( "nmapsi4:: update translation " )
endif (RELOAD_TRANSLATIONS)

if (Q_WS_X11)
  ADD_EXECUTABLE(nmapsi4 ${SOURCES_H} ${SOURCES_CPP} ${SOURCES_UI_H} ${RESOURCES_QRC_SOURCES} ${qms})
  INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR}
	    ${CMAKE_BINARY_DIR}/lib/history/
	    ${CMAKE_BINARY_DIR}/lib/about/
  )
  TARGET_LINK_LIBRARIES( nmapsi4 ${QT_LIBRARIES} ${QT_QTWEBKIT_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${QT_QTDBUS_LIBRARY} nmapsi4_history nmapsi4_about)
elseif(WIN32)
  ADD_EXECUTABLE(nmapsi4 WIN32 ${SOURCES_H} ${SOURCES_CPP} ${SOURCES_UI_H} ${RESOURCES_QRC_SOURCES} ${qms})
  INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR} ${CMAKE_BINARY_DIR}/lib/history/ ${CMAKE_BINARY_DIR}/lib/about/)
  TARGET_LINK_LIBRARIES( nmapsi4 ${QT_LIBRARIES} ${QT_QTWEBKIT_LIBRARY} ${QT_QTNETWORK_LIBRARY} nmapsi4_history nmapsi4_about)
elseif(Q_WS_MAC)
  MESSAGE( STATUS MAC Osx)
  ADD_EXECUTABLE(nmapsi4 MACOSX_BUNDLE ${SOURCES_H} ${SOURCES_CPP} ${SOURCES_UI_H} ${RESOURCES_QRC_SOURCES} ${qms})
  INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR} ${CMAKE_BINARY_DIR}/lib/history/ ${CMAKE_BINARY_DIR}/lib/about/)
  TARGET_LINK_LIBRARIES( nmapsi4 ${QT_LIBRARIES} ${QT_QTWEBKIT_LIBRARY} ${QT_QTNETWORK_LIBRARY} nmapsi4_history nmapsi4_about)
  ADD_CUSTOM_COMMAND( TARGET nmapsi4 POST_BUILD
        COMMAND mkdir ARGS ${CMAKE_CURRENT_BINARY_DIR}/nmapsi4.app/Contents/Resources
        COMMAND cp ARGS ${MACOSX_BUNDLE_ICON_FILE} ${CMAKE_CURRENT_BINARY_DIR}/nmapsi4.app/Contents/Resources
        COMMAND cp ARGS *.qm ${CMAKE_CURRENT_BINARY_DIR}/nmapsi4.app/Contents/Resources)
endif (Q_WS_X11)


if (Q_WS_X11)
  install(FILES ${qms} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/nmapsi4/locale)
  install(TARGETS nmapsi4 DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
  install(FILES "desktop/nmapsi4.desktop" DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications/kde4/)
  install(FILES "desktop/nmapsi4-admin.desktop" DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications/kde4/)
  install(FILES "dbus/org.nmapsi4.Nmapsi4.xml" DESTINATION ${CMAKE_INSTALL_PREFIX}/share/dbus-1/interfaces/)
  install(FILES "../icons/32x32/nmapsi4.png" DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/32x32/apps/)
  install(FILES "../icons/64x64/nmapsi4.png" DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/64x64/apps/)
  install(FILES "../icons/128x128/nmapsi4.png" DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/128x128/apps/)
endif (Q_WS_X11)

