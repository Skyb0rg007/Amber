cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(LDoc
    VERSION 1.4.3
    HOMEPAGE_URL "http://stevedonovan.github.io/ldoc/"
    DESCRIPTION "a documentation generator for Lua"
    LANGUAGES NONE)

# The LDoc script itself
set(LDOC_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/bin/ldoc.lua" CACHE INTERNAL "")
set(LDOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/?.lua" CACHE INTERNAL "")

if(NOT TARGET lua::lua)
    message(FATAL_ERROR "LDoc needs the lua::lua imported target!")
endif()

# LDoc dependencies
add_subdirectory(dependencies/luafilesystem/)
add_subdirectory(dependencies/penlight/)

function(ldoc_config_settings output_dir)
    set(config_file "${output_dir}/config.ld")

    # header + clear file
    file(WRITE "${config_file}"
        "-- This is a generated file - do not edit!\n\n\n")

    # dir
    file(APPEND "${config_file}" "dir = '${output_dir}'\n\n")

    # file
    file(APPEND "${config_file}" "file = {\n")
    foreach(file IN LISTS LDOC_UNPARSED_ARGUMENTS)
        if(NOT IS_ABSOLUTE file)
            set(file "${working_dir}/${file}")
        endif()
        # file(RELATIVE_PATH relative_file "${output_dir}" "${file}")
        set(relative_file "${file}")
        file(APPEND "${config_file}" "\t'${relative_file}',\n")
    endforeach()
    file(APPEND "${config_file}" "}\n\n")

    # project
    file(APPEND "${config_file}" "project = '${PROJECT_NAME}'\n\n")

endfunction()

function(ldoc_add_docs target_name) 
    set(options "ALL")
    set(single "WORKING_DIRECTORY" "COMMENT" "OUTPUT_DIRECTORY")
    set(multi "")
    cmake_parse_arguments(PARSE_ARGV 1 LDOC "${options}" "${single}" "${multi}")

    # Setting variables
    # output_dir - must be unique!
    # comment
    # working_dir

    if(DEFINED LDOC_OUTPUT_DIRECTORY)
        set(output_dir "${LDOC_OUTPUT_DIRECTORY}")
    else()
        set(output_dir "${CMAKE_CURRENT_BINARY_DIR}/${target_name}")
    endif()

    if(NOT DEFINED LDOC_COMMENT)
        set(comment "Building LDoc documentation")
    else()
        set(comment "${LDOC_COMMENT}")
    endif()

    if(NOT DEFINED LDOC_WORKING_DIRECTORY)
        set(working_dir "${CMAKE_CURRENT_SOURCE_DIR}")
    else()
        set(working_dir "${LDOC_WORKING_DIRECTORY}")
    endif()

    ldoc_config_settings("${output_dir}")

    # Setup package.(c)path to find luafilesystem and penlight, then call ldoc
    add_custom_target("${target_name}"
        COMMAND lua::lua -E
        -e "package.cpath = '$<TARGET_FILE_DIR:lua::luafilesystem>/?.so;'"
        -e "package.path = '${LUA_PENLIGHT_PATH};${LDOC_PATH}'"
        "${LDOC_SCRIPT}" --not_luadoc .

        VERBATIM
        WORKING_DIRECTORY "${output_dir}"
        COMMENT "${comment}"
        )
    add_dependencies("${target_name}" lua::lua lua::luafilesystem)

endfunction()
