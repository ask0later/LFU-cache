cmake_minimum_required(VERSION 3.11)
project(my_cache)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_executable(lfu_cache lfu_cache.cpp)
target_include_directories(lfu_cache PUBLIC ${INCLUDE_DIR})

add_executable(ideal_cache ideal_cache.cpp)
target_include_directories(ideal_cache PUBLIC ${INCLUDE_DIR})