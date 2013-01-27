PROJECT( nmapsi4 )

set(VERSION_MAJOR 0)
set(VERSION_MINOR 4)
set(VERSION_PATCH 65)
set(VERSION  ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})


cmake_minimum_required(VERSION 2.6.0)

if (NOT WIN32 AND NOT APPLE)
    FIND_PACKAGE(Qt4 4.7 COMPONENTS QtCore QtGui QtNetwork QtWebKit QtDBus QtDeclarative REQUIRED)
else (NOT WIN32 AND NOT APPLE)
    FIND_PACKAGE(Qt4 4.7 COMPONENTS QtCore QtGui QtNetwork QtWebKit QtDeclarative REQUIRED)
endif (NOT WIN32 AND NOT APPLE)

OPTION(BUILD_KDELIBS "Enable building with kdelibs support." ON)

if (BUILD_KDELIBS)
    find_package(KDE4)
endif (BUILD_KDELIBS)

if (BUILD_KDELIBS)
    if(KDE4_FOUND)
        add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
        include(${QT_USE_FILE})
        include(KDE4Defaults)
        set(USE_KDELIBS 1)
    else(KDE4_FOUND)
        add_definitions (${QT_DEFINITIONS})
        include(${QT_USE_FILE})
    endif(KDE4_FOUND)
endif (BUILD_KDELIBS)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config-nmapsi4.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-nmapsi4.h )

ADD_DEFINITIONS(-O2 -Wall -Wextra -g -DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)

if (WIN32)
    include(CMakeMinGWFindMake)
endif (WIN32)

if(APPLE)
    SET( CMAKE_OSX_ARCHITECTURES i386;x86_64 )
    SET( MACOSX_BUNDLE_ICON_FILE ${PROJECT_SOURCE_DIR}/icons/nmapsi4.icns )
endif(APPLE)

CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/cmake_uninstall.cmake.in"
                "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake"
                IMMEDIATE @ONLY )

ADD_CUSTOM_TARGET( uninstall
  "${CMAKE_COMMAND}" -P "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake" )

message("-------------------------------------------------------------------")
message(" Required for build:   qt4-dev (>>= 4.7) ")
message(" Required for runtime: nmap+nping ( >= 6.00 ), dig ")

if (BUILD_KDELIBS)
    if(KDE4_FOUND)
        message(" OPTIONAL: Kdelibs-dev founded - build with kde support ")
    else(KDE4_FOUND)
        message(" OPTIONAL: Kdelibs-dev not founded - build without kde support ")
    endif(KDE4_FOUND)
endif (BUILD_KDELIBS)

message("-------------------------------------------------------------------")

add_subdirectory( src )

