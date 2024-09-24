#include "lfu_cache.hpp"
#include <iostream>
#include <vector>
#include <cassert>

namespace {
    int slow_get_page(int key)
    {
        return key;
    }

    size_t get_hits(size_t num_requests, caches::LFUCache<int> cache)
    {
        size_t hits = 0;

        for (size_t i = 0; i < num_requests; i++)
        {
            int elem = 0;
            std::cin >> elem;
            assert(std::cin.good());

            if (cache.lookup_update(elem, slow_get_page))
                hits++;
        }

        return hits;
    }

} // namespace

int main()
{
    size_t cache_size = 0;
    size_t num_requests = 0;

    std::cin >> cache_size >> num_requests;
    assert(std::cin.good());

    caches::LFUCache<int> lfu_cache{cache_size};

    size_t lfu_hits = get_hits(num_requests, lfu_cache);
    std::cout << lfu_hits << std::endl;

    return 0;
}