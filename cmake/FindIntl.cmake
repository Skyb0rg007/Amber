
find_path(Intl_INCLUDE_DIR libintl.h)

find_library(Intl_LIBRARY intl)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Intl
    DEFAULT_MSG
    Intl_INCLUDE_DIR)

if(NOT TARGET gettext::intl)
    add_library(gettext::intl INTERFACE IMPORTED GLOBAL)
    target_include_directories(gettext::intl
        INTERFACE $<BUILD_INTERFACE:${Intl_INCLUDE_DIR}>)
    if(Intl_LIBRARY)
        set_target_properties(gettext::intl PROPERTIES
            IMPORTED_LOCATION ${Intl_LIBRARY})
    endif()
endif()
