cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(Amber
    DESCRIPTION "A Game Engine written in C"
    VERSION 0.1
    HOMEPAGE_URL "www.github.com"
    LANGUAGES C)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

if(UNIX AND NOT APPLE)
    set(CMAKE_INSTALL_RPATH "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}/")
endif()

if (C_COMPILER_ID MATCHES "(GNU|Clang|AppleClang)")
    list(APPEND COMPILE_OPTIONS -Wall -Wextra -Wvla -pedantic -Wconversion)
endif()

# External Dependencies
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL 3.3 MODULE COMPONENTS OpenGL REQUIRED) # OpenGL::OpenGL
find_package(SDL2 2.0.8 MODULE REQUIRED) # SDL2::SDL2

# Internal dependencies
add_subdirectory(thirdparty/nuklear/)   # nk::nuklear
add_subdirectory(thirdparty/glad/)      # glad::glad
add_subdirectory(thirdparty/stb_image/) # stb::stb_image

# Testing
include(CTest)

# Libraries and executables
add_subdirectory(Amber/util/     util/)     # Amber::util
add_subdirectory(Amber/ECS/      ECS/)      # Amber::ECS
add_subdirectory(Amber/graphics/ graphics/) # Amber::graphics
add_subdirectory(Amber/ui/       ui/)       # Amber::ui
add_subdirectory(Amber/driver/   driver/)   # Amber::driver

# Installation
install(EXPORT amber-targets
    NAMESPACE Amber::
    FILE Amber-targets.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Amber/)
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

# Packaging
include(CPack)
