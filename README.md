# Amber
## A rendering engine with unrealistic goals

The project was named after [The Cherno Project](https://www.youtube.com/user/TheChernoProject)'s
Game Engine series. He definitely has a good idea on how to design a game engine,
but I think focusing on working with C and Lua would lend for a better intro to
game engine design.

Also SDL2 > GLFW don't @me.

# Building
When this library is functional, I'll provide binary releases for simplicity.
If you want to clone and build yourself though, here are the hoops:

This project relies on [SDL2](https://www.libsdl.org/) being installed.
You will need the development library from the site's [download page](https://www.libsdl.org/download-2.0.php)

To build tests, this project also requires installation of [cmocka](https://cmocka.org/).
The project automatically disables tests which use mocking when not available.
Download the project from [here](https://cmocka.org/files/1.1/)


Building:

    $ mkdir build && cd build
    $ ###### Unix ########
    $ # For normal Makefiles, however using Ninja may improve compile-time
    $ cmake .. -DCMAKE_BUILD_TYPE=Release
    $ cmake --build .
    $ ###### Windows - MinGW #######
    $ # if using cmd.exe with MinGW compiler
    $ cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
    $ # if using MSYS2 with MinGW compiler (recommended)
    $ cmake .. -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Release
    $ cmake --build .
    $ ###### Windows - MSVC (sorry) #######
    $ cmake .. -G "Visual Studio 15 Win64"       # Omit 'Win64' for 32-bit
    $ cmake --build . --config Release

Building docs:

    $ cmake --build . --target docs # For C documentation - build/docs/html/index.html
    $ cmake --build . --target ldoc # For Lua documentation - build/ldoc/index.html

Installation:

    $ cmake .. -DCMAKE_INSTALL_PREFIX=... # Where to install
    $ # For a blanket install (dev + runtime)
    $ cmake --build . --target install
    $ # For just Runtime tools
    $ cmake -DCMAKE_INSTALL_COMPONENT=Runtime -P cmake_install.cmake

Testing: (ensure you have [cmocka](https://cmocka.org/) installed)

    $ cmake .. -DBUILD_TESTING=ON      # Testing defaults to OFF
    $ ctest --output-on-failure                     # Run all tests
    $ ctest --output-on-failure -R Amber.util       # Run the Amber::util tests
    $ ctest --output-on-failure -L basic            # Run basic tests

Packaging:

    $ cpack -G "..."   # The desired package to create

