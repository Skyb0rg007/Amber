#############################################################################
##
## Setup
##
#############################################################################

cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(Amber
    DESCRIPTION "A Game Engine written in C"
    VERSION 0.1
    HOMEPAGE_URL "https://github.com/Skyb0rg007/Amber"
    LANGUAGES C)

## Use custom CMake modules for finding packages
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

include(GNUInstallDirs)            # ${CMAKE_INSTALL_*}
include(CMakePackageConfigHelpers) # write_basic_package_version_file()
include(CheckIPOSupported)         # check_ipo_supported()
include(AddAmberTest)              # add_amber_test()
include(EnableWarnings)            # enable_warnings()

#############################################################################
##
## Configuration Options
##
#############################################################################

option(AB_GETTEXT_SUPPORT "Enable i18n through GNU gettext" OFF) # TODO
option(AB_ENABLE_ASAN "Enable asan on tests" OFF)
option(AB_BUILD_DOCUMENTATION "Build the library documentation" OFF)
set(AB_CACHELINE_SIZE 64 CACHE STRING "The size of L1 cache")

## Rpath support
if(UNIX AND NOT APPLE)
    set(CMAKE_INSTALL_RPATH "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}/")
elseif(APPLE)
    set(CMAKE_INSTALL_RPATH "@rpath/../${CMAKE_INSTALL_LIBDIR}")
endif()

## Address Sanitizer support for Debug builds
if(AB_ENABLE_ASAN)
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address")
endif()

## Enable LTO for release builds
check_ipo_supported(RESULT ipo_supported OUTPUT ipo_reason LANGUAGES C)
if(ipo_supported)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
else()
    message(STATUS "IPO not supported: ${ipo_reason}")
endif()

#############################################################################
##
## Dependencies
##
#############################################################################

## External dependencies (not in source tree)
set(OpenGL_GL_PREFERENCE "LEGACY") # For Linux OpenGL
#            Dep      Vsn
find_package(OpenGL   3.3    MODULE REQUIRED) # OpenGL::GL
find_package(SDL2     2.0.8  MODULE REQUIRED) # SDL2::SDL2
find_package(Sodium   1.0.17 MODULE REQUIRED) # sodium::sodium
find_package(ZLIB     1.2    MODULE REQUIRED) # ZLIB::ZLIB
find_package(Freetype 2.9.1  MODULE REQUIRED) # Freetype::Freetype
if(AB_GETTEXT_SUPPORT)
    # Runtime gettext package
    find_package(Intl MODULE REQUIRED) # gettext::intl
    # Compiletime gettext package
    include(Gettext_helpers)           # configure_gettext()
endif()
if(AB_BUILD_DOCUMENTATION)
    find_package(Doxygen MODULE OPTIONAL_COMPONENTS dot) # doxygen_add_docs()
    if(NOT DOXYGEN_FOUND)
        message(WARNING "Doxygen not found - can only build LDoc documentation")
    endif()
endif()

# Internal dependencies (in source tree)
add_subdirectory(thirdparty/nuklear/)  # nk::nuklear
add_subdirectory(thirdparty/glad/   )  # glad::glad
add_subdirectory(thirdparty/stb/    )  # stb::stb_image
add_subdirectory(thirdparty/sqlite3/)  # sqlite3::sqlite3
add_subdirectory(thirdparty/cglm/   )  # cglm::cglm
add_subdirectory(thirdparty/lua-5.3/)  # lua::lua,luac,lualib,lualib-static
add_subdirectory(thirdparty/ldoc/   )  # ldoc_add_docs()
add_subdirectory(thirdparty/lpeg/   )  # lpeg::lpeg

#############################################################################
##
## Testing
##
#############################################################################

if(NOT DEFINED BUILD_TESTING)
    set(BUILD_TESTING OFF CACHE BOOL "Enable testing" FORCE)
endif()
include(CTest)
if(BUILD_TESTING)
    find_package(CMocka MODULE REQUIRED) # cmocka::cmocka
endif()

#############################################################################
##
## Libraries and executables
##
#############################################################################

add_subdirectory(Amber/util/      util/     )  # Amber::util
add_subdirectory(Amber/ECS/       ECS/      )  # Amber::ECS
add_subdirectory(Amber/graphics/  graphics/ )  # Amber::graphics
add_subdirectory(Amber/scripting/ scripting/)  # Amber::scripting
add_subdirectory(Amber/ui/        ui/       )  # Amber::ui
add_subdirectory(Amber/driver/    driver/   )  # Amber::driver

#############################################################################
##
## Documentation
##
#############################################################################

if(AB_BUILD_DOCUMENTATION)

    ldoc_add_docs(ldoc
        Amber/scripting/
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/ldoc/"
        COMMENT "Generating LDoc documentation")

    if(DOXYGEN_FOUND)
        set(DOXYGEN_GENERATE_HTML TRUE)
        set(DOXYGEN_EXTRACT_STATIC TRUE)
        set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs/)
        # set(DOXYGEN_FULL_PATH_NAMES OFF)
        set(DOXYGEN_STRIP_FROM_PATH "${CMAKE_CURRENT_BINARY_DIR},Amber")
        set(DOXYGEN_PREDEFINED "__DOXYGEN__")
        doxygen_add_docs(docs
            Amber/util/include/
            Amber/ECS/include/
            ${CMAKE_CURRENT_BINARY_DIR}/util/include/
            COMMENT "Generating Doxygen documentation")
    endif()

endif()

#############################################################################
##
## Installation of exported CMake targets
##
#############################################################################

install(EXPORT amber-targets
    NAMESPACE Amber::
    FILE Amber-targets.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Amber/ COMPONENT Development)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Amber-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/Amber-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Amber/)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/Amber-config-version.cmake
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY ExactVersion)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/Amber-config-version.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/Amber-config.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Amber/ COMPONENT Development)

#############################################################################
##
## Packaging
##
#############################################################################

include(CPack)
