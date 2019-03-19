
# Example of a small program
add_amber_test(Amber.util.minimal-race
    SOURCES minimal-ring-race.c
    LINK_LIBRARIES SDL2::SDL2
    COMPILE_FEATURES c_std_99)

# Example of a verify-able version using stdatomic
add_amber_test(Amber.util.minimal-race-stdatomic
    SOURCES minimal-ring-race-stdatomic.c
    LINK_LIBRARIES SDL2::SDL2
    COMPILE_FEATURES c_std_11
    SANITIZER thread)

add_amber_test(Amber.util.vector-test
    SOURCES vector-test.c
    LINK_LIBRARIES SDL2::SDL2 Amber::util
    COMPILE_FEATURES c_std_99)

add_amber_test(Amber.util.ring-test
    SOURCES ring-test.c
    LINK_LIBRARIES SDL2::SDL2 Amber::util
    COMPILE_FEATURES c_std_99)

add_amber_test(Amber.util.linux-hashtable
    SOURCES linux-hashtable-test.c
    LINK_LIBRARIES Amber::util
    COMPILE_FEATURES c_std_99)

