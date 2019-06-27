
add_amber_test(Amber.config.basic_test
    SOURCES basic_test.c
    ARGS ${CMAKE_CURRENT_SOURCE_DIR}/basic_test.lua
    LINK_LIBRARIES Amber::util Amber::config
    COMPILE_FEATURES c_std_99)
