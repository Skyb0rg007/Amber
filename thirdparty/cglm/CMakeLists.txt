cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(cglm
    DESCRIPTION "optimized 3D math library written in C99"
    HOMEPAGE_URL "https://github.com/recp/cglm"
    LANGUAGES C)

add_library(cglm
    src/affine.c
    src/bezier.c
    src/box.c
    src/cam.c
    src/config.h
    src/curve.c
    # src/dllmain.c
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
# cglm uses restrict and inline
target_compile_features(cglm
    PUBLIC c_std_99)
if(CMAKE_C_COMPILER_ID MATCHES "(GNU|Clang|AppleClang)")
    target_compile_options(cglm
        PRIVATE -Wstrict-aliasing=2 -fstrict-aliasing)
endif()
if(WIN32 AND $<TARGET_PROPERTY:cglm,TYPE> STREQUAL "SHARED_LIBRARY")
    target_compile_definitions(cglm PRIVATE CGLM_DLL)
endif()
check_library_exists("m" "pow" "" HAVE_LIBM)
if(HAVE_LIBM)
    # Link PUBLIC because cglm has inline functions
    target_link_libraries(cglm PUBLIC m)
endif()

add_library(cglm::cglm ALIAS cglm)

