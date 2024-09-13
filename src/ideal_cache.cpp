#include "cache.hpp"
#include <iostream>

int slow_get_page(int key);

using func_t = int(int);

int main()
{
    size_t cache_size = 0, request_nums = 0, ideal_hits = 0;

    std::cin >> cache_size >> request_nums;

    int key = 0;
    std::list<int> request_list;

    for (size_t i = 0; i < request_nums; i++)
    {
        std::cin >> key;
        request_list.push_back(key);
    }

    caches::IdealCache<int, int(int)> ideal_cache{cache_size, request_list};
    
    for (auto it = request_list.begin(); it != request_list.end(); it = std::next(it))
    {
        if (ideal_cache.lookup_update(*it, slow_get_page))
            ideal_hits++;
    }

    std::cout << ideal_hits << std::endl;

    return 0;
}

int slow_get_page(int key)
{
    return key;
}