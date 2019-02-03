
add_library(my_module MODULE
    module.c)
target_link_libraries(my_module
    PRIVATE lua::lualib)
target_link_options(my_module
    PRIVATE "LINKER:-E")
