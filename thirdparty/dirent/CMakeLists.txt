cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(dirent
    VERSION 1.23.2
    LANGUAGES C
    HOMEPAGE_URL "https://github.com/tronkko/dirent"
    DESCRIPTION "POSIX compatible Dirent interface for Microsoft Windows")

include(CheckIncludeFile)

add_library(dirent INTERFACE)
check_include_file("dirent.h" DIRENT_H_FOUND)

if(WIN32 AND NOT DIRENT_H_FOUND)
    target_include_directories(dirent
        INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
    install(DIRECTORY include/
        COMPONENTS Development
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
elseif(NOT DIRENT_H_FOUND)
    message(FATAL_ERROR "Could not find dirent.h!")
endif()

add_library(dirent::dirent ALIAS dirent)
