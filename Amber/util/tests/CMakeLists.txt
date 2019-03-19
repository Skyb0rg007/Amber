
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

# add_amber_test(Amber.util.noise-test
    # SOURCES noise-test.c
    # LINK_LIBRARIES SDL2::SDL2 Amber::util
    # COMPILE_FEATURES c_std_99
    # COMPILE_OPTIONS -Wall -Wextra)
# add_executable(noise-test
    # noise-test.c)
# target_link_libraries(noise-test
    # PRIVATE SDL2::SDL2 Amber::util)
# target_compile_features(noise-test
    # PRIVATE c_std_99)
# target_compile_options(noise-test
    # PRIVATE -Wall -Wextra)
# if(NOT DISABLE_ASAN)
    # target_compile_options(noise-test
        # PUBLIC -fsanitize=thread)
    # target_link_options(noise-test
        # PUBLIC -fsanitize=thread)
# endif()
# add_test(Amber.util.noise-test noise-test)

add_amber_test(Amber.util.linux-hashtable
    SOURCES linux-hashtable-test.c
    LINK_LIBRARIES Amber::util
    COMPILE_FEATURES c_std_99)

add_amber_test(Amber.util.vector-new-test
    SOURCES vector-new-test.c
    LINK_LIBRARIES Amber::util SDL2::SDL2
    COMPILE_FEATURES c_std_90)
