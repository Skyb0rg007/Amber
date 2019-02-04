
add_library(scripting
    src/load_config.c)
target_link_libraries(scripting
    PRIVATE
    lua::lualib
    Amber::util
    SDL2::SDL2)
target_include_directories(scripting
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
target_compile_features(scripting
    PRIVATE c_std_99)

add_library(Amber::scripting ALIAS scripting)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

install(TARGETS scripting # EXPORT amber-targets TODO
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Runtime NAMELINK_COMPONENT Development
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT Development
    FILES_MATCHING PATTERN "*.h")
