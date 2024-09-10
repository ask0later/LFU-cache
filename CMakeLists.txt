cmake_minimum_required(VERSION 3.11)
project(my_cache)

message("Build project by cmake ...\n")

set(CMAKE_CXX_FLAGS_DEBUG "-Wall -g -O0 -fsanitize=address -fsanitize=undefined")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_subdirectory(src)


# cmake [<options>] -B <path-to-build> [-S <path-to-source>]


# Build a project
# cmake --build <dir>
# ./build/src/cache