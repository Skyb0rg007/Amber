
if [[ ${1,,} = mingw ]]; then
    echo "Installing 7zip"
    choco install 7zip.portable

    echo "Installing SDL2"
    pushd /tmp/sdl2/ || exit 1
    wget https://www.libsdl.org/release/SDL2-devel-2.0.9-mingw.tar.gz
    tar -xzf SDL2-devel-2.0.9-mingw.tar.gz SDL2-2.0.9/
    SDL2_ROOT="$PWD/SDL2-2.0.9/"
    ls -lt
    popd

    echo "Installing cmocka"
    mkdir /tmp/cmocka/
    pushd /tmp/cmocka/ || exit 1
    wget https://cmocka.org/files/1.1/cmocka-1.1.3.tar.xz
    ls -lt
    7z e cmocka-1.1.3.tar.xz && 7z x cmocka-1.1.3.tar
    ls -lt
    cd cmocka-1.1.3/ || exit 1
    mkdir build
    cd build || exit 1
    cmake .. -DCMAKE_BUILD_TYPE="$CONFIG" \
        -DCMAKE_INSTALL_PREFIX=/tmp/cmocka/install/ \
        "${CMAKE_OPTS[@]}"
    cmake --build . --parallel
    cmake --build . --target install
    CMOCKA_ROOT="/tmp/cmocka/install/"
    popd

    echo "Setting up CMake"
    export CMAKE=cmake CTEST=ctest CPACK=cpack
    CMAKE_OPTS+=("-DCMAKE_PREFIX_PATH=$SDL2_ROOT;$CMOCKA_ROOT")
fi
