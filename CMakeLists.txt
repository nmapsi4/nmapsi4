cmake_minimum_required(VERSION 2.6.0)

FIND_PACKAGE(Qt4 4.5.3 COMPONENTS QtCore QtGui QtNetwork QtWebKit REQUIRED)

include(${QT_USE_FILE})
ADD_DEFINITIONS(-Wall -Wextra -g)

if (Q_WS_WIN)
  include(CMakeMinGWFindMake)
endif (Q_WS_WIN)

if(Q_WS_MAC)
    SET( CMAKE_OSX_ARCHITECTURES ppc;i386 )
    SET( MACOSX_BUNDLE_ICON_FILE ${PROJECT_SOURCE_DIR}/icons/nmapsi4.icns )
endif(Q_WS_MAC)


add_subdirectory( lib )
add_subdirectory( nmapsi4 )
add_subdirectory( nmapsi4-logr )

CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/cmake_uninstall.cmake.in"
                "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake"
                IMMEDIATE @ONLY )
 
ADD_CUSTOM_TARGET( uninstall
  "${CMAKE_COMMAND}" -P "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake" )

message("------------------------------------------------")
message(" Required for build:   qt4-dev (>> 4.5.3) ")
message(" Required for runtime: nmap ( >= 5.51 ) with nping for discovery tool")
message(" Recommends for runtime: dig ")
message("------------------------------------------------")

