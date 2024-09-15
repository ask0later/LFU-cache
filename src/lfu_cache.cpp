#include "cache.hpp"
#include <iostream>
#include <functional>

namespace {
    int slow_get_page(int key)
    {
        return key;
    }
} // namespace

int main()
{
    auto pair = caches::get_input(std::cin);

    caches::LFUCache<int, std::function<int(int)>> lfu_cache{pair.first};

    size_t lfu_hits = lfu_cache.get_hits(pair.second, slow_get_page);

    std::cout << lfu_hits << std::endl;

    return 0;
}