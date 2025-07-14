cmake_minimum_required(VERSION 3.18.0 FATAL_ERROR)
# CMP0000: Call the cmake_minimum_required() command at the beginning of the top-level
# CMakeLists.txt file even before calling the project() command.
# The cmake_minimum_required(VERSION) command implicitly invokes the cmake_policy(VERSION)
# command to specify that the current project code is written for the given range of CMake
# versions.
Project(qps)

option(UPDATE_TRANSLATIONS "Update source translation translations/*.ts files" OFF)

set(MAJOR_VERSION 2)
set(MINOR_VERSION 11)
set(PATCH_VERSION 1)
set(QPS_VERSION_STRING ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})
add_definitions(
    -DQPS_VERSION="${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}"
)

# Minimum Versions
set(LXQTBT_MINIMUM_VERSION "2.2.0")
set(LXQT_MINIMUM_VERSION "2.2.0")
set(QT_MINIMUM_VERSION "6.6.0")

find_package(lxqt ${LXQT_MINIMUM_VERSION} REQUIRED)
find_package(Qt6 ${QT_MINIMUM_VERSION} REQUIRED COMPONENTS Widgets LinguistTools)
find_package(lxqt2-build-tools ${LXQTBT_MINIMUM_VERSION} REQUIRED)

include(GNUInstallDirs)
include(LXQtPreventInSourceBuilds)
include(LXQtCompilerSettings NO_POLICY_SCOPE)
include(LXQtTranslateTs)
include(LXQtTranslateDesktop)
include(Qt6TranslationLoader)

# LXQtCompilerSettings adds them but qps still uses implicit string casts
remove_definitions(
    -DQT_NO_CAST_FROM_ASCII
    -DQT_NO_CAST_TO_ASCII
    -DQT_NO_URL_CAST_FROM_STRING
    -DQT_NO_CAST_FROM_BYTEARRAY
)

# Must be defined after including GNUInstallDirs. Move with care.
set(QPS_TRANSLATIONS_DIR
    "${CMAKE_INSTALL_FULL_DATAROOTDIR}/${PROJECT_NAME}/translations"
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include(CheckCXXCompilerFlag)

set(AVX_SUPPORTED FALSE)
set(AVX_COMPILER_FLAG "")

# Prioritize AVX2 as it's more capable than AVX1.
# Common flags for AVX2: -march=haswell (for Intel), -mavx2 (generic)
CHECK_CXX_COMPILER_FLAG("-march=haswell" CXX_SUPPORTS_HASWELL)
CHECK_CXX_COMPILER_FLAG("-mavx2" CXX_SUPPORTS_AVX2)

if(CXX_SUPPORTS_HASWELL)
    set(AVX_SUPPORTED TRUE)
    set(AVX_COMPILER_FLAG "-march=haswell")
    message(STATUS "AVX2 (Haswell) support detected. Adding compiler flag: ${AVX_COMPILER_FLAG}")
elseif(CXX_SUPPORTS_AVX2)
    set(AVX_SUPPORTED TRUE)
    set(AVX_COMPILER_FLAG "-mavx2")
    message(STATUS "Generic AVX2 support detected. Adding compiler flag: ${AVX_COMPILER_FLAG}")
else()
    # Common flags for AVX1: -march=sandybridge (for Intel), -mavx (generic)
    CHECK_CXX_COMPILER_FLAG("-march=sandybridge" CXX_SUPPORTS_SANDYBRIDGE)
    CHECK_CXX_COMPILER_FLAG("-mavx" CXX_SUPPORTS_AVX)

    if(CXX_SUPPORTS_SANDYBRIDGE)
        set(AVX_SUPPORTED TRUE)
        set(AVX_COMPILER_FLAG "-march=sandybridge")
        message(STATUS "AVX1 (Sandy Bridge) support detected. Adding compiler flag: ${AVX_COMPILER_FLAG}")
    elseif(CXX_SUPPORTS_AVX)
        set(AVX_SUPPORTED TRUE)
        set(AVX_COMPILER_FLAG "-mavx")
        message(STATUS "Generic AVX1 support detected. Adding compiler flag: ${AVX_COMPILER_FLAG}")
    else()
        message(STATUS "Neither AVX1 nor AVX2 support detected. No specific AVX compiler flags added.")
    endif()
endif()

if(AVX_SUPPORTED)
    add_compile_options(${AVX_COMPILER_FLAG})
else()
    message(STATUS "Building without specific AVX optimizations.")
endif()

add_subdirectory(src)

# install the man page
install(FILES qps.1 DESTINATION "${CMAKE_INSTALL_FULL_DATADIR}/man/man1")

# install the app icon
install(FILES
    icon/qps.png
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/48x48/apps"
)
# install the
install(FILES org.lxqt.Qps.appdata.xml
    DESTINATION "${CMAKE_INSTALL_DATADIR}/metainfo")
