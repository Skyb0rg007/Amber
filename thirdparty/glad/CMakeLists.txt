cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(glad
    VERSION 0.1.28
    DESCRIPTION "Multi-Language Vulkan/GL/GLES/EGL/GLX/WGL Loader-Generator based on the official specs."
    HOMEPAGE_URL "https://github.com/Dav1dde/glad"
    LANGUAGES C)

add_library(glad
    src/glad.c)
target_include_directories(glad
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
target_link_libraries(glad
    PRIVATE OpenGL::GL ${CMAKE_DL_LIBS})
target_compile_features(glad
    PRIVATE c_std_90)
set_target_properties(glad PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION}
    WINDOWS_EXPORT_ALL_SYMBOLS FALSE)

enable_warnings(glad "-Wall -Wextra /W3")

if($<TARGET_PROPERTY:glad,TYPE> STREQUAL "SHARED_LIBRARY")
    target_compile_definitions(glad
        PUBLIC "GLAD_GLAPI_EXPORT"
        PRIVATE "GLAD_GLAPI_EXPORT_BUILD")
endif()

add_library(glad::glad ALIAS glad)

install(TARGETS glad EXPORT amber-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Runtime NAMELINK_COMPONENT Development
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT Development)
