#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <string>

#include "ideal_cache.hpp"
#include "lfu_cache.hpp"
#include "lru_cache.hpp"

struct Data
{
    int key;
    std::string str;
    int data;
};

Data slow_get_page(int key)
{
    Data a;
    a.key = key;
    std::string tmp (key, '=');
    a.str = tmp;
    a.data = key * 2;

    return a;
}

TEST(LFU_LOOKUP, test_1)
{
    size_t cache_size = 4;

    std::vector<Data> requests;

    requests.push_back({1, "=",    2});
    requests.push_back({2, "==",   4});
    requests.push_back({3, "===",  6});
    requests.push_back({4, "====", 8});

    caches::LFUCache<Data> lfu_cache{cache_size};
    
    size_t hits = 0;
    
    for (size_t i = 0; i < 4; i++)
    {
        Data target_data = lfu_cache.lookup(requests[i].key, slow_get_page);
        ASSERT_EQ(requests[i].key, target_data.key);
        ASSERT_EQ(requests[i].data, target_data.data);
        ASSERT_EQ(requests[i].str, target_data.str);
    }
}

TEST(LFU_LOOKUP, test_2)
{
    size_t cache_size = 3;

    std::vector<Data> requests;

    requests.push_back({1, "=",     2});
    requests.push_back({2, "==",    4});
    requests.push_back({3, "===",   6});
    requests.push_back({4, "====",  8});
    requests.push_back({4, "====",  8});
    requests.push_back({4, "====",  8});
    requests.push_back({2, "==",    4});
    requests.push_back({1, "=",     2});
    requests.push_back({5, "=====", 10});
    requests.push_back({4, "====",  8});
    requests.push_back({2, "==",    4});
    requests.push_back({1, "=",     2});

    caches::LFUCache<Data> lfu_cache{cache_size};
    
    size_t hits = 0;
    
    for (size_t i = 0; i < 8; i++)
    {
        Data target_data = lfu_cache.lookup(requests[i].key, slow_get_page);
        ASSERT_EQ(requests[i].key, target_data.key);
        ASSERT_EQ(requests[i].data, target_data.data);
        ASSERT_EQ(requests[i].str, target_data.str);
    }
}