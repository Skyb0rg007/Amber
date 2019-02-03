cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(penlight
    VERSION 1.5.4
    DESCRIPTION "Generally useful pure Lua modules, inspired by Python stdlib"
    HOMEPAGE_URL "https://github.com/stevedonovan/Penlight"
    LANGUAGES NONE)

list(APPEND PENLIGHT_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/?/init.lua"
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/?.lua")
set(LUA_PENLIGHT_PATH "${PENLIGHT_PATH}" CACHE INTERNAL
    "LUA_PATH to find penlight")
