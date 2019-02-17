cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(lpeg
    VERSION 1.0.1
    DESCRIPTION ""
    HOMEPAGE_URL "http://www.inf.puc-rio.br/~roberto/lpeg/"
    LANGUAGES C)

add_library(lpeg MODULE
    lpcap.h
    lpcode.h
    lpprint.h
    lptree.h
    lptypes.h
    lpvm.h
    lpcap.c
    lpcode.c
    lpprint.c
    lptree.c
    lpvm.c)
target_compile_features(lpeg
    PRIVATE c_std_90)
target_link_libraries(lpeg
    PRIVATE lua::lualib)
add_library(lpeg::lpeg ALIAS lpeg)
