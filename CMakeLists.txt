cmake_minimum_required(VERSION 3.11)
project(my_cache)

message("Build project by cmake ...\n")

option(WITH_UNIT_TESTS "unit tests" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_subdirectory(src)

if (WITH_UNIT_TESTS)
    include(FetchContent)

    FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )
    
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    message("Build binary file for UNIT tests ...")
    add_subdirectory(tests)
endif()