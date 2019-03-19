cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(sqlite3
    VERSION 3.26.0
    DESCRIPTION
        "SQLite is a C-language library that implements a small, fast, \
        self-contained, high-reliability, full-featured, SQL database engine."
    HOMEPAGE_URL "https://www.sqlite.org"
    LANGUAGES C)

include(GNUInstallDirs)

# This CMakeLists.txt defines:
#  sqlite3::sqlite3 - the sqlite3 shared/static library, installed with Runtime
#  sqlite3::shell - the sqlite3 shell, installed with Development

# Options
option(SQLITE3_DEBUG    "Build with SQLite3 debug features" OFF)
option(SQLITE3_MEMDEBUG "Build with SQLite3 memory debug features" OFF)
option(SQLITE3_RTREE    "Build R*Tree index extension" OFF)

# Threading is needed for the sqlite3 shell
set(CMAKE_THREAD_PREFER_PTHREAD ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Build the shared/static sqlite3 library
add_library(sqlite3
    src/sqlite3.c)
target_include_directories(sqlite3
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
target_compile_features(sqlite3
    PRIVATE c_std_90)
set_target_properties(sqlite3 PROPERTIES
    SOVERSION "${PROJECT_VERSION}"
    VERSION "${PROJECT_VERSION}"
    WINDOWS_EXPORT_ALL_SYMBOLS OFF)
target_compile_definitions(sqlite3
    PRIVATE
        $<$<BOOL:${SQLITE3_DEBUG}>:SQLITE_DEBUG>
        $<$<BOOL:${SQLITE3_MEMDEBUG}>:SQLITE_MEMDEBUG>
        $<$<BOOL:${SQLITE3_RTREE}>:SQLITE_ENABLE_RTREE>)
add_library(sqlite3::sqlite3 ALIAS sqlite3)

# Ensure things are properly dllimported/dllexported
get_target_property(sqlite_lib sqlite3 TYPE)
if(WIN32 AND sqlite_lib STREQUAL "SHARED_LIBRARY")
    target_compile_definitions(sqlite3
        PRIVATE "SQLITE_API=__declspec(dllexport)"
        INTERFACE "SQLITE_API=__declspec(dllimport)")
endif()

# The sqlite3 shell
add_executable(sqlite3-shell
    src/shell.c)
target_link_libraries(sqlite3-shell
    PRIVATE sqlite3::sqlite3 Threads::Threads ${CMAKE_DL_LIBS})
set_target_properties(sqlite3-shell PROPERTIES
    RUNTIME_OUTPUT_NAME "sqlite3")
add_executable(sqlite3::shell ALIAS sqlite3-shell)

# Installation
install(TARGETS sqlite3
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Runtime NAMELINK_COMPONENT Development
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT Development
    FILES_MATCHING PATTERN "*.h")
install(TARGETS sqlite3-shell
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Development)

