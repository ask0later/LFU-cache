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
    
    std::list<int> request_list;
    for (auto vector_elem : pair.second)
    {
        request_list.push_back(vector_elem);
    }

    caches::IdealCache<int, std::function<int(int)>> ideal_cache{pair.first, request_list};

    size_t ideal_hits = ideal_cache.get_hits(pair.second, slow_get_page);

    std::cout << ideal_hits << std::endl;

    return 0;
}