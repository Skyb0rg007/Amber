
function(add_amber_test target_name)

    set(options)
    set(single "SANITIZER")
    set(multi
        "SOURCES" "LINK_LIBRARIES"
        "COMPILE_FEATURES" "LABELS"
        "ARGS")

    cmake_parse_arguments(add_amber_test
        "${options}" "${single}" "${multi}"
        "${ARGN}")

    if(NOT DEFINED add_amber_test_SOURCES)
        message(FATAL_ERROR "No sources provided for target ${target_name}")
    endif()

    add_executable("${target_name}" ${add_amber_test_SOURCES})

    # Add ASAN testing
    if(AB_ENABLE_ASAN)
        if(add_amber_test_SANITIZER STREQUAL "address")
            target_compile_options("${target_name}"
                PRIVATE "-fsanitize=address" "-fno-omit-frame-pointer")
            target_link_options("${target_name}"
                PRIVATE "-fsanitize=address" "-fno-omit-frame-pointer")
        elseif(add_amber_test_SANITIZER STREQUAL "thread")
            target_compile_options("${target_name}"
                PRIVATE "-fsanitize=thread")
            target_link_options("${target_name}"
                PRIVATE "-fsanitize=thread")
        elseif(DEFINED add_amber_test_SANITIZER)
            message(STATUS "${add_amber_test_SANITIZER} is not supported")
        endif()
    endif()

    # Add additional warnings
    if(C_COMPILER_ID MATCHES "(GNU|AppleClang|Clang)")
        target_compile_options("${target_name}"
            PRIVATE "-Wall" "-Wextra" "-pedantic")
    elseif(C_COMPILER_ID STREQUAL MSVC)
        target_compile_options("${target_name}"
            PRIVATE "/W3")
    endif()

    # Link with cmocka
    list(APPEND add_amber_test_LINK_LIBRARIES cmocka)

    if(DEFINED add_amber_test_COMPILE_FEATURES)
        target_compile_features("${target_name}"
            PRIVATE ${add_amber_test_COMPILE_FEATURES})
    endif()

    if(DEFINED add_amber_test_LINK_LIBRARIES)
        target_link_libraries("${target_name}"
            PRIVATE ${add_amber_test_LINK_LIBRARIES})
    endif()

    add_test("${target_name}" "${target_name}" ${add_amber_test_ARGS})

    if(DEFINED add_amber_test_LABELS)
        set_tests_properties("${target_name}" PROPERTIES
            LABELS "${add_amber_test_LABELS}")
    endif()

endfunction()

