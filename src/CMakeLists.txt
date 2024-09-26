set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_library(cache_lib INTERFACE)
target_include_directories(cache_lib INTERFACE ${INCLUDE_DIR})

add_executable(lfu_cache   lfu_cache.cpp)
add_executable(ideal_cache ideal_cache.cpp)

target_link_libraries(lfu_cache   cache_lib)
target_link_libraries(ideal_cache cache_lib)