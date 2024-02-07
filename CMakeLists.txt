cmake_minimum_required(VERSION 3.27)
project(DiskUsageAnalyzer C)

set(CMAKE_C_STANDARD 23)

if (MSVC)
    # warning level 4
    add_compile_options(/W4)
else()
    # additional warnings
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_executable(DiskUsageAnalyzer src/main.c
        src/numformat.h
        src/directorytree.h
        src/types.h)
