
find_path(READLINE_INCLUDE_DIR
    NAMES readline/readline.h)

find_library(READLINE_LIBRARY
    NAMES readline)

find_library(NCURSES_LIBRARY
    NAMES ncurses)

mark_as_advanced(READLINE_INCLUDE_DIR READLINE_LIBRARY NCURSES_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Readline DEFAULT_MSG
    READLINE_INCLUDE_DIR READLINE_LIBRARY)

if(NOT TARGET readline::readline)
    add_library(readline::readline UNKNOWN IMPORTED)
    set_target_properties(readline::readline PROPERTIES
        IMPORTED_LOCATION ${READLINE_LIBRARY}
        INTERFACE_INCLUDE_DIRECTORIES ${READLINE_INCLUDE_DIR}
        INTERFACE_LINK_LIBRARIES ${NCURSES_LIBRARY})
endif()
