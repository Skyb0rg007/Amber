
# Testing the real thing
add_executable(ring-test
    ring-test.c)
target_link_libraries(ring-test
    PRIVATE Amber::util SDL2::SDL2)
target_compile_features(ring-test
    PRIVATE c_std_99)
add_test(Amber.util.ring ring-test)

# Example of a small program
add_executable(minimal-race
    minimal-ring-race.c)
target_link_libraries(minimal-race
    PRIVATE SDL2::SDL2)
target_compile_features(minimal-race
    PRIVATE c_std_99)
add_test(Amber.util.minimal-race minimal-race)

# Example of a verify-able version using stdatomic
add_executable(minimal-race-stdatomic
    minimal-ring-race-stdatomic.c)
target_link_libraries(minimal-race-stdatomic
    PRIVATE SDL2::SDL2)
target_compile_features(minimal-race-stdatomic
    PRIVATE c_std_11)
if(NOT DISABLE_ASAN)
    target_compile_options(minimal-race-stdatomic
        PUBLIC -fsanitize=thread)
    target_link_options(minimal-race-stdatomic
        PUBLIC -fsanitize=thread)
endif()
add_test(Amber.util.minimal-race-stdatomic minimal-race-stdatomic)

add_executable(hashtable-test
    hashtable-test.c)
target_link_libraries(hashtable-test
    PRIVATE SDL2::SDL2 Amber::util klib::khash)
target_compile_features(hashtable-test
    PRIVATE c_std_99)
target_compile_options(hashtable-test
    PRIVATE -Wall -Wextra -pedantic -Wconversion)
# if(NOT DISABLE_ASAN)
    # target_compile_options(hashtable-test
        # PUBLIC -fsanitize=address)
    # target_link_options(hashtable-test
        # PUBLIC -fsanitize=address)
# endif()
add_test(Amber.util.hashtable-test hashtable-test)

add_executable(vector-test
    vector-test.c)
target_link_libraries(vector-test
    PRIVATE SDL2::SDL2 Amber::util)
target_compile_features(vector-test
    PRIVATE c_std_99)
target_compile_options(vector-test
    PRIVATE -Wall -Wextra)
# if(NOT DISABLE_ASAN)
    # target_compile_options(vector-test
        # PUBLIC -fsanitize=thread)
    # target_link_options(vector-test
        # PUBLIC -fsanitize=thread)
# endif()
add_test(Amber.util.vector-test vector-test)

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


add_executable(mocking-test
    mocking-test.c)
target_link_libraries(mocking-test
    PRIVATE cmocka Amber::util)
target_compile_features(mocking-test
    PRIVATE c_std_99)
add_test(Amber.util.mocking mocking-test)
