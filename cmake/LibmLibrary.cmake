include(CheckSymbolExists)

set(CMAKE_REQUIRED_LIBRARIES)
check_symbol_exists(pow math.h POW_FUNC_EXISTS)

if(POW_FUNC_EXISTS)
    set(LIBM_LIBRARY)
else()
    set(CMAKE_REQUIRED_LIBRARIES "m")
    check_symbol_exists(pow math.h POW_FUNC_EXISTS_LIBM)
    if(NOT POW_FUNC_EXISTS_LIBM)
        message(FATAL_ERROR "Unable to locate the pow() function")
    endif()

    set(LIBM_LIBRARY "m")
endif()
