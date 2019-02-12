
add_amber_test(Amber.graphics.objtest
    SOURCES objtest.c
    COMPILE_FEATURES c_std_99
    LINK_LIBRARIES Amber::util Amber::graphics objloader)
