
add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/objparse.c
        ${CMAKE_CURRENT_BINARY_DIR}/objparse.h
    COMMAND
        ${LEMON_EXECUTABLE}
            -d${CMAKE_CURRENT_BINARY_DIR}
            -T${LEMON_TEMPLATE}
            src/objparse.y
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}
    MAIN_DEPENDENCY
        src/objparse.y
    BYPRODUCTS
        ${CMAKE_CURRENT_BINARY_DIR}/objparse.out
    )

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/objlex.c
        ${CMAKE_CURRENT_BINARY_DIR}/objlex.h
    COMMAND
        ${FLEX_EXECUTABLE}
            --outfile=${CMAKE_CURRENT_BINARY_DIR}/objlex.c
            --header-file=${CMAKE_CURRENT_BINARY_DIR}/objlex.h
            src/objlex.l
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/objparse.c
    MAIN_DEPENDENCY
        src/objlex.l
    )

add_library(objloader
    ${CMAKE_CURRENT_BINARY_DIR}/objlex.c
    ${CMAKE_CURRENT_BINARY_DIR}/objparse.c)
target_include_directories(objloader
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
target_compile_features(objloader
    PRIVATE c_std_99)
target_link_libraries(objloader
    PUBLIC Amber::util cglm::cglm)

add_library(graphics
    src/graphics_system.c)
target_link_libraries(graphics
    PRIVATE Amber::ECS Amber::util SDL2::SDL2 glad::glad OpenGL::GL)
target_include_directories(graphics
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
target_compile_features(graphics
    PRIVATE c_std_99)
add_library(Amber::graphics ALIAS graphics)

install(TARGETS graphics EXPORT amber-targets
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
