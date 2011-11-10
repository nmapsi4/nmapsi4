PROJECT(nmapsi4)

configure_file(config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h )

QT4_ADD_DBUS_ADAPTOR(nmapsi4adaptor_SRCS
                     dbus/org.nmapsi4.Nmapsi4.xml
                     nmapsi4/core/monitor/monitor.h
                     monitor)

if (NOT Q_WS_X11)
SET(SOURCES_CPP core/slots.cpp
		core/resizeNmapsi4.cpp
                core/parser/parserUtils.cpp
		core/parser/nmapParser.cpp
                core/parser/scanParser.cpp
                core/parser/pluginsParser.cpp
		core/extensions.cpp
		core/hostCheck.cpp
		mainwin.cpp
		core/updateUI.cpp
		core/toolsUI.cpp
		core/mainWinTools.cpp
		core/preference/profilemain.cpp
		core/preference/profilemainupdate.cpp
		core/profile/globalProfile.cpp
		core/profile/profileSession.cpp
		core/profile/defaultProfile.cpp
		core/logUtils.cpp
		core/vulnerability/searchVuln.cpp
		core/vulnerability/defaultUrlVuln.cpp
		core/vulnerability/tabVuln.cpp
		core/vulnerability/utilsVuln.cpp
		core/vulnerability/addurl.cpp
		../lib/history/loghistory.cpp
		../lib/history/coreHistory.cpp
		../lib/about/about.cpp
		core/scanMT/scanThread.cpp
		core/lookup/lookUpT.cpp
		core/monitor/monitor.cpp
		core/monitor/monitorEvents.cpp
		core/monitor/details.cpp
		core/digSupport/digSupport.cpp
		core/digSupport/digThread.cpp
		core/parserObj/parserObj.cpp
		core/parserObj/parserObjUtil.cpp
		core/actionMenu.cpp
                core/bookmarks/bookCalls.cpp
                core/saveUiSettings.cpp
		core/nse/nseCalls.cpp
		core/nse/nseUpdateCalls.cpp
		core/startupTools.cpp
		core/discover/maindiscover.cpp
		core/discover/discoverCalls.cpp
		core/discover/pingThread.cpp
		core/bookmarks/addparbook.cpp
		core/mainScan.cpp
		main.cpp)
#MESSAGE( STATUS ${SOURCES_CPP} )
else (NOT Q_WS_X11)
  SET(SOURCES_CPP  ${nmapsi4adaptor_SRCS}
                core/slots.cpp
		core/resizeNmapsi4.cpp
		core/parser/parserUtils.cpp
		core/parser/nmapParser.cpp
                core/parser/scanParser.cpp
                core/parser/pluginsParser.cpp
		core/extensions.cpp
		core/hostCheck.cpp
		mainwin.cpp
		core/updateUI.cpp
		core/toolsUI.cpp
		core/mainWinTools.cpp
		core/preference/profilemain.cpp
		core/preference/profilemainupdate.cpp
		core/profile/globalProfile.cpp
		core/profile/profileSession.cpp
		core/profile/defaultProfile.cpp
		core/logUtils.cpp
		core/vulnerability/searchVuln.cpp
		core/vulnerability/defaultUrlVuln.cpp
		core/vulnerability/tabVuln.cpp
		core/vulnerability/utilsVuln.cpp
		core/vulnerability/addurl.cpp
		core/scanMT/scanThread.cpp
		core/lookup/lookUpT.cpp
		core/monitor/monitor.cpp
		core/monitor/monitorEvents.cpp
		core/monitor/details.cpp
		core/digSupport/digSupport.cpp
		core/digSupport/digThread.cpp
		core/parserObj/parserObj.cpp
		core/parserObj/parserObjUtil.cpp
		core/actionMenu.cpp
                core/bookmarks/bookCalls.cpp
                core/saveUiSettings.cpp
		core/nse/nseCalls.cpp
		core/nse/nseUpdateCalls.cpp
		core/startupTools.cpp
		core/discover/maindiscover.cpp
		core/discover/discoverCalls.cpp
		core/discover/pingThread.cpp
		core/bookmarks/addparbook.cpp
		core/mainScan.cpp
		main.cpp)

#MESSAGE( STATUS ${SOURCES_CPP} )
endif (NOT Q_WS_X11)

if(Q_WS_X11)
  SET(SOURCES_H mainwin.h 
	core/preference/profilemain.h 
	core/scanMT/scanThread.h 
	core/lookup/lookUpT.h 
	core/digSupport/digSupport.h 
	core/digSupport/digThread.h 
	core/vulnerability/addurl.h 
	core/bookmarks/addparbook.h
	core/discover/maindiscover.h
	core/discover/pingThread.h
	core/monitor/details.h
        core/monitor/monitor.h)
  #MESSAGE( STATUS ${SOURCES_H} )
else (Q_WS_X11)
  SET(SOURCES_H mainwin.h 
	core/preference/profilemain.h 
	../lib/about/about.h 
	core/scanMT/scanThread.h 
	core/lookup/lookUpT.h 
	core/digSupport/digSupport.h 
	core/digSupport/digThread.h 
	core/vulnerability/addurl.h 
	core/bookmarks/addparbook.h
	core/discover/maindiscover.h
	core/discover/pingThread.h
	core/monitor/details.h
        core/monitor/monitor.h)
  #MESSAGE( STATUS ${SOURCES_H} )
endif (Q_WS_X11)

if(Q_WS_X11)
  SET(SOURCES_UI mainwin.ui 
	core/preference/profilemain.ui 
	core/vulnerability/addurl.ui 
	core/bookmarks/addparbook.ui
	core/monitor/details.ui)
  #MESSAGE( STATUS ${SOURCES_UI} )
else (Q_WS_X11)
  SET(SOURCES_UI mainwin.ui 
	core/preference/profilemain.ui 
	../lib/about/about.ui 
	core/vulnerability/addurl.ui 
	core/bookmarks/addparbook.ui
	core/monitor/details.ui)
  #MESSAGE( STATUS ${SOURCES_UI} )
endif (Q_WS_X11)

SET(RESOURCES_QRC ../mainwin.qrc)
#MESSAGE( STATUS ${RESOURCES_QRC} )

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

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} )

if (Q_WS_X11)
  ADD_EXECUTABLE(nmapsi4 ${SOURCES_H} ${SOURCES_CPP} ${SOURCES_UI_H} ${RESOURCES_QRC_SOURCES} ${qms})
  INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR} ${CMAKE_BINARY_DIR}/lib/history/ ${CMAKE_BINARY_DIR}/lib/about/)
  TARGET_LINK_LIBRARIES( nmapsi4 ${QT_LIBRARIES} ${QT_QTWEBKIT_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${QT_QTDBUS_LIBRARY} nmapsi4_history nmapsi4_about)
elseif(WIN32)
  ADD_EXECUTABLE(nmapsi4 WIN32 ${SOURCES_H} ${SOURCES_CPP} ${SOURCES_UI_H} ${RESOURCES_QRC_SOURCES} ${qms})
  INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR})			
  TARGET_LINK_LIBRARIES( nmapsi4 ${QT_LIBRARIES} ${QT_QTWEBKIT_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${QT_QTDBUS_LIBRARY})
elseif(Q_WS_MAC)
  MESSAGE( STATUS MAC Osx)
  ADD_EXECUTABLE(nmapsi4 MACOSX_BUNDLE ${SOURCES_H} ${SOURCES_CPP} ${SOURCES_UI_H} ${RESOURCES_QRC_SOURCES} ${qms})
  INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR})
  TARGET_LINK_LIBRARIES( nmapsi4 ${QT_LIBRARIES} ${QT_QTWEBKIT_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${QT_QTDBUS_LIBRARY})
  ADD_CUSTOM_COMMAND( TARGET nmapsi4 POST_BUILD
        COMMAND mkdir ARGS ${CMAKE_CURRENT_BINARY_DIR}/nmapsi4.app/Contents/Resources
        COMMAND cp ARGS ${MACOSX_BUNDLE_ICON_FILE} ${CMAKE_CURRENT_BINARY_DIR}/nmapsi4.app/Contents/Resources
        COMMAND cp ARGS *.qm ${CMAKE_CURRENT_BINARY_DIR}/nmapsi4.app/Contents/Resources)
endif (Q_WS_X11)

#
#INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/../lib/)


if (Q_WS_X11)
  install(FILES ${qms} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/nmapsi4/locale)
  install(TARGETS nmapsi4 DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
  install(FILES "desktop/nmapsi4.desktop" DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications/kde4/)
  install(FILES "core/dbus/org.nmapsi4.Nmapsi4.xml" DESTINATION ${CMAKE_INSTALL_PREFIX}/share/dbus-1/interfaces/)
  install(FILES "desktop/nmapsi4-admin.desktop" DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications/kde4/)
  install(FILES "../icons/32x32/nmapsi4.png" DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/32x32/apps/)
  install(FILES "../icons/64x64/nmapsi4.png" DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/64x64/apps/)
  install(FILES "../icons/128x128/nmapsi4.png" DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/128x128/apps/)
endif (Q_WS_X11)

