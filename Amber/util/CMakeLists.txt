include(CheckSymbolExists)
include(WriteCompilerDetectionHeader)
include(CheckIncludeFile)

# if(NOT AB_HAS__PRINTF_P)
    # try_run(test_ran test_compiled
        # ${CMAKE_CURRENT_BINARY_DIR}/dev/printf_positional
        # ${CMAKE_CURRENT_SOURCE_DIR}/dev/printf_positional.c
        # RUN_OUTPUT_VARIABLE test_output)
    # string(STRIP "${test_output}" test_output)
    # if(test_compiled
            # AND (test_ran EQUAL 0)
            # AND (test_output STREQUAL "2 1"))
        # set(AB_PRINTF_POSITIONAL TRUE)
    # elseif(AB_GETTEXT_SUPPORT)
        # # TODO: manual fallback
        # message(FATAL_ERROR "Unable to find printf with positional support")
    # endif()
# endif()

# Amber::util
add_library(util
    include/Amber/util/common.h
    include/Amber/util/ring.h
    include/Amber/util/vector.h
    src/ring.c
    )
target_link_libraries(util
    PUBLIC SDL2::SDL2 Amber::compat)
target_include_directories(util
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
target_compile_features(util
    PRIVATE c_std_99)
enable_warnings(util "-Wall -Wextra -pedantic")

add_library(Amber::util ALIAS util)

install(TARGETS util EXPORT amber-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Runtime NAMELINK_COMPONENT Development
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT Development
    FILES_MATCHING PATTERN "*.h")

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
