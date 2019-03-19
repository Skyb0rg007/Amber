
find_package(cmocka CONFIG)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(cmocka CONFIG_MODE)

if(NOT TARGET cmocka::cmocka)
    add_library(cmocka::cmocka SHARED IMPORTED)
    set_target_properties(cmocka::cmocka PROPERTIES
        IMPORTED_LOCATION "${CMOCKA_LIBRARY}")
    target_include_directories(cmocka::cmocka INTERFACE
        "${CMOCKA_INCLUDE_DIR}"
        "${PROJECT_SOURCE_DIR}/thirdparty/cmocka/include/") # Amber wrapper
endif()
