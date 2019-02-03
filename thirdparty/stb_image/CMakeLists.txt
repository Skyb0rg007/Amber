cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(stb-image
    VERSION 2.19
    HOMEPAGE_URL "https://github.com/nothings/stb"
    DESCRIPTION "public domain image loader"
    LANGUAGES C)

add_library(stb_image
    src/stb_image-implementation.c)
target_include_directories(stb_image
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_compile_features(stb_image
    PRIVATE c_std_90)
set_target_properties(stb_image PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION}
    WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

add_library(stb::stb_image ALIAS stb_image)

install(TARGETS stb_image EXPORT amber-targets
    RUNTIME DESTINATION bin COMPONENT Runtime
    LIBRARY DESTINATION lib64 COMPONENT Runtime NAMELINK_COMPONENT Development
    ARCHIVE DESTINATION lib64 COMPONENT Development)
