
#[=[
    Amber::compat - compatibility headers
    
    atomic.h - atomic primitives based on C11/pthreads/SDL2 atomics
    inttypes.h, stdbool.h, stdint.h - C99 standard headers + fallbacks
    compiler.h - detecting the current compiler
    compat.h - useful compatibility macros based on the compiler
]=]#

include(WriteCompilerDetectionHeader)
include(GNUInstallDirs)
include(CheckIncludeFile)
include(CheckSymbolExists)

write_compiler_detection_header(
    FILE ${CMAKE_CURRENT_BINARY_DIR}/include/Amber/compat/compiler.h
    PREFIX AB
    PROLOG "/* Amber::compat - compiler detection header */"
    FEATURES c_restrict # BS feature for compiler detection
    VERSION 3.14
    COMPILERS "GNU" "Clang" "AppleClang" "MSVC" "Intel")

check_include_file("inttypes.h"  HAS_INTTYPES_H)
check_include_file("stdbool.h"   HAS_STDBOOL_H)
check_include_file("stdint.h"    HAS_STDINT_H)
check_include_file("stdatomic.h" HAS_STDATOMIC_H)

set(CMAKE_REQUIRED_QUIET FALSE)
set(CMAKE_REQUIRED_DEFINITIONS "-D_GNU_SOURCE")
check_symbol_exists("getline"  "stdio.h"  HAS_GETLINE)
check_symbol_exists("asprintf" "stdio.h"  HAS_GETLINE)
check_symbol_exists("strdup"   "string.h" HAS_STRDUP)
check_symbol_exists("_strdup"  "string.h" HAS__STRDUP)

foreach(header IN ITEMS atomic.h inttypes.h stdbool.h stdint.h stdio.h string.h)
    configure_file(
        include/Amber/compat/${header}.in
        ${CMAKE_CURRENT_BINARY_DIR}/include/Amber/compat/${header}
        @ONLY)
endforeach()

add_library(compat INTERFACE)
target_include_directories(compat INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include/>)
target_compile_definitions(compat INTERFACE
    $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,"SHARED_LIBRARY">:AB_BUILDING_SHARED>)

add_library(Amber::compat ALIAS compat)

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT Development
    FILES_MATCHING PATTERN "*.h")

