cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(inih
    DESCRIPTION "Simple .INI file parser written in C"
    HOMEPAGE_URL "https://github.com/benhoyt/inih"
    LANGUAGES C)

add_library(inih
    ini.c)
target_include_directories(inih
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_compile_features(inih
    PRIVATE c_std_90)

add_library(inih::inih ALIAS inih)

install(TARGETS inih
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Runtime NAMELINK_COMPONENT Development
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/ini.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT Development)
