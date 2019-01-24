
add_library(util
    src/ring.c)
target_link_libraries(util
    PUBLIC SDL2::SDL2)
target_include_directories(util
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
target_compile_features(util
    PRIVATE c_std_99)

add_library(Amber::util ALIAS util)

install(TARGETS util EXPORT amber-targets
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
