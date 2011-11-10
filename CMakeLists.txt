cmake_minimum_required(VERSION 2.6.0)

FIND_PACKAGE(Qt4 4.5.3 COMPONENTS QtCore QtGui QtNetwork QtWebKit QtDBus REQUIRED)

include(${QT_USE_FILE})

if (Q_WS_X11)
	ADD_DEFINITIONS(-O2 -Wall -Wextra -g -fno-var-tracking-assignments -DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
else (Q_WS_X11)
	ADD_DEFINITIONS(-O2 -Wall -Wextra -g -DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
endif (Q_WS_X11)

if (Q_WS_WIN)
  include(CMakeMinGWFindMake)
endif (Q_WS_WIN)

if(Q_WS_MAC)
    SET( CMAKE_OSX_ARCHITECTURES i386;x86_64 )
    SET( MACOSX_BUNDLE_ICON_FILE ${PROJECT_SOURCE_DIR}/icons/nmapsi4.icns )
endif(Q_WS_MAC)

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR})


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

