cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(lua
    VERSION 5.3.5
    DESCRIPTION
        "Lua is a powerful, fast, lightweight, embeddable scripting language"
    HOMEPAGE_URL "www.lua.org"
    LANGUAGES C)

include(GNUInstallDirs)
include(CheckLibraryExists)

find_package(Readline MODULE) # Optional readline support for the interpreter

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

# Dynamic library
add_library(lualib SHARED
    ${LUA_SOURCES})
target_include_directories(lualib
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/>)
target_compile_features(lualib
    PRIVATE c_std_90)
add_library(lua::lualib ALIAS lualib)

# Static library
add_library(lualib-static STATIC
    ${LUA_SOURCES})
target_include_directories(lualib-static
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/>)
target_compile_features(lualib-static
    PRIVATE c_std_90)
add_library(lua::lualib-static ALIAS lualib-static)

# Do we need to link to libm?
check_library_exists("m" "pow" "" HAVE_LIBM)
if(HAVE_LIBM)
    target_link_libraries(lualib PRIVATE m)
    target_link_libraries(lualib-static PRIVATE m)
endif()

# Lua interpreter
add_executable(lua
    src/lua.c)
target_link_libraries(lua
    PRIVATE lualib)
add_executable(lua::lua ALIAS lua)

# Lua compiler
add_executable(luac
    src/luac.c)
target_link_libraries(luac
    PRIVATE lualib-static)
add_executable(lua::luac ALIAS luac)

# Readline support(?)
if(READLINE_FOUND)
    target_compile_definitions(lualib PUBLIC LUA_USE_READLINE)
    target_compile_definitions(lualib-static PUBLIC LUA_USE_READLINE)
    target_link_libraries(lua PRIVATE readline::readline)
endif()

# Enable extra assertions for debug builds
target_compile_definitions(lualib        PRIVATE $<$<CONFIG:Debug>:LUA_USE_APICHECK>)
target_compile_definitions(lualib-static PRIVATE $<$<CONFIG:Debug>:LUA_USE_APICHECK>)

# System-config
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_compile_definitions(lualib PUBLIC LUA_USE_POSIX LUA_USE_DLOPEN)
    target_link_libraries(lualib PRIVATE ${CMAKE_DL_LIBS})
    target_compile_definitions(lualib-static PUBLIC LUA_USE_POSIX LUA_USE_DLOPEN)
    target_link_libraries(lualib-static PRIVATE ${CMAKE_DL_LIBS})
    target_link_options(lua PRIVATE "LINKER:-E")
    target_link_options(luac PRIVATE "LINKER:-E")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_compile_definitions(lualib PUBLIC LUA_USE_POSIX LUA_USE_DLOPEN)
    target_link_libraries(lualib PRIVATE ${CMAKE_DL_LIBS})
    target_compile_definitions(lualib-static PUBLIC LUA_USE_POSIX LUA_USE_DLOPEN)
    target_link_libraries(lualib-static PRIVATE ${CMAKE_DL_LIBS})
elseif(WIN32)
    target_compile_definitions(lualib PUBLIC LUA_USE_WINDOWS)
    target_compile_definitions(lualib PRIVATE LUA_BUILD_AS_DLL)
    target_compile_definitions(lualib-static PUBLIC LUA_USE_WINDOWS)
elseif(CMAKE_SYSTEM_NAME STREQUAL "SunOS")
    target_compile_definitions(lualib PUBLIC LUA_USE POSIX LUA_USE_DLOPEN _REENTRANT)
    target_link_libraries(lualib PRIVATE ${CMAKE_DL_LIBS})
    target_compile_definitions(lualib-static PUBLIC LUA_USE POSIX LUA_USE_DLOPEN _REENTRANT)
    target_link_libraries(lualib-static PRIVATE ${CMAKE_DL_LIBS})
elseif(CMAKE_SYSTEM_NAME STREQUAL "AIX")
    target_compile_definitions(lualib PUBLIC LUA_USE_POSIX LUA_USE_DLOPEN)
    target_compile_options(lualib PRIVATE -O2)
    target_link_libraries(lualib PRIVATE ${CMAKE_DL_LIBS})
    target_link_options(lualib PRIVATE -brtl -bexpall)
    target_compile_definitions(lualib-static PUBLIC LUA_USE_POSIX LUA_USE_DLOPEN)
    target_compile_options(lualib-static PRIVATE -O2)
    target_link_libraries(lualib-static PRIVATE ${CMAKE_DL_LIBS})
    target_link_options(lualib-static PRIVATE -brtl -bexpall)
elseif(UNIX AND CMAKE_SYSTEM_NAME MATCHES "*BSD*")
    target_compile_definitions(lualib PUBLIC LUA_USE_POSIX LUA_USE_DLOPEN)
    target_compile_definitions(lualib-static PUBLIC LUA_USE_POSIX LUA_USE_DLOPEN)
    target_link_options(lua PRIVATE "LINKER:-E")
    target_link_options(luac PRIVATE "LINKER:-E")
endif()

install(TARGETS lualib lualib-static
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Runtime NAMELINK_COMPONENT Development
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES src/lua.h src/luaconf.h src/lualib.h src/lauxlib.h lua.hpp
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT Development)
install(TARGETS lua luac
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Development)

# Testing
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
