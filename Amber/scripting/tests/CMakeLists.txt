
add_amber_test(Amber.scripting.config-test
    SOURCES config-test.c
    LINK_LIBRARIES Amber::scripting Amber::util
    COMPILE_FEATURES c_std_99
    ARGS ${CMAKE_CURRENT_SOURCE_DIR}/config-test.lua)

