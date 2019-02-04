
add_executable(script-config-test
    config-test.c)
target_link_libraries(script-config-test
    PRIVATE Amber::scripting Amber::util)
add_test(Amber.scripting.config 
    script-config-test ${CMAKE_CURRENT_SOURCE_DIR}/config-test.lua)
