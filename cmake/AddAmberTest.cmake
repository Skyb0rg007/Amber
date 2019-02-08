include(CheckCCompilerFlag)

if(C_COMPILER_ID MATCHES "(GNU|AppleClang|Clang)")
    set(ADD_AMBER_TEST_WARNINGS "-Wall -Wextra -Wconversion"
        CACHE INTERNAL "Compile Options to give extra warnings")
elseif(C_COMPILER_ID STREQUAL MSVC)
    set(ADD_AMBER_TEST_WARNINGS "/W3"
        CACHE INTERNAL "Compile Options to give extra warnings")
endif()

if(ENABLE_ASAN)
    set(ADD_AMBER_TEST_COMPILE_OPTIONS "-fsanitize=address")
else()
endif()

function(add_amber_test target_name)

    set(options)
    set(single "SANITIZER")
    set(multi
        "SOURCES" "COMPILE_OPTIONS"
        "LINK_LIBRARIES" "LINK_OPTIONS"
        "COMPILE_FEATURES")

    cmake_parse_arguments(add_amber_test
        "${options}" "${single}" "${multi}" "${ARGN}")

    if(NOT DEFINED add_amber_test_SOURCES)
        message(FATAL_ERROR "No sources provided for target ${target_name}")
    endif()

    if(AB_ENABLE_ASAN)
        if(add_amber_test_SANITIZER STREQUAL "address")
            list(APPEND add_amber_test_COMPILE_OPTIONS
                "-fsanitize=address" "-fno-omit-frame-pointer")
            list(APPEND add_amber_test_LINK_OPTIONS
                "-fsanitize=address" "-fno-omit-frame-pointer")
        elseif(add_amber_test_SANITIZER STREQUAL "thread")
            list(APPEND add_amber_test_COMPILE_OPTIONS "-fsanitize=thread")
            list(APPEND add_amber_test_LINK_OPTIONS "-fsanitize=thread")
        elseif(DEFINED add_amber_test_SANITIZER)
            message(STATUS "${add_amber_test_SANITIZER} is not supported")
        endif()
    endif()

    add_executable("${target_name}" ${add_amber_test_SOURCES})

    if(DEFINED add_amber_test_COMPILE_OPTIONS)
        target_compile_options("${target_name}"
            PRIVATE ${add_amber_test_COMPILE_OPTIONS})
    endif()

    if(DEFINED add_amber_test_COMPILE_FEATURES)
        target_compile_features("${target_name}"
            PRIVATE ${add_amber_test_COMPILE_FEATURES})
    endif()

    if(DEFINED add_amber_test_LINK_LIBRARIES)
        target_link_libraries("${target_name}"
            PRIVATE ${add_amber_test_LINK_LIBRARIES})
    endif()

    if(DEFINED add_amber_test_LINK_OPTIONS)
        target_link_options("${target_name}"
            PRIVATE ${add_amber_test_LINK_OPTIONS})
    endif()

    add_test("${target_name}" "${target_name}")

endfunction()

