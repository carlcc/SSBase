cmake_minimum_required(VERSION 3.2)

project(SSBase)

set(CMAKE_CXX_STANDARD 14)

option(SSBASE_BUILD_TEST "Build the test program" OFF)

include(common.cmake)

SSBaseFindLibrary(UV_LIB "uv" True)
SSBaseFindPath(UV_INCLUDE "uv.h" True)
SSBaseFindLibrary(ZIP_LIB "zip" True)
SSBaseFindPath(ZIP_INCLUDE "zip.h" True)

if (WIN32)
    SSBaseFindLibrary(Z_LIB "zlib" True)
else ()
    SSBaseFindLibrary(Z_LIB "z" True)
endif ()


if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    MESSAGE(STATUS "current platform: Linux ")
    add_definitions(-DSS_PLATFORM_LINUX)
    add_definitions(-DSS_PLATFORM_UNIX)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    MESSAGE(STATUS "current platform: Windows")
    add_definitions(-DSS_PLATFORM_WIN32 -D_CRT_SECURE_NO_WARNINGS)
    # The default encoding is not utf-8, but our code is saved in utf-8
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
elseif (${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
    MESSAGE(STATUS "current platform: FreeBSD")
    add_definitions(-DSS_PLATFORM_UNIX)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    MESSAGE(STATUS "current platform: Darwin")
    add_definitions(-DSS_PLATFORM_UNIX)
    add_definitions(-DSS_PLATFORM_APPLE)
else ()
    MESSAGE(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")
endif ()

add_subdirectory(src/SSBase)
add_subdirectory(src/SSIO)
add_subdirectory(src/SSNet)

if (SSBASE_BUILD_TEST)
    add_subdirectory(tests)
endif()