#pragma once 

#include <list>
#include <tuple>
#include <unordered_map>
#include <cstdlib>
#include <iostream>

namespace caches {

template <typename T, typename KeyT = int> class LFUCache
{
private:
    size_t size_;
    std::list<std::tuple<KeyT, T, size_t>> cache_;

    using ListIt = typename std::list<std::tuple<KeyT, T, size_t>>::iterator;
    std::unordered_map<KeyT, ListIt> hash_map_;

    bool full() const {return cache_.size() == size_;}

public:
    LFUCache(size_t size) : size_(size) {}

    template <typename F> bool lookup_update(KeyT key, F slow_get_page)
    {
        auto hit = hash_map_.find(key);

        if (hit == hash_map_.end()) // key not found
        {
            if (full())
            {
                // LFU algoritm

                // find min
                auto it = cache_.begin();
                auto min_it = it;

                size_t min_counter = std::get<2>(*it);
                for (; it != cache_.end(); it = std::next(it))
                {
                    size_t counter = std::get<2>(*it);
                    if (min_counter > counter)
                    {
                        min_counter = counter;
                        min_it = it;
                    }
                }
            
                hash_map_.erase(std::get<0>(*min_it));
                cache_.erase(min_it);
            }
            
            cache_.emplace_front(key, slow_get_page(key), 1);
            hash_map_.emplace(key, cache_.begin());

            return false;
        }
        

        // key found
        auto list_it = hit->second;
        std::get<2>(*list_it) += 1;
    
        if (list_it != cache_.begin())
        {
            cache_.splice(cache_.begin(), cache_, list_it, std::next(list_it));
        }

        return true;
    }
};

} // namespace caches