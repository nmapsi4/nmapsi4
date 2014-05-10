

include_directories(${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/src/platform/
    ${CMAKE_SOURCE_DIR}/src/platform/logwriter/
    ${CMAKE_SOURCE_DIR}/src/platform/history/
    ${CMAKE_SOURCE_DIR}/src/platform/about/
    ${CMAKE_SOURCE_DIR}/src/platform/monitor/
    ${CMAKE_SOURCE_DIR}/src/platform/parser/
    ${CMAKE_SOURCE_DIR}/src/common/
    ${CMAKE_SOURCE_DIR}/src/app/
    ${CMAKE_SOURCE_DIR}/src/app/preference/
    ${CMAKE_SOURCE_DIR}/src/app/profiler/
    )


if (NOT WIN32 AND NOT APPLE)
QT5_ADD_DBUS_ADAPTOR(nmapsi4adaptor_SRCS
    dbus/org.nmapsi4.Nmapsi4.xml
    src/platform/monitor/monitor.h
    Monitor
    )
endif (NOT WIN32 AND NOT APPLE)

SET(SOURCES_CPP
    platform/selectprofiledialog.cpp
    platform/addvulnerabilityurl.cpp
    platform/vulnerability.cpp
    platform/lookupmanager.cpp
    platform/digmanager.cpp
    platform/nsemanager.cpp
    platform/discover.cpp
    platform/discovermanager.cpp
    platform/addparameterstobookmark.cpp
    platform/logwriter/logwriter.cpp
    platform/logwriter/logwriterxml.cpp
    platform/history/history.cpp
    platform/about/about.cpp
    platform/monitor/monitor.cpp
    platform/monitor/monitorhostscandetails.cpp
    platform/parser/parsermanager.cpp
    common/utilities.cpp
    common/pushbuttonorientated.cpp
    common/processthread.cpp
    common/pobjects.cpp
    common/notify.cpp
    common/package.cpp
    common/mouseeventfilter.cpp
    app/preference/preferencesdialog.cpp
    app/profiler/profilermanager.cpp
    app/profiler/profiler.cpp
    app/profilehandler.cpp
    app/mainwindow.cpp
    app/bookmarkmanager.cpp
    app/actionmanager.cpp
    app/qmlwelcome.cpp
    app/main.cpp
    )


if (NOT WIN32 AND NOT APPLE)
    SET(SOURCES_CPP ${SOURCES_CPP} ${nmapsi4adaptor_SRCS})
endif (NOT WIN32 AND NOT APPLE)

SET(SOURCES_MOC
    platform/lookupmanager.h
    platform/digmanager.h
    platform/addvulnerabilityurl.h
    platform/vulnerability.h
    platform/addparameterstobookmark.h
    platform/discover.h
    platform/discovermanager.h
    platform/nsemanager.h
    platform/selectprofiledialog.h
    platform/monitor/monitorhostscandetails.h
    platform/monitor/monitor.h
    platform/parser/parsermanager.h
    common/utilities.h
    common/processthread.h
    common/mouseeventfilter.h
    app/profiler/profilermanager.h
    app/profiler/profiler.h
    app/preference/preferencesdialog.h
    app/bookmarkmanager.h
    app/actionmanager.h
    app/profilehandler.h
    app/mainwindow.h
    app/qmlwelcome.h
    )

SET(SOURCES_UI
    platform/ui/addvulnerabilityurl.ui
    platform/ui/addparameterstobookmark.ui
    platform/ui/about.ui
    platform/ui/monitorhostscandetails.ui
    platform/ui/selectprofiledialog.ui
    app/ui/preferencesdialog.ui
    app/ui/profilerdialog.ui
    app/ui/discoverwidget.ui
    app/ui/scanbookmarkwidget.ui
    app/ui/vulnbookmarkwidget.ui
    app/ui/vulnerabilitywidget.ui
    app/ui/monitorwidget.ui
    app/ui/scanwidget.ui
    )

SET(RESOURCES_QRC
    images.qrc
    qml.qrc
    )

qt5_add_resources(RESOURCES_QRC_SOURCES  ${RESOURCES_QRC} )
qt5_wrap_ui(SOURCES_UI_H ${SOURCES_UI})
qt5_wrap_cpp(SOURCES_MOC_H ${SOURCES_MOC} )

SET(TRANSLATIONS_FILES
    ts/nmapsi4_it.ts
    ts/nmapsi4_fr.ts 
    ts/nmapsi4_es.ts 
    ts/nmapsi4_de.ts 
    ts/nmapsi4_cs.ts
    ts/nmapsi4_ru.ts 
    ts/nmapsi4_ca.ts
    )

qt5_add_translation(qms ${TRANSLATIONS_FILES})

INCLUDE_DIRECTORIES( ${QT5_INCLUDE_DIR} )

if (NOT WIN32 AND NOT APPLE)
    ADD_EXECUTABLE(nmapsi4 ${SOURCES_MOC} ${SOURCES_CPP} ${SOURCES_MOC_H} ${SOURCES_UI_H} ${RESOURCES_QRC_SOURCES} ${qms})
elseif(WIN32)
    ADD_EXECUTABLE(nmapsi4 WIN32 ${SOURCES_MOC} ${SOURCES_CPP} ${SOURCES_MOC_H} ${SOURCES_UI_H} ${RESOURCES_QRC_SOURCES} ${qms})
elseif(APPLE)
    MESSAGE( STATUS MAC Osx)
    ADD_EXECUTABLE(nmapsi4 MACOSX_BUNDLE ${SOURCES_MOC} ${SOURCES_CPP} ${SOURCES_MOC_H} ${SOURCES_UI_H} ${RESOURCES_QRC_SOURCES} ${qms})
    ADD_CUSTOM_COMMAND(TARGET nmapsi4 POST_BUILD
        COMMAND mkdir -p ARGS ${CMAKE_CURRENT_BINARY_DIR}/nmapsi4.app/Contents/Resources
        COMMAND mkdir -p ARGS ${CMAKE_CURRENT_BINARY_DIR}/nmapsi4.app/Contents/MacOS
        COMMAND cp ARGS ${MACOSX_BUNDLE_ICON_FILE} ${CMAKE_CURRENT_BINARY_DIR}/nmapsi4.app/Contents/Resources
        COMMAND cp ARGS *.qm ${CMAKE_CURRENT_BINARY_DIR}/nmapsi4.app/Contents/Resources)
endif (NOT WIN32 AND NOT APPLE)

target_link_libraries(nmapsi4
    ${QT5_LIBRARIES}
    ${QT5_QTWEBKIT_LIBRARY}
    ${QT5_QTNETWORK_LIBRARY}
    Qt5::Widgets
    Qt5::WebKitWidgets
    Qt5::Declarative
    Qt5::DBus
    )

if (NOT WIN32 AND NOT APPLE)
    target_link_libraries(nmapsi4
        ${QT5_QTDBUS_LIBRARY}
        )
endif (NOT WIN32 AND NOT APPLE)

if (NOT WIN32 AND NOT APPLE)
    install(TARGETS nmapsi4 DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
    install(FILES ${qms} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/nmapsi4/locale)
    install(FILES "desktop/nmapsi4.desktop" DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications/kde4/)
    install(FILES "desktop/nmapsi4-admin.desktop" DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications/kde4/)
    install(FILES "dbus/org.nmapsi4.Nmapsi4.xml" DESTINATION ${CMAKE_INSTALL_PREFIX}/share/dbus-1/interfaces/)
    install(FILES "../icons/32x32/nmapsi4.png" DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/32x32/apps/)
    install(FILES "../icons/64x64/nmapsi4.png" DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/64x64/apps/)
    install(FILES "../icons/128x128/nmapsi4.png" DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/128x128/apps/)
endif (NOT WIN32 AND NOT APPLE)

