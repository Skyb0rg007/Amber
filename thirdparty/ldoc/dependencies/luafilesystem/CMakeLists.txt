cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(luafilesystem
    VERSION 1.7.0
    DESCRIPTION "Filesystem support for Lua"
    HOMEPAGE_URL "https://keplerproject.github.io/luafilesystem/"
    LANGUAGES C)

# Defines lua::luafilesystem

add_library(luafilesystem MODULE
    src/lfs.h
    src/lfs.c
    src/lfs.def)
target_link_libraries(luafilesystem
    PRIVATE lua::lualib)
target_compile_features(luafilesystem
    PRIVATE c_std_90)
set_target_properties(luafilesystem PROPERTIES
    PREFIX ""
    OUTPUT_NAME lfs # produces 'lfs.so' or 'lfs.dll'
    WINDOWS_EXPORT_ALL_SYMBOLS OFF) # Supplies .def file
if(C_COMPILER_ID MATCHES "(GNU|Clang|AppleClang)")
    # Enable extra warnings
    target_compile_options(luafilesystem
        PRIVATE
        -Wall -W -Waggregate-return -Wcast-align -Wmissing-prototypes
        -Wnested-externs -Wshadow -Wwrite-strings -pedantic)
endif()
add_library(lua::luafilesystem ALIAS luafilesystem)
