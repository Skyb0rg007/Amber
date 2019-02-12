find_program(LEMON_EXECUTABLE
    NAMES lemon
    HINTS ENV LEMON_DIR
    DOC "Lemon executable"
)

find_file(LEMON_TEMPLATE
    NAMES lempar.c
    PATH_SUFFIXES lemon/ share/lemon/
    HINTS ENV LEMON_TEMPLATE
    DOC "Lemon template file"
)

if(LEMON_EXECUTABLE)
    execute_process(
        COMMAND "${LEMON_EXECUTABLE}" -x
        RESULT_VARIABLE lemon_valid
        OUTPUT_VARIABLE lemon_version_string)
    if(lemon_valid EQUAL 0)
        string(REPLACE "Lemon version " "" LEMON_VERSION "${lemon_version_string}")
        string(REPLACE "\n" "" LEMON_VERSION "${LEMON_VERSION}")
    else()
        message(FATAL_ERROR "Error calling '${LEMON_EXECUTABLE}'!")
    endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Lemon
    REQUIRED_VARS LEMON_EXECUTABLE LEMON_TEMPLATE
    VERSION_VAR LEMON_VERSION
)
mark_as_advanced(LEMON_EXECUTABLE LEMON_TEMPLATE LEMON_VERSION)

