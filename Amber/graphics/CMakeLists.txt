
add_library(objloader
    src/objloader.c)
target_include_directories(objloader
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
target_compile_features(objloader
    PRIVATE c_std_99)
target_link_libraries(objloader
    PUBLIC Amber::util cglm::cglm Amber::compat)

add_library(graphics
    src/graphics_system_init.c
    src/graphics_system_cleanup.c
    src/graphics_system_message_handler.c
    src/graphics_system_run.c
    )
target_link_libraries(graphics
    PRIVATE Amber::ECS Amber::util SDL2::SDL2 OpenGL::GL)
if(NOT EMSCRIPTEN)
    target_link_libraries(graphics PRIVATE glad::glad)
else()
    target_link_options(graphics PUBLIC "SHELL:-s USE_SDL=2")
    target_compile_options(graphics PUBLIC "SHELL:-s USE_SDL=2")
endif()
target_include_directories(graphics
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
target_compile_features(graphics
    PRIVATE c_std_99)
add_library(Amber::graphics ALIAS graphics)

install(TARGETS objloader graphics EXPORT amber-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Runtime NAMELINK_COMPONENT Development
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT Development
    FILES_MATCHING PATTERN "*.h")

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
