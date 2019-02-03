## Setup
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(Amber
    DESCRIPTION "A Game Engine written in C"
    VERSION 0.1
    HOMEPAGE_URL "www.github.com"
    LANGUAGES C)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

## Configuration
option(AB_GETTEXT_SUPPORT "Enable i18n through GNU gettext" OFF)

# Rpath support
if(UNIX AND NOT APPLE)
    set(CMAKE_INSTALL_RPATH "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}/")
elseif(APPLE)
    set(CMAKE_INSTALL_RPATH "@rpath/../${CMAKE_INSTALL_LIBDIR}")
endif()

# Enable warnings
if (C_COMPILER_ID MATCHES "(GNU|Clang|AppleClang)")
    list(APPEND COMPILE_OPTIONS -Wall -Wextra -Wvla -pedantic -Wconversion)
endif()

## Dependencies
# External dependencies
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL 3.3 MODULE COMPONENTS OpenGL REQUIRED) # OpenGL::OpenGL
find_package(SDL2 2.0.8 MODULE REQUIRED)                   # SDL2::SDL2
find_package(Doxygen MODULE QUIET OPTIONAL_COMPONENTS dot)
if(AB_GETTEXT_SUPPORT)
    find_package(Intl MODULE REQUIRED) # gettext::intl
    include(Gettext_helpers) # configure_gettext()
endif()

# Internal dependencies
add_subdirectory(thirdparty/nuklear/)   # nk::nuklear
add_subdirectory(thirdparty/glad/)      # glad::glad
add_subdirectory(thirdparty/stb_image/) # stb::stb_image
add_subdirectory(thirdparty/klib/)      # klib::khash
add_subdirectory(thirdparty/inih/)      # inih::inih
add_subdirectory(thirdparty/sqlite3/)   # sqlite3::sqlite3
add_subdirectory(thirdparty/lua-5.3.5/) # lua::lualib

## Testing
if(NOT DEFINED BUILD_TESTING)
    set(BUILD_TESTING OFF CACHE BOOL "Enable testing" FORCE)
endif()
include(CTest)
if(BUILD_TESTING)
    find_package(cmocka CONFIG REQUIRED) # cmocka
endif()

## Libraries and executables
add_subdirectory(Amber/util/     util/)     # Amber::util
add_subdirectory(Amber/ECS/      ECS/)      # Amber::ECS
add_subdirectory(Amber/graphics/ graphics/) # Amber::graphics
add_subdirectory(Amber/ui/       ui/)       # Amber::ui
add_subdirectory(Amber/driver/   driver/)   # Amber::driver

## Documentation
if(DOXYGEN_FOUND)
    set(DOXYGEN_GENERATE_HTML ON)
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs/)
    set(DOXYGEN_PREDEFINED "__DOXYGEN__")
    doxygen_add_docs(docs
        Amber/util/
        COMMENT "Generating Doxygen documentation")
endif()

## Installation of exported CMake targets
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

## Packaging
include(CPack)
