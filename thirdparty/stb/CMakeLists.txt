cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(stb
    HOMEPAGE_URL "https://github.com/nothings/stb"
    DESCRIPTION "public domain image loader"
    LANGUAGES C)

add_library(stb_image
    src/stb_image-implementation.c)
target_include_directories(stb_image
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_compile_features(stb_image
    PRIVATE c_std_99) # // comments
set_target_properties(stb_image PROPERTIES
    VERSION 2.19
    SOVERSION 2.19
    WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
enable_warnings(stb_image "-Wall -pedantic /W3")

add_library(stb::stb_image ALIAS stb_image)

install(TARGETS stb_image EXPORT amber-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Runtime NAMELINK_COMPONENT Development
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development)
