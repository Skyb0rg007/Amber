
add_amber_test(Amber.ECS.basic
    SOURCES basic-test.c
    LABELS basic
    LINK_LIBRARIES SDL2::SDL2 Amber::util Amber::ECS
    COMPILE_FEATURES c_std_99)

