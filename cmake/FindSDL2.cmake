
find_library(SDL2_LIBRARY 
    NAMES SDL2)
if(SDL2_LIBRARY MATCHES ".*\.framework")
    set(SDL2_LIBRARY "${SDL2_LIBRARY}/SDL2")
endif()
find_library(SDL2_MAIN_LIBRARY
    NAMES SDL2main)
find_path(SDL2_INCLUDE_DIR
    NAMES SDL_haptic.h
    PATH_SUFFIXES SDL2)

if(SDL2_INCLUDE_DIR AND EXISTS "${SDL2_INCLUDE_DIR}/SDL_version.h")
    file(STRINGS "${SDL2_INCLUDE_DIR}/SDL_version.h" SDL_VERSION_MAJOR_LINE REGEX "^#define[ \t]+SDL_MAJOR_VERSION[ \t]+[0-9]+$")
    file(STRINGS "${SDL2_INCLUDE_DIR}/SDL_version.h" SDL_VERSION_MINOR_LINE REGEX "^#define[ \t]+SDL_MINOR_VERSION[ \t]+[0-9]+$")
    file(STRINGS "${SDL2_INCLUDE_DIR}/SDL_version.h" SDL_VERSION_PATCH_LINE REGEX "^#define[ \t]+SDL_PATCHLEVEL[ \t]+[0-9]+$")
    string(REGEX REPLACE "^#define[ \t]+SDL_MAJOR_VERSION[ \t]+([0-9]+)$" "\\1" SDL_VERSION_MAJOR "${SDL_VERSION_MAJOR_LINE}")
    string(REGEX REPLACE "^#define[ \t]+SDL_MINOR_VERSION[ \t]+([0-9]+)$" "\\1" SDL_VERSION_MINOR "${SDL_VERSION_MINOR_LINE}")
    string(REGEX REPLACE "^#define[ \t]+SDL_PATCHLEVEL[ \t]+([0-9]+)$" "\\1" SDL_VERSION_PATCH "${SDL_VERSION_PATCH_LINE}")
    set(SDL_VERSION_STRING ${SDL_VERSION_MAJOR}.${SDL_VERSION_MINOR}.${SDL_VERSION_PATCH})
    unset(SDL_VERSION_MAJOR_LINE)
    unset(SDL_VERSION_MINOR_LINE)
    unset(SDL_VERSION_PATCH_LINE)
    unset(SDL_VERSION_MAJOR)
    unset(SDL_VERSION_MINOR)
    unset(SDL_VERSION_PATCH)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDL2
    REQUIRED_VARS SDL2_INCLUDE_DIR SDL2_LIBRARY
    VERSION_VAR SDL2_VERSION_STRING)
mark_as_advanced(SDL2_INCLUDE_DIR SDL2_LIBRARY SDL2_MAIN_LIBRARY SDL2_VERSION_STRING)

if(NOT TARGET SDL2::SDL2)
    add_library(SDL2::SDL2 UNKNOWN IMPORTED)
    set_target_properties(SDL2::SDL2 PROPERTIES
        IMPORTED_LOCATION "${SDL2_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_DIR}")
    if(MINGW)
        target_compile_options(SDL2::SDL2 INTERFACE "-mwindows")
    elseif(APPLE)
        target_compile_options(SDL2::SDL2 INTERFACE "-framework Cocoa")
    endif()

    if(SDL2_MAIN_LIBARARY)
        add_library(SDL2::SDL2main UNKNOWN IMPORTED)
        set_target_properties(SDL2::SDL2main PROPERTIES
            IMPORTED_LOCATION "${SDL2_MAIN_LIBRARY}")
    else()
        add_library(SDL2::SDL2main INTERFACE IMPORTED)
    endif()
    target_link_libraries(SDL2::SDL2main INTERFACE SDL2::SDL2)
endif()
