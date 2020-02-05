project(SSIO)

file(GLOB_RECURSE SSIO_SOURCE_FILES * thirdparty/*)

add_library(SSIO ${SSIO_SOURCE_FILES})
target_include_directories(SSIO PRIVATE .. ../../thirdparty/include)
add_dependencies(SSIO SSBase)
