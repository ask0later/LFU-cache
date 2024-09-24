#include <list>
#include <unordered_map>
#include <iterator>
#include <vector>
#include <iostream>

#pragma once

namespace caches
{
    template <typename T, typename KeyT = int>
    class LRUCache
    {
        using ListElem_t = typename std::pair<KeyT, T>;
        using ListIt     = typename std::list<ListElem_t>::iterator;

        const size_t                        size_;
        std::list<ListElem_t>               cache_;
        std::unordered_map<KeyT, ListIt>    hash_map_;
        
        bool full() const
        {
            return cache_.size() == size_;
        }

public:
        LRUCache(const size_t size) : size_(size) {}

        template <typename F>
        bool lookup_update(KeyT request_key, F slow_get_page) 
        { 
            auto hit = hash_map_.find(request_key);

            if (hit == hash_map_.end())
            {
                if (full())
                {
                    hash_map_.erase(cache_.back().first);
                    cache_.pop_back();
                }

                cache_.emplace_front(request_key, slow_get_page(request_key));
                hash_map_.emplace(request_key, cache_.begin());

                return false;
            }

            auto list_it = hit->second;
            if (list_it != cache_.begin())
                cache_.splice(cache_.begin(), cache_, list_it, std::next(list_it));
            
            return true;
        }

        void print() const
        {
            std::cout << "Cache:" << std::endl << "keys:";
            for (auto hash_elem : hash_map_)
            {
                std::cout << " " << hash_elem.first;
            }
            std::cout << std::endl << std::endl;
        }

    };
} // namespace caches