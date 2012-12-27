PROJECT( nmapsi4 )

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config-nmapsi4.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-nmapsi4.h )

cmake_minimum_required(VERSION 2.6.0)

if (NOT WIN32 AND NOT APPLE)
    FIND_PACKAGE(Qt4 4.6 COMPONENTS QtCore QtGui QtNetwork QtWebKit QtDBus REQUIRED)
else (NOT WIN32 AND NOT APPLE)
    FIND_PACKAGE(Qt4 4.6 COMPONENTS QtCore QtGui QtNetwork QtWebKit REQUIRED)
endif (NOT WIN32 AND NOT APPLE)

find_package(KDE4)

if(KDE4_FOUND)
    add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
    include(${QT_USE_FILE})
    include(KDE4Defaults)
    set(USE_KDELIBS 1)
else(KDE4_FOUND)
    add_definitions (${QT_DEFINITIONS})
    include(${QT_USE_FILE})
endif(KDE4_FOUND)

ADD_DEFINITIONS(-O2 -Wall -Wextra -g -DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)

if (Q_WS_WIN)
    include(CMakeMinGWFindMake)
endif (Q_WS_WIN)

if(Q_WS_MAC)
    SET( CMAKE_OSX_ARCHITECTURES i386;x86_64 )
    SET( MACOSX_BUNDLE_ICON_FILE ${PROJECT_SOURCE_DIR}/icons/nmapsi4.icns )
endif(Q_WS_MAC)

CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/cmake_uninstall.cmake.in"
                "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake"
                IMMEDIATE @ONLY )

ADD_CUSTOM_TARGET( uninstall
  "${CMAKE_COMMAND}" -P "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake" )

message("-------------------------------------------------------------------")
message(" Required for build:   qt4-dev (>>= 4.6) ")
message(" Required for runtime: nmap+nping ( >= 6.00 ), dig ")

if(KDE4_FOUND)
    message(" OPTIONAL: Kdelibs-dev founded - build with kde support ")
else(KDE4_FOUND)
    message(" OPTIONAL: Kdelibs-dev not founded - build without kde support ")
endif(KDE4_FOUND)

message("-------------------------------------------------------------------")

add_subdirectory( src )