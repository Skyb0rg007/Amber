include(CheckSymbolExists)
include(CheckFunctionExists)
include(WriteCompilerDetectionHeader)

# Detect the compiler being used
write_compiler_detection_header(
    FILE ${CMAKE_CURRENT_BINARY_DIR}/include/Amber/util/AB_compiler_detection.h
    PREFIX AB
    PROLOG "/* Amber::util - compiler detection header */"
    FEATURES c_restrict
    COMPILERS "GNU" "Clang" "AppleClang" "MSVC" "Intel")

# Detect the availability of functions that may be in libc
set(CMAKE_REQUIRED_QUIET FALSE)
set(CMAKE_REQUIRED_DEFINITIONS "-D_GNU_SOURCE") 
set(CMAKE_REQUIRED_INCLUDES  "")
check_symbol_exists("asprintf"   "stdio.h"   AB_HAS_ASPRINTF)  # glibc
check_symbol_exists("vasprintf"  "stdio.h"   AB_HAS_VASPRINTF) # glibc
check_symbol_exists("strdup"     "string.h"  AB_HAS_STRDUP)    # Glibc
check_symbol_exists("_strdup"    "string.h"  AB_HAS__STRDUP)   # Windows
check_symbol_exists("strtok_r"   "string.h"  AB_HAS_STRTOK_R)  # Posix
check_symbol_exists("strtok_s"   "string.h"  AB_HAS_STRTOK_S)  # Windows
check_symbol_exists("va_copy"    "stdarg.h"  AB_HAS_VA_COPY)   # C99
check_symbol_exists("__va_copy"  "stdarg.h"  AB_HAS___VA_COPY) # old systems
check_symbol_exists("_printf_p"  "stdio.h"   AB_HAS__PRINTF_P) # Windows
if(NOT AB_HAS__PRINTF_P)
    try_run(test_ran test_compiled
        ${CMAKE_CURRENT_BINARY_DIR}/dev/printf_positional
        ${CMAKE_CURRENT_SOURCE_DIR}/dev/printf_positional.c
        RUN_OUTPUT_VARIABLE test_output)
    if(test_compiled
            AND (test_ran EQUAL 0)
            AND (test_output STREQUAL "2 1"))
        set(AB_PRINTF_POSITIONAL TRUE)
    endif()
endif()
# Create a compatibility header depending on the available options
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
    src/compat.c
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
