#include "lfu_cache.hpp"
#include <iostream>
#include <functional>
#include <vector>

namespace {
    int slow_get_page(int key)
    {
        return key;
    }

    template <typename KeyT = int>
    std::pair<size_t, std::vector<KeyT>> get_input(std::istream &is)
    {
        size_t cache_size = 0, request_nums = 0;
        std::vector<KeyT> request_list{};

        is >> cache_size >> request_nums;
        
        KeyT request_key = 0;
        for (size_t i = 0; i < request_nums; i++)
        {
            is >> request_key;
            request_list.push_back(request_key);
        }

        std::pair<size_t, std::vector<KeyT>> pair{cache_size, request_list};

        return pair;
    }

    size_t get_hits(std::vector<int> request_list, caches::LFUCache<int> cache)
    {
        size_t hits = 0;

        for (auto vector_elem : request_list)
        {
            if (cache.lookup_update(vector_elem, slow_get_page))
                hits++;
        }

        return hits;
    }
} // namespace

int main()
{
    auto [cache_size, requests] = get_input(std::cin);

    caches::LFUCache<int> lfu_cache{cache_size};

    size_t lfu_hits = get_hits(requests, lfu_cache);

    std::cout << lfu_hits << std::endl;

    return 0;
}