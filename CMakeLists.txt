project(qps)
# CMP0000 - A minimum required CMake version must be specified.
# Note that the command invocation must appear in the CMakeLists.txt
# file itself; a call in an included file is not sufficient.
cmake_minimum_required(VERSION 3.6.2 FATAL_ERROR)

option(UPDATE_TRANSLATIONS "Update source translation translations/*.ts files" OFF)

# additional cmake files
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(MAJOR_VERSION 1)
set(MINOR_VERSION 10)
set(PATCH_VERSION 18)
set(QPS_VERSION_STRING ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})
add_definitions(
    -DQPS_VERSION="${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}"
)

find_package(Qt5 5.7.1 REQUIRED COMPONENTS Widgets X11Extras DBus LinguistTools)
find_package(lxqt-build-tools)

include(GNUInstallDirs)
include(LXQtTranslateTs) # Although the name it doesn't depend on LXQt in any way
include(LXQtTranslateDesktop)
include(LXQtCompilerSettings)
include(Qt5TranslationLoader)

# Must be defined after including GNUInstallDirs. Move with care.
set(QPS_TRANSLATIONS_DIR
    "${CMAKE_INSTALL_FULL_DATAROOTDIR}/${PROJECT_NAME}/translations"
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_subdirectory(src)

# install the man page
install(FILES qps.1 DESTINATION "${CMAKE_INSTALL_FULL_DATADIR}/man/man1")

# install the app icon
install(FILES
    icon/qps.png
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/48x48/apps"
)
