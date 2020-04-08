
set(SSBASE_LIBRARY_FIND_PATH "" CACHE STRING "Where to find the required libraries")
if (NOT SSBASE_LIBRARY_FIND_PATH)
    set(SSBASE_LIBRARY_FIND_PATH
            ${CMAKE_CURRENT_LIST_DIR}/thirdparty/lib64
            ${CMAKE_CURRENT_LIST_DIR}/thirdparty/lib
            /usr/local/lib64
            /usr/local/lib
            /usr/lib64
            /usr/lib
            )
    message("SSBASE_LIBRARY_FIND_PATH is not set, use default: ${SSBASE_LIBRARY_FIND_PATH}")
endif()

set(SSBASE_PATH_FIND_PATH "" CACHE STRING "Where to find the required header files")
if (NOT SSBASE_PATH_FIND_PATH)
    set(SSBASE_PATH_FIND_PATH
            ${CMAKE_CURRENT_LIST_DIR}/thirdparty/include
            /usr/local/include
            /usr/include
            )
    message("SSBASE_PATH_FIND_PATH is not set, use default: ${SSBASE_PATH_FIND_PATH}")
endif()

function(SSBaseFindLibrary output libName isRequired)

#    if (CMAKE_SYSTEM_NAME MATCHES "Linux")
#        MESSAGE(STATUS "current platform: Linux ")
#    elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
#        MESSAGE(STATUS "current platform: Windows")
#    elseif (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
#        MESSAGE(STATUS "current platform: FreeBSD")
#    else ()
#        MESSAGE(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")
#    endif ()

    if (WIN32)
        set(PLATFORM_LIB_NAME lib${libName}.dll ${libName}.dll
                lib${libName}.lib ${libName}.lib
                lib${libName}.a ${libName}.a)
    elseif (APPLE)
        set(PLATFORM_LIB_NAME lib${libName}.dylib lib${libName}.a)
    else ()
        set(PLATFORM_LIB_NAME lib${libName}.so lib${libName}.a)
    endif ()

    find_library(${output}
            NAMES ${PLATFORM_LIB_NAME}
            PATHS ${SSBASE_LIBRARY_FIND_PATH}
            NO_DEFAULT_PATH
            )
    if (${output})
        message("Found ${libName}: ${${output}}")
    else()
        if (isRequired)
            message(FATAL_ERROR "Library ${libName} Not found!")
        endif()
        message("Library ${libName} Not found!")
    endif()
endfunction(SSBaseFindLibrary)

# NOTE: output will be cached
function(SSBaseFindPath output fileName isRequired)
    find_path(${output}
            NAMES ${fileName}
            PATHS ${SSBASE_PATH_FIND_PATH}
            NO_DEFAULT_PATH
            )
    if (${output})
        message("Found ${fileName} under: ${${output}}")
    else()
        if (${isRequired})
            message(FATAL_ERROR "File ${fileName} Not found!")
        endif()
        message("File ${fileName} Not found!")
    endif()

endfunction(SSBaseFindPath)


function(SSBaseAddLinkLibraryAndIncludePath target libVars)
    foreach(libVar ${libVars})
        target_link_libraries(${target} ${${libVar}_LIB})
        target_include_directories(${target} PRIVATE ${${libVar}_INCLUDE})
    endforeach()
endfunction()