cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(klib
    HOMEPAGE_URL "https://github.com/attractivechaos/klib"
    DESCRIPTION "a standalone and lightweight C library")

add_library(khash INTERFACE)
target_include_directories(khash
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
target_compile_features(khash
    INTERFACE c_std_90)

add_library(klib::khash ALIAS khash)

add_library(kvec INTERFACE)
target_include_directories(kvec
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
target_compile_features(kvec
    INTERFACE c_std_90)

add_library(klib::kvec ALIAS kvec)
