
include(CheckCCompilerFlag)

function(enable_warnings target warnings)

    separate_arguments(compiler_flags UNIX_COMMAND "${warnings}")

    set(flags)

    foreach(flag IN LISTS compiler_flags)

        string(MAKE_C_IDENTIFIER "${target}_${flag}" varname)

        check_c_compiler_flag("${flag}" "${varname}")
        if("${${varname}}")
            set(flags "${flags} ${flag}")
        endif()

    endforeach()

    target_compile_options("${target}" PRIVATE "SHELL: ${flags}")

endfunction()


