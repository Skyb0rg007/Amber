cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(glad
    VERSION 0.1.28
    DESCRIPTION "Multi-Language Vulkan/GL/GLES/EGL/GLX/WGL Loader-Generator based on the official specs."
    HOMEPAGE_URL "https://github.com/Dav1dde/glad"
    LANGUAGES C)

set(OpenGL_GL_PREFERENCE "LEGACY")
find_package(OpenGL 3.3 MODULE REQUIRED) # OpenGL::GL

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

get_target_property(GLAD_LIB_TYPE glad TYPE)
if(GLAD_LIB_TYPE STREQUAL "SHARED_LIBRARY")
    target_compile_definitions(glad
        PUBLIC "GLAD_GLAPI_EXPORT"
        PRIVATE "GLAD_GLAPI_EXPORT_BUILD")
endif()

add_library(glad::glad ALIAS glad)

install(TARGETS glad EXPORT amber-targets
    RUNTIME DESTINATION bin COMPONENT Runtime
    LIBRARY DESTINATION lib64 COMPONENT Runtime NAMELINK_COMPONENT Development
    ARCHIVE DESTINATION lib64 COMPONENT Development
    INCLUDES DESTINATION include)
install(DIRECTORY include/ DESTINATION include COMPONENT Development)
