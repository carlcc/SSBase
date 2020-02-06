cmake_minimum_required(VERSION 3.2)

project(SSBase)

set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE SSBASE_SOURCE_FILES src/*)

add_subdirectory(src/SSBase)
add_subdirectory(src/SSIO)

add_executable(testmain tests/testmain.cpp)
add_dependencies(testmain SSBase SSIO)
target_link_directories(testmain PRIVATE thirdparty/lib)
target_link_libraries(testmain SSBase SSIO archive zip z)
target_include_directories(testmain PRIVATE src)