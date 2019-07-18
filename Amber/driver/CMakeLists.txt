
add_executable(driver WIN32 MACOSX_BUNDLE
    src/main.c)
target_link_libraries(driver
    PRIVATE SDL2::SDL2 Amber::graphics Amber::ECS)
target_include_directories(driver
    PRIVATE include/)
target_compile_features(driver
    PRIVATE c_std_99)
if(EMSCRIPTEN)
    target_link_options(driver
        PRIVATE "SHELL:-s ERROR_ON_UNDEFINED_SYMBOLS=0 --emrun")
    set_target_properties(driver PROPERTIES
        SUFFIX .html)
endif()

add_executable(Amber::driver ALIAS driver)

install(TARGETS driver EXPORT amber-targets
    BUNDLE DESTINATION . COMPONENT Runtime
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime)
