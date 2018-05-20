cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)
project(qps)

option(UPDATE_TRANSLATIONS "Update source translation translations/*.ts files" OFF)

# additional cmake files
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(MAJOR_VERSION 1)
set(MINOR_VERSION 10)
set(PATCH_VERSION 18)
set(QPS_VERSION_STRING ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})
add_definitions(
    -DQPS_VERSION="${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}"
    -DQT_NO_FOREACH
)

# C++11 support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(FATAL "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. C++11 support is required")
endif()

find_package(Qt5 5.7.1 REQUIRED COMPONENTS Widgets X11Extras DBus LinguistTools)

include(GNUInstallDirs)
include(LXQtTranslateTs) # Although the name it doesn't depend on LXQt in any way
include(LXQtTranslateDesktop)
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
