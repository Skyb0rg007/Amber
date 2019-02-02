add_executable(basic
    basic-test.c)
target_link_libraries(basic
    PRIVATE SDL2::SDL2 Amber::util Amber::ECS)
target_compile_features(basic
    PRIVATE c_std_99)
add_test(Amber.ECS.basic basic)
