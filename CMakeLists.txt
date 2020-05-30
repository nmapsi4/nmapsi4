PROJECT( nmapsi4 )

set(VERSION_MAJOR 0)
set(VERSION_MINOR 4)
set(VERSION_PATCH 81)
set(VERSION  ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

cmake_minimum_required(VERSION 2.8.2)

if (NOT WIN32 AND NOT APPLE)
    FIND_PACKAGE(Qt5 5.7.0 COMPONENTS
	Core 
	Widgets 
	Network 
	WebEngineWidgets
	DBus 
	Script 
	LinguistTools 
	Quick
	Qml
	Gui
	REQUIRED)
else (NOT WIN32 AND NOT APPLE)
    FIND_PACKAGE(Qt5 5.7.0
	COMPONENTS 
	Core 
	Widgets 
	Network 
	WebEngineWidgets
	Script 
	LinguistTools 
	Quick
	Qml
	Gui
	REQUIRED)
endif (NOT WIN32 AND NOT APPLE)

add_definitions (${QT5_DEFINITIONS})
include_directories(${QT5_USE_FILE})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config-nmapsi4.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-nmapsi4.h )

add_definitions(-O2 -Wall -Wextra -g -DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS -DQT_NO_URL_CAST_FROM_STRING -DQT_NO_FOREACH)

if (WIN32)
    include(CMakeMinGWFindMake)
endif (WIN32)

if(APPLE)
    SET( CMAKE_OSX_ARCHITECTURES x86_64 )
    SET( MACOSX_BUNDLE_ICON_FILE ${PROJECT_SOURCE_DIR}/icons/nmapsi4.icns )
    set( MACOSX_BUNDLE_BUNDLE_NAME nmapsi4 )
    set( MACOSX_BUNDLE_GUI_IDENTIFIER "nmapsi4" )
    set( MACOSX_BUNDLE_INFO_STRING "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}" ","
                                    " Copyright 2013 Francesco Cecconi" )
    set( MACOSX_BUNDLE_SHORT_VERSION_STRING "${VERSION_MAJOR}.${VERSION_MINOR}" )
    set( MACOSX_BUNDLE_LONG_VERSION_STRING "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}" )
    set( MACOSX_BUNDLE_BUNDLE_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}" )
    set( MACOSX_BUNDLE_COPYRIGHT "(C) 2013 Francesco Cecconi" )
endif(APPLE)

CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/cmake_uninstall.cmake.in"
                "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake"
                IMMEDIATE @ONLY )

message("-------------------------------------------------------------------")
message(" Required for build:   qt5-dev (>>= 5.7.0) ")
message(" Required for runtime: nmap+nping ( >= 6.00 ), dig ")
message("-------------------------------------------------------------------")

add_subdirectory( src )

