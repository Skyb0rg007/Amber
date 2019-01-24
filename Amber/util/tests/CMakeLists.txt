
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
