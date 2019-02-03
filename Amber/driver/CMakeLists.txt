
add_executable(driver WIN32 MACOSX_BUNDLE
    src/main.c)
target_link_libraries(driver
    PRIVATE SDL2::SDL2 Amber::graphics Amber::ECS)
target_include_directories(driver
    PRIVATE include/)
target_compile_features(driver
    PRIVATE c_std_99)

add_executable(Amber::driver ALIAS driver)

install(TARGETS driver EXPORT amber-targets
    BUNDLE DESTINATION . COMPONENT Runtime
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime)
