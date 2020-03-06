#!/bin/bash

THIRDPARTY_ROOT="$(pwd)"

#echo "== Building zstd..."
#cd "${THIRDPARTY_ROOT}" || exit
#tar xfp zstd-1.4.4.tar.gz
#cd zstd-1.4.4/build/cmake || exit
#mkdir build
#cd build || exit
#cmake .. -DCMAKE_INSTALL_PREFIX="${THIRDPARTY_ROOT}" -DZSTD_BUILD_PROGRAMS=OFF -DZSTD_BUILD_SHARED=OFF
#make -j 4 && make install

#echo "== Building lz4..."
#cd "${THIRDPARTY_ROOT}" || exit
#tar xfp lz4-1.9.2.tar.gz
#cd lz4-1.9.2 || exit
## shellcheck disable=SC2016
#echo 'cmake_minimum_required(VERSION 3.2)
#
#project(lz4)
#
#file(GLOB LZ4_SOURCE_FILES lib/*.c lib/*.h)
#add_library(lz4 STATIC ${LZ4_SOURCE_FILES})
#
#install(TARGETS lz4
#       RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#       LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#       ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#       )
#install(FILES lib/lz4.h lib/lz4frame.h lib/lz4frame_static.h lib/lz4hc.h
#       TYPE INCLUDE)
#' > CMakeLists.txt
#mkdir build
#cd build || exit
#cmake .. -DCMAKE_INSTALL_PREFIX="${THIRDPARTY_ROOT}"
#make -j 4 && make install || exit

echo "== Building zlib..."
cd "${THIRDPARTY_ROOT}" || exit
tar xfp zlib-1.2.11.tar.gz
cd zlib-1.2.11 || exit
mkdir build
cd build || exit
cmake .. -DCMAKE_INSTALL_PREFIX="${THIRDPARTY_ROOT}"
make -j 4 && make install

echo "== Building libzip..."
cd "${THIRDPARTY_ROOT}" || exit
tar xfp libzip-1.6.1.tar.gz
cd libzip-1.6.1/ || exit
mkdir build
cd build || exit
# no cryption, only deflate
cmake .. -DENABLE_COMMONCRYPTO=OFF \
         -DENABLE_GNUTLS=OFF \
         -DENABLE_MBEDTLS=OFF \
         -DENABLE_OPENSSL=OFF \
         -DENABLE_WINDOWS_CRYPTO=OFF \
         -DENABLE_BZIP2=OFF \
         -DENABLE_LZMA=OFF \
         -DBUILD_TOOLS=OFF \
         -DBUILD_REGRESS=OFF \
         -DBUILD_EXAMPLES=OFF \
         -DBUILD_DOC=OFF \
         -DCMAKE_FIND_ROOT_PATH="${THIRDPARTY_ROOT}/lib" \
         -DCMAKE_INSTALL_PREFIX="${THIRDPARTY_ROOT}"
make -j 4 && make install

echo "== Building libuv..."
cd "${THIRDPARTY_ROOT}" || exit
tar xfp libuv-1.34.2.tar.gz
cd libuv-1.34.2 || exit
mkdir build
cd build || exit
cmake .. -DLIBUV_BUILD_TESTS=OFF \
         -DCMAKE_FIND_ROOT_PATH="${THIRDPARTY_ROOT}/lib" \
         -DCMAKE_INSTALL_PREFIX="${THIRDPARTY_ROOT}"
cmake -j 4 && make install
