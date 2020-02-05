#!/bin/bash

THIRDPARTY_ROOT="$(pwd)"

tar xfp zstd-1.4.4.tar.gz
cd zstd-1.4.4/build/cmake || exit
mkdir build
cd build || exit
cmake .. -DCMAKE_INSTALL_PREFIX="${THIRDPARTY_ROOT}" -DZSTD_BUILD_PROGRAMS=OFF -DZSTD_BUILD_SHARED=OFF
make -j 4 && make install

