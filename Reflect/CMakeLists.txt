cmake_minimum_required(VERSION 3.10)

project(ReflectLib)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(source_list
    #includes
    "inc/Reflect.h"
    "inc/ReflectStructs.h"

    "inc/Core/Compiler.h"
    "inc/core/Core.h"
    "inc/core/Enums.h"
    "inc/core/ReflectObject.h"
    "inc/core/Util.h"

    "inc/CodeGenerate/CodeGenerate.h"
    "inc/CodeGenerate/CodeGenerateHeader.h"
    "inc/CodeGenerate/CodeGenerateSource.h"

    "inc/FileParser/FileParser.h"

    #sources
    "src/core/Enums.cpp"
    "src/core/ReflectObject.cpp"
    "src/core/Util.cpp"

    "src/CodeGenerate/CodeGenerate.cpp"
    "src/CodeGenerate/CodeGenerateHeader.cpp"
    "src/CodeGenerate/CodeGenerateSource.cpp"

    "src/FileParser/FileParser.cpp"
    )

# This is a library
add_library(ReflectLib STATIC ${source_list})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

target_include_directories(ReflectLib PUBLIC inc)
target_include_directories(ReflectLib PRIVATE src)

function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${source_list})
#assign_source_group(${source_list})