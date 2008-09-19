cmake_minimum_required(VERSION 2.6.0)

FIND_PACKAGE(Qt4 REQUIRED)

include(${QT_USE_FILE})
ADD_DEFINITIONS(-O2 -Wall -Wextra)

if (Q_WS_WIN)
  include(CMakeMinGWFindMake)
endif (Q_WS_WIN)

add_subdirectory( lib )
add_subdirectory( nmapsi4 )
add_subdirectory( nmapsi4-logr )

