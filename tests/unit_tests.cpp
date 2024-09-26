#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>

#include "ideal_cache.hpp"
#include "lfu_cache.hpp"
#include "lru_cache.hpp"

struct T1
{
    char* string;
    double data;
    int key;
};

T1 slow_get_page1(int key)
{
    T1 a;
    a.key = key;

    return a;
}

TEST(LFU_LOOKUP, test_1)
{
    size_t cache_size = 4;

    std::vector<int> requests;

    requests.push_back(1);
    requests.push_back(2);
    requests.push_back(3);
    requests.push_back(4);

    caches::LFUCache<T1> lfu_cache{cache_size};
    
    size_t hits = 0;
    
    for (size_t i = 0; i < 4; i++)
    {
        T1 target_data = lfu_cache.lookup(requests[i], slow_get_page1);
        ASSERT_EQ(requests[i], target_data.key);
    }
}

TEST(LFU_LOOKUP, test_2)
{
    size_t cache_size = 3;

    std::vector<int> requests;

    requests.push_back(1);
    requests.push_back(2);
    requests.push_back(3);
    requests.push_back(4);
    requests.push_back(4);
    requests.push_back(4);
    requests.push_back(2);
    requests.push_back(1);

    caches::LFUCache<T1> lfu_cache{cache_size};
    
    size_t hits = 0;
    
    for (size_t i = 0; i < 8; i++)
    {
        T1 target_data = lfu_cache.lookup(requests[i], slow_get_page1);
        ASSERT_EQ(requests[i], target_data.key);
    }
}


struct T2
{
    char a;
    double data;
    int key;
};

T2 slow_get_page2(int key)
{
    T2 a;
    a.key = key * 17 + 12;

    return a;
}


TEST(LFU_LOOKUP, test_3)
{
    size_t cache_size = 3;

    std::vector<int> requests;

    requests.push_back(1);
    requests.push_back(3);
    requests.push_back(4);
    requests.push_back(5);
    requests.push_back(6);
    requests.push_back(7);
    requests.push_back(8);
    requests.push_back(9);

    caches::LFUCache<T2> lfu_cache{cache_size};
    
    size_t hits = 0;
    
    for (size_t i = 0; i < 8; i++)
    {
        T2 target_data = lfu_cache.lookup(requests[i], slow_get_page2);
        ASSERT_EQ(requests[i] * 17 + 12, target_data.key);
    }
}



TEST(LFU_LOOKUP_UPDATE, test_1)
{
    size_t cache_size = 3;

    std::vector<int> requests;

    requests.push_back(1);
    requests.push_back(2);
    requests.push_back(3);
    requests.push_back(2);
    requests.push_back(3);
    requests.push_back(2);
    requests.push_back(3);

    caches::LFUCache<T1> lfu_cache{cache_size};
    
    size_t hits = 0;
    
    for (size_t i = 0; i < 7; i++)
    {
        lfu_cache.lookup(requests[i], slow_get_page1);
    }

    ASSERT_EQ(lfu_cache.lookup_update(4, slow_get_page1), false);
    ASSERT_EQ(lfu_cache.lookup_update(1, slow_get_page1), false);
}