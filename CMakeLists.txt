cmake_minimum_required(VERSION 2.6.0)

FIND_PACKAGE(Qt4 REQUIRED)
include(${QT_USE_FILE})
ADD_DEFINITIONS(-O2 -Wall -Wextra)

if (Q_WS_WIN)
  include(CMakeMinGWFindMake)
endif (Q_WS_WIN)

if ( EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/lib )
  add_subdirectory( lib )
endif ( EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/lib )

if ( EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/nmapsi4 )
  add_subdirectory( nmapsi4 )
endif ( EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/nmapsi4 )

if ( EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/nmapsi4-logr )
  add_subdirectory( nmapsi4-logr )
endif ( EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/nmapsi4-logr )

