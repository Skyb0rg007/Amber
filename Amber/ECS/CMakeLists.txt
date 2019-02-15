
add_library(ECS
    src/world.c
    )
target_include_directories(ECS
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
target_compile_features(ECS
    PRIVATE c_std_99)
target_link_libraries(ECS
    PUBLIC Amber::util)

add_library(Amber::ECS ALIAS ECS)

install(TARGETS ECS EXPORT amber-targets
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
