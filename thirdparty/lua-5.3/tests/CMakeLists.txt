add_executable(lua-module-test
    module_main.c)
target_link_libraries(lua-module-test
    PRIVATE lua::lualib)
target_compile_features(lua-module-test
    PRIVATE c_std_99)

add_library(my_module MODULE
    my_module.c)
target_link_libraries(my_module
    PRIVATE lua::lualib)
target_link_options(my_module
    PRIVATE "LINKER:-E")

add_test(Lua.module-test lua-module-test 
    ${CMAKE_CURRENT_SOURCE_DIR}/module_test.lua)
