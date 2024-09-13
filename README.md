# LFU Cache

## Introduction

This project is an implementation of the LFU Cache used by std::unordered_map, std::list.

## Build and Run

If you want to build and run the project, write this after copying the project:
```
cmake -S . -B build
cmake --build build
./build/src/cache
```


## Test

If you want to test my cache, write this:
```
python3 run_tests.py [cache]
```
Cache field can be either `lfu` or `ideal`.


