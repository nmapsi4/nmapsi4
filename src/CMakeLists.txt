PROJECT(nmapsi4)

configure_file(config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h )

include_directories(${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/src/app/
    ${CMAKE_SOURCE_DIR}/src/app/parser/
    ${CMAKE_SOURCE_DIR}/src/app/monitor/
    ${CMAKE_SOURCE_DIR}/src/app/preference/
    ${CMAKE_SOURCE_DIR}/src/app/profiler/
    ${CMAKE_SOURCE_DIR}/src/common/
    ${CMAKE_SOURCE_DIR}/src/platform/
    ${CMAKE_SOURCE_DIR}/src/platform/logwriter/
    ${CMAKE_SOURCE_DIR}/src/platform/history/
    ${CMAKE_SOURCE_DIR}/src/platform/about/
)


if (Q_WS_X11)
QT4_ADD_DBUS_ADAPTOR(nmapsi4adaptor_SRCS
                     dbus/org.nmapsi4.Nmapsi4.xml
                     src/app/monitor/monitor.h
                     monitor)
endif (Q_WS_X11)

SET(SOURCES_CPP
		app/parser/parsermanager.cpp
		app/parser/parserObjects.cpp
		app/monitor/monitor.cpp
		app/monitor/details.cpp
		app/preference/preferencesdialog.cpp
                app/profiler/profilerdialog.cpp
                app/profiler/profilerdialogoptions.cpp
                app/loadDefaultProfile.cpp
                app/bookmarkcallevents.cpp
                app/updateUI.cpp
                app/slots.cpp
                app/resizeuievents.cpp
                app/defaultuisettings.cpp
                app/actions.cpp
                app/settings.cpp
                app/mainScan.cpp
                app/mainwindow.cpp
                app/main.cpp
		common/utilities.cpp
                common/qpushbuttonorientated.cpp
		common/qprocessthread.cpp
		platform/addvulnerabilityurl.cpp
                platform/vulnerability.cpp
		platform/lookupmanager.cpp
		platform/digmanager.cpp
		platform/nsemanager.cpp
		platform/discover.cpp
		platform/discovermanager.cpp
		platform/addparameterstobookmark.cpp
                platform/logwriter/logwriter.cpp
                platform/history/history.cpp
                platform/about/about.cpp
)


if (Q_WS_X11)
    SET(SOURCES_CPP ${SOURCES_CPP} ${nmapsi4adaptor_SRCS})
endif (Q_WS_X11)

SET(SOURCES_H app/mainwindow.h
	platform/lookupmanager.h
	platform/digmanager.h
	platform/addvulnerabilityurl.h
        platform/vulnerability.h
	platform/addparameterstobookmark.h
	platform/discover.h
        platform/discovermanager.h
        platform/nsemanager.h
        common/utilities.h
	common/qprocessthread.h
	app/preference/preferencesdialog.h
	app/monitor/details.h
        app/monitor/monitor.h
        app/profiler/profilerdialog.h
	app/parser/parsermanager.h
)

SET(SOURCES_UI app/mainwindow.ui
	platform/addvulnerabilityurl.ui
	platform/addparameterstobookmark.ui
        platform/about/about.ui
	app/preference/preferencesdialog.ui
        app/profiler/profilerdialog.ui
	app/monitor/details.ui
)

SET(RESOURCES_QRC ../mainwin.qrc)

qt4_add_RESOURCES(RESOURCES_QRC_SOURCES  ${RESOURCES_QRC} )
QT4_WRAP_UI(SOURCES_UI_H ${SOURCES_UI})
QT4_WRAP_CPP( SOURCES_CPP ${SOURCES_H} )

if (RELOAD_TRANSLATIONS)
  QT4_CREATE_TRANSLATION(qms ${SOURCES_CPP} ${SOURCES_UI_H} ts/nmapsi4_it.ts
    ts/nmapsi4_fr.ts ts/nmapsi4_es.ts ts/nmapsi4_de.ts ts/nmapsi4_cs.ts
    ts/template/nmapsi4.ts ts/nmapsi4_ru.ts OPTIONS -no-obsolete)
  message( "nmapsi4:: update and rebuild translation " )
else (NOT RELOAD_TRANSLATIONS)
  QT4_ADD_TRANSLATION(qms ts/nmapsi4_it.ts ts/nmapsi4_fr.ts
    ts/nmapsi4_es.ts ts/nmapsi4_de.ts ts/nmapsi4_cs.ts ts/nmapsi4_ru.ts)
  message( "nmapsi4:: update translation " )
endif (RELOAD_TRANSLATIONS)

if (Q_WS_X11)
  ADD_EXECUTABLE(nmapsi4 ${SOURCES_H} ${SOURCES_CPP} ${SOURCES_UI_H} ${RESOURCES_QRC_SOURCES} ${qms})
  INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR} )
  TARGET_LINK_LIBRARIES( nmapsi4 ${QT_LIBRARIES} ${QT_QTWEBKIT_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${QT_QTDBUS_LIBRARY} )
elseif(WIN32)
  ADD_EXECUTABLE(nmapsi4 WIN32 ${SOURCES_H} ${SOURCES_CPP} ${SOURCES_UI_H} ${RESOURCES_QRC_SOURCES} ${qms})
  INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR} )
  TARGET_LINK_LIBRARIES( nmapsi4 ${QT_LIBRARIES} ${QT_QTWEBKIT_LIBRARY} ${QT_QTNETWORK_LIBRARY} )
elseif(Q_WS_MAC)
  MESSAGE( STATUS MAC Osx)
  ADD_EXECUTABLE(nmapsi4 MACOSX_BUNDLE ${SOURCES_H} ${SOURCES_CPP} ${SOURCES_UI_H} ${RESOURCES_QRC_SOURCES} ${qms})
  INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR} )
  TARGET_LINK_LIBRARIES( nmapsi4 ${QT_LIBRARIES} ${QT_QTWEBKIT_LIBRARY} ${QT_QTNETWORK_LIBRARY} )
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

