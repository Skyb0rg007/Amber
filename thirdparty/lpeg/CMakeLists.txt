cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(lpeg
    VERSION 1.0.1
    DESCRIPTION "Pattern-matching library for Lua, based on Parsing Expression Grammars"
    HOMEPAGE_URL "http://www.inf.puc-rio.br/~roberto/lpeg/"
    LANGUAGES C)

add_library(lpeg
    src/lpcap.h
    src/lpcode.h
    src/lpprint.h
    src/lptree.h
    src/lptypes.h
    src/lpvm.h
    src/lpcap.c
    src/lpcode.c
    src/lpprint.c
    src/lptree.c
    src/lpvm.c)
target_compile_features(lpeg
    PRIVATE c_std_99)
target_link_libraries(lpeg
    PRIVATE lua::lualib)
add_library(lpeg::lpeg ALIAS lpeg)
