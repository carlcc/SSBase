project(SSNet)

file(GLOB SSNet_SOURCE_FILES * thirdparty/*)

add_library(SSNet ${SSNet_SOURCE_FILES})
target_include_directories(SSNet PRIVATE .. ../../thirdparty/include)
add_dependencies(SSNet SSBase SSIO)
