include(CheckSymbolExists)
include(CheckFunctionExists)
include(WriteCompilerDetectionHeader)

# Detect the compiler being used
write_compiler_detection_header(
    FILE ${CMAKE_CURRENT_BINARY_DIR}/include/Amber/util/AB_compiler_detection.h
    PREFIX AB
    PROLOG "/* Amber::util - compiler detection header */"
    FEATURES c_restrict
    COMPILERS GNU Clang AppleClang MSVC Intel)

# Detect the availability of functions that may be in libc
set(CMAKE_REQUIRED_QUIET FALSE)
set(CMAKE_REQUIRED_DEFINITIONS "-D_GNU_SOURCE") # Enable use of glibc stuff
set(CMAKE_REQUIRED_INCLUDES  "")
check_symbol_exists("asprintf"   "stdio.h"   AB_HAS_ASPRINTF)
check_symbol_exists("vasprintf"  "stdio.h"   AB_HAS_VASPRINTF)
check_symbol_exists("strdup"     "string.h"  AB_HAS_STRDUP)
check_symbol_exists("_strdup"    "string.h"  AB_HAS__STRDUP)
check_symbol_exists("strtok_r"   "string.h"  AB_HAS_STRTOK_R)
check_symbol_exists("strtok_s"   "string.h"  AB_HAS_STRTOK_S)
configure_file(include/Amber/util/compat.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/Amber/util/compat.h
    @ONLY)

# Amber::util
add_library(util
    ${CMAKE_CURRENT_BINARY_DIR}/include/Amber/util/AB_compiler_detection.h
    ${CMAKE_CURRENT_BINARY_DIR}/include/Amber/util/compat.h
    include/Amber/util/common.h
    include/Amber/util/ring.h
    include/Amber/util/vector.h
    src/ring.c
    # src/noise.c
    )
target_link_libraries(util
    PUBLIC SDL2::SDL2)
target_include_directories(util
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include/>)
target_compile_features(util
    PRIVATE c_std_99)

add_library(Amber::util ALIAS util)

install(TARGETS util EXPORT amber-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Runtime NAMELINK_COMPONENT Development
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT Development
    FILES_MATCHING PATTERN "*.h")
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT Development
    FILES_MATCHING PATTERN "*.h")

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
