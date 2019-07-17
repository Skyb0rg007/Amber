cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(cglm
    DESCRIPTION "optimized 3D math library written in C99"
    VERSION 0.5.1
    HOMEPAGE_URL "https://github.com/recp/cglm"
    LANGUAGES C)

include(CheckLibraryExists)

add_library(cglm
    src/affine.c
    src/bezier.c
    src/box.c
    src/cam.c
    src/config.h
    src/curve.c
    src/ease.c
    src/euler.c
    src/frustum.c
    src/io.c
    src/mat3.c
    src/mat4.c
    src/plane.c
    src/project.c
    src/quat.c
    src/sphere.c
    src/vec3.c
    src/vec4.c)
target_include_directories(cglm
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
target_compile_features(cglm
    PUBLIC c_std_99)
if(WIN32 AND $<TARGET_PROPERTY:cglm,TYPE> STREQUAL "SHARED_LIBRARY")
    target_compile_definitions(cglm PRIVATE CGLM_DLL)
endif()
check_library_exists("m" "pow" "" HAVE_LIBM)
if(HAVE_LIBM)
    # Link PUBLIC because cglm has inline functions
    target_link_libraries(cglm PUBLIC m)
endif()
set_target_properties(cglm PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION}
    WINDOWS_EXPORT_ALL_SYMBOLS OFF)

enable_warnings(cglm 
    "-Wall -Wextra -pedantic -Wstrict-aliasing -fstrict-aliasing -Wconversion /W4")

add_library(cglm::cglm ALIAS cglm)

install(TARGETS cglm EXPORT amber-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Runtime NAMELINK_COMPONENT Development
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    COMPONENT Development)
