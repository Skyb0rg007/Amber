cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(lua
    VERSION 5.3.5
    DESCRIPTION
        "Lua is a powerful, fast, lightweight, embeddable scripting language"
    HOMEPAGE_URL "www.lua.org"
    LANGUAGES C)

include(GNUInstallDirs)
include(CheckLibraryExists)

set(LUA_SOURCES
    # Core
    src/lapi.c
    src/lcode.c
    src/lctype.c
    src/ldebug.c
    src/ldo.c
    src/ldump.c
    src/lfunc.c
    src/lgc.c
    src/llex.c
    src/lmem.c
    src/lobject.c
    src/lopcodes.c
    src/lparser.c
    src/lstate.c
    src/lstring.c
    src/ltable.c
    src/ltm.c
    src/lundump.c
    src/lvm.c
    src/lzio.c
    # Aux
    src/lauxlib.c
    src/lbaselib.c
    src/lbitlib.c
    src/lcorolib.c
    src/ldblib.c
    src/linit.c
    src/liolib.c
    src/lmathlib.c
    src/loadlib.c
    src/loslib.c
    src/lstrlib.c
    src/ltablib.c
    src/lutf8lib.c)

add_library(lualib
    ${LUA_SOURCES})
target_include_directories(lualib
    PRIVATE src/
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
target_compile_features(lualib
    PUBLIC c_std_99) # Public since `long long` is used in headers
target_link_libraries(lualib 
    PRIVATE ${CMAKE_DL_LIBS})
add_library(lua::lualib ALIAS lualib)

# Do we need to link to libm?
check_library_exists("m" "pow" "" HAVE_LIBM)
if(HAVE_LIBM)
    target_link_libraries(lualib PRIVATE m)
endif()

# Enable extra assertions for debug builds
target_compile_definitions(lualib
    PRIVATE $<$<CONFIG:Debug>:LUA_USE_APICHECK>)

# System configuration
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_compile_definitions(lualib PUBLIC LUA_USE_POSIX LUA_USE_DLOPEN)
elseif(APPLE)
    target_compile_definitions(lualib PUBLIC LUA_USE_POSIX LUA_USE_DLOPEN)
elseif(WIN32)
    target_compile_definitions(lualib PUBLIC LUA_USE_WINDOWS)
    target_compile_definitions(lualib PRIVATE LUA_BUILD_AS_DLL)
elseif(CMAKE_SYSTEM_NAME STREQUAL "SunOS")
    target_compile_definitions(lualib PUBLIC LUA_USE POSIX LUA_USE_DLOPEN _REENTRANT)
elseif(CMAKE_SYSTEM_NAME STREQUAL "AIX")
    target_compile_definitions(lualib PUBLIC LUA_USE_POSIX LUA_USE_DLOPEN)
    target_link_options(lualib PRIVATE -brtl -bexpall)
elseif(UNIX AND CMAKE_SYSTEM_NAME MATCHES "BSD")
    target_compile_definitions(lualib PUBLIC LUA_USE_POSIX LUA_USE_DLOPEN)
endif()

install(TARGETS lualib
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Runtime NAMELINK_COMPONENT Development
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY include
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT Development)

