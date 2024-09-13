# LFU Cache

## Introduction

This project is an implementation of the LFU Cache used by std::unordered_map, std::list.

## Build and Run

If you want to build the project, write this in the project directory:
```
cmake -S . -B build
cmake --build build
```

After that, you can run with LRU cache or Ideal cache program:

```
./build/src/lfu_cache
```
or
```
./build/src/ideal_cache
```

## Test

If you want to test my cache, write this:
```
python3 run_tests.py [cache]
```
Cache field can be either `lfu` or `ideal`.


