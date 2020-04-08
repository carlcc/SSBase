project(testmain)

file(GLOB SSBASE_TEST_SRC_FILES *)

add_executable(ssbase_test ${SSBASE_TEST_SRC_FILES})
add_dependencies(ssbase_test SSBase SSIO SSNet)

if (WIN32)
    set(SSBASE_PLATFORM_LIBS ws2_32)
else ()
    set(SSBASE_PLATFORM_LIBS pthread)
endif ()
target_link_libraries(ssbase_test SSBase SSIO SSNet ${UV_LIB} ${ZIP_LIB} ${Z_LIB} ${SSBASE_PLATFORM_LIBS})
target_include_directories(ssbase_test PRIVATE ../src ${UV_INCLUDE} ${ZIP_INCLUDE})

