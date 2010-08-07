project(kdewin-single-package-installer)

set (TARGET kdewin-single-package-installer)

add_definitions(
    ${GUI_DEFINITIONS}
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${GUI_INCLUDE_DIR}
)

set (UI_SOURCES
    installerdialog.ui
)

set (SOURCES
    main.cpp
    installerdialog.cpp
    installerenginesinglepackage.cpp
    single-package-installer.rc
)

set (HEADERS
    installerdialog.h
    installerenginesinglepackage.h
)

qt4_wrap_ui(SOURCES ${UI_SOURCES})
qt4_wrap_cpp(SOURCES ${HEADERS})
qt4_add_resources(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/installerdialog.qrc)
add_executable(${TARGET} WIN32 ${SOURCES} ${HEADERS})
target_link_libraries(${TARGET} ${GUI_LIBS})

install_targets(/bin ${TARGET})

pack_target(${TARGET})
create_checksum_file(${TARGET})