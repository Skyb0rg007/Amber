
add_library(config
    src/conf.c)
target_link_libraries(config
    PUBLIC Amber::util lua::lualib)
target_include_directories(config
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
target_compile_features(config
    PRIVATE c_std_99)

add_library(Amber::config ALIAS config)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
