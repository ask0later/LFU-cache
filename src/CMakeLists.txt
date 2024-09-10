cmake_minimum_required(VERSION 3.11)
project(my_cache)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_executable(cache cache.cpp)
target_include_directories(cache PUBLIC ${INCLUDE_DIR})