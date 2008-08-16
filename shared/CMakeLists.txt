
include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(installer_shared_HEADERS
    database.h
    debug.h
    downloader.h
    downloader_p.h
    downloaderprogress.h
    externalinstallercontrol.h
    globalconfig.h
    hintfile.h
    installer.h
    installerengine.h
    installerprogress.h
    package.h
    packagecategorycache.h
    packagelist.h
    site.h
    mirrors.h
    misc.h
    registry.h
    releases.h
    settings.h
    uninstaller.h
    uninstaller_p.h
    unpacker.h
    unpacker_p.h
#    installerbase.h
    ${CMAKE_BINARY_DIR}/config.h
)

set(installer_shared_SOURCES
    database.cpp
    debug.cpp
    downloader.cpp
    downloaderprogress.cpp
    externalinstallercontrol.cpp
    globalconfig.cpp
    hintfile.cpp
    installer.cpp
    installerengine.cpp
    installerprogress.cpp
    mirrors.cpp
    misc.cpp
    package.cpp
    packagecategorycache.cpp
    packagelist.cpp
    site.cpp
    registry.cpp
    releases.cpp
    settings.cpp
    uninstaller.cpp
    unpacker.cpp
#    installerbase.cpp
)
set(installer_shared_console_SOURCES
    ${installer_shared_SOURCES}
)

set(installer_shared_gui_SOURCES
    ${installer_shared_SOURCES}
)

set (shared_sources_console ${installer_shared_console_SOURCES} ${installer_shared_HEADERS})
set (shared_sources_gui ${installer_shared_gui_SOURCES} ${installer_shared_HEADERS})

qt4_automoc(${shared_sources_console})
qt4_automoc(${shared_sources_gui})

add_library(shared-console ${shared_sources_console})
target_link_libraries(shared-console)

add_library(shared-gui ${shared_sources_gui})
set_target_properties(shared-gui PROPERTIES COMPILE_FLAGS -DUSE_GUI)
target_link_libraries(shared-gui)