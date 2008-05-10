PROJECT(nmapsi4)
cmake_minimum_required(VERSION 2.4.0)

FIND_PACKAGE(Qt4 REQUIRED)
include(${QT_USE_FILE})

configure_file(config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h )

SET(SOURCES_CPP src/core/nmapsi4Slot.cpp
		src/core/resizeNmapsi4.cpp
		src/core/nmapParser.cpp 
		src/core/extensions.cpp 
		src/mainwin.cpp 
		src/core/updateUI.cpp 
		src/core/toolsUI.cpp
		src/preference/profilemain.cpp
		src/preference/profilemainupdate.cpp
		src/core/profileSession.cpp
		src/core/saveLog.cpp
		src/main.cpp )
MESSAGE( STATUS ${SOURCES_CPP} )

SET(SOURCES_H src/mainwin.h src/preference/profilemain.h)
MESSAGE( STATUS ${SOURCES_H} )

SET(SOURCES_UI src/mainwin.ui src/preference/profilemain.ui)
MESSAGE( STATUS ${SOURCES_UI} )

SET(RESOURCES_QRC mainwin.qrc)
MESSAGE( STATUS ${RESOURCES_QRC} )

qt4_add_RESOURCES(RESOURCES_QRC_SOURCES  ${RESOURCES_QRC} )
QT4_WRAP_UI(SOURCES_UI_H ${SOURCES_UI})
QT4_WRAP_CPP( SOURCES_CPP ${SOURCES_H} )

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} )

ADD_EXECUTABLE(nmapsi4 ${SOURCES_H} ${SOURCES_CPP} ${SOURCES_UI_H} ${RESOURCES_QRC_SOURCES})

INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR} )

TARGET_LINK_LIBRARIES( nmapsi4 ${QT_LIBRARIES} )

#
# Check lupdate and lrelease
# Cmake < 2.5
#
set(po_SRCS ts/nmapsi4_it.ts ts/nmapsi4_fr.ts ts/nmapsi4_es.ts ts/nmapsi4_de.ts ts/nmapsi4_pl.ts)

find_program(QT_LUPDATE
  NAMES lupdate-qt4 lupdate
  PATHS $ENV{QTDIR}/bin
  DOC "Path to the lupdate program"
)

find_program(QT_LRELEASE
  NAMES lrelease-qt4 lrelease
  PATHS $ENV{QTDIR}/bin
  DOC "Path to the lrelease program"
)

# Update translations (note that [clean-]merge targets write to source directory)
if (QT_LUPDATE)
  # Updates all translations
  add_custom_target(merge
    ${QT_LUPDATE} ${translate_files} -ts ${po_SRCS}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

  # Update all translations and remove obsolete strings
  add_custom_target(clean-merge
    ${QT_LUPDATE} -noobsolete ${translate_files} -ts ${po_SRCS}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

  # Create template.ts to be used for new translations
  add_custom_command(OUTPUT template.ts
    COMMAND ${QT_LUPDATE} -noobsolete ${translate_files} -ts template.ts
    DEPENDS ${translate_files})
  add_custom_target(template DEPENDS template.ts)
else (QT_LUPDATE)
  message(STATUS "lupdate could not be found. You will not be able to update the translations.")
endif (QT_LUPDATE)


# Build translations
if (QT_LRELEASE)
  # Clear list
  set(qms)

  # Add command to build X.qm from X.po
  foreach (po ${po_SRCS})
    get_filename_component(qm ${po} NAME_WE)
    set(qm "${CMAKE_CURRENT_BINARY_DIR}/${qm}.qm")
    set(po "${CMAKE_CURRENT_SOURCE_DIR}/${po}")
    add_custom_command(OUTPUT ${qm}
      COMMAND ${QT_LRELEASE} ${po} -qm ${qm}
      DEPENDS ${po})
    set(qms ${qms} ${qm})
  endforeach(po)

  # Finish off with a command that builds *.qm
  add_custom_target(translations ALL DEPENDS ${qms})
  install(FILES ${qms} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/nmapsi4/locale)

else (QT_LRELEASE)
  message(STATUS "lrelease could not be found. You're losing the localisation.")
endif (QT_LRELEASE)

# Cmake >= 2.5
#qt4_create_translation(qms ts/nmapsi4_it.ts ts/nmapsi4_fr.ts ts/nmapsi4_es.ts nmapsi4_de.ts ts/nmapsi4_pl.ts)
#qt4_add_translation(qms ts/nmapsi4_it.ts ts/nmapsi4_fr.ts ts/nmapsi4_es.ts nmapsi4_de.ts ts/nmapsi4_pl.ts)

  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/nmapsi4" DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
  install(FILES "src/kde/nmapsi4.desktop" DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications/kde/)
  install(FILES "src/kde/nmapsi4-admin.desktop" DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications/kde/)
  install(FILES "icons/16x16/nmapsi4.png" DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/16x16/apps/)
  install(FILES "icons/32x32/nmapsi4.png" DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/32x32/apps/)
  install(FILES "icons/64x64/nmapsi4.png" DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/64x64/apps/)
  install(FILES "icons/128x128/nmapsi4.png" DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/128x128/apps/)

