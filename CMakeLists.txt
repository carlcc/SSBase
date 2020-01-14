cmake_minimum_required(VERSION 3.2)

project(SSBase)

set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE SSBASE_SOURCE_FILES src/*)

add_library(SSBase ${SSBASE_SOURCE_FILES})

add_executable(testmain tests/testmain.cpp)
target_link_libraries(testmain SSBase)
target_include_directories(testmain PRIVATE src)