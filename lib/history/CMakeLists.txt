PROJECT(nmapsi4_history)

include_directories(${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${QT_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/src/common/
)

SET( SOURCES_CPP coreHistory.cpp loghistory.cpp)

ADD_LIBRARY( nmapsi4_history STATIC ${SOURCES_CPP})



