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

### End-to-end
If you want to test my cache by end-to-end, write this:
```
python3 run_tests.py [cache]
```
Cache field can be either `lfu` or `ideal`.

### Unit

If you want to run unit tests, generate Makefiles with the WITH_UNIT_TEST flag:
```
cmake [...] -DWITH_UNIT_TEST=1
```

Then build `unit_test` target:
```
cmake --build build --target unit_test
```

After that, run:
```
./build/tests/unit_test
```