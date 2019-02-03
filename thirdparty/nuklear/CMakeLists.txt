cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(nuklear
    VERSION 1.0
    HOMEPAGE_URL "https://github.com/vurtun/nuklear"
    DESCRIPTION "an immediate mode GUI toolkit written in ANSI C"
    LANGUAGES C)

include(CheckLibraryExists)

add_library(nuklear
    src/nuklear-implementation.c)
target_include_directories(nuklear
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
target_compile_features(nuklear
    PRIVATE c_std_90)
set_target_properties(nuklear PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION}
    WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

check_library_exists("m" "pow" "" HAVE_LIBM)
if(HAVE_LIBM)
    target_link_libraries(nuklear
        PRIVATE "m")
endif()

add_library(nk::nuklear ALIAS nuklear)

install(TARGETS nuklear EXPORT amber-targets
    RUNTIME DESTINATION bin COMPONENT Runtime
    ARCHIVE DESTINATION lib64 COMPONENT Development
    LIBRARY DESTINATION lib64 COMPONENT Runtime NAMELINK_COMPONENT Development
    INCLUDES DESTINATION include)
install(DIRECTORY include/ DESTINATION include COMPONENT Development)
