#include "cache.hpp"
#include <iostream>

int slow_get_page(int key)
{
    std::cout << "foo\n";
    return key;
}

int main()
{
    size_t cache_size = 0, request_nums = 0, lru_hits = 0, lfu_hits = 0;

    std::cin >> cache_size >> request_nums;

    caches::LFUCache<int> cache{cache_size};

    int request_key = 0;
    for (size_t i = 0; i < request_nums; i++)
    {
        std::cin >> request_key;
        
        if (cache.lookup_update(request_key, slow_get_page))
            lfu_hits++;
    }
    
    std::cout << "LFU hits = " << lfu_hits << "/" << request_nums << std::endl;

    return 0;
}