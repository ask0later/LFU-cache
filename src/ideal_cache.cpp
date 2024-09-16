#include "ideal_cache.hpp"
#include <iostream>
#include <functional>
#include <vector>

namespace
{
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
} // namespace


int main()
{
    auto [cache_size, requests_vector] = get_input(std::cin);
    std::list<int> request_list(requests_vector.begin(), requests_vector.end());

    caches::IdealCache<int> ideal_cache{cache_size, request_list};
    size_t ideal_hits = ideal_cache.get_hits(requests_vector, slow_get_page);

    std::cout << ideal_hits << std::endl;

    return 0;
}