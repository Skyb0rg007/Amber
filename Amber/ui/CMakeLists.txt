
add_library(ui
    src/ui_subsystem.c)
target_link_libraries(ui
    PRIVATE Amber::ECS SDL2::SDL2 nk::nuklear)
target_include_directories(ui
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
    PRIVATE src/)
target_compile_features(ui
    PRIVATE c_std_99)

add_library(Amber::ui ALIAS ui)

install(TARGETS ui EXPORT amber-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Runtime NAMELINK_COMPONENT Development
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT Development
    FILES_MATCHING PATTERN "*.h")
