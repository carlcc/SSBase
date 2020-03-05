project(SSIO)

file(GLOB SSIO_SOURCE_FILES file/* stream/* * thirdparty/*)

add_library(SSIO ${SSIO_SOURCE_FILES})
target_include_directories(SSIO PRIVATE .. ../../thirdparty/include)
add_dependencies(SSIO SSBase)
