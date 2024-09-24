#include <list>
#include <unordered_map>
#include <iterator>
#include <vector>
#include <iostream>

#pragma once

namespace caches
{
    template <typename T, typename KeyT = int>
    class LFUCache
    {
        using ListElem_t    = typename std::tuple<KeyT, T, size_t>;
        using ListIt        = typename std::list<ListElem_t>::iterator;

        const size_t                        size_;
        std::list<ListElem_t>               cache_;
        std::unordered_map<KeyT, ListIt>    hash_map_;

        static KeyT get_key(ListIt it)
        {
            return std::get<0>(*it);
        }

        static size_t get_count(ListIt it)
        {
            return std::get<2>(*it);
        }

        static void set_count(ListIt it, const size_t count)
        {
            std::get<2>(*it) = count;
        }

        bool full() const
        {
            return cache_.size() == size_;
        }

        void process_hash_hit(ListIt list_it)
        {
            auto next_it = std::next(list_it);

            if (next_it != cache_.begin() && next_it != cache_.end())
                if (get_count(list_it) > get_count(next_it))
                    cache_.splice(list_it, cache_, next_it, std::next(next_it));

            return;
        }

        template <typename F> 
        void process_hash_miss(KeyT request_key, F slow_get_page)
        {
            if (full())
            {
                hash_map_.erase(get_key(cache_.begin()));
                cache_.pop_front();
            }

            cache_.emplace_front(request_key, slow_get_page(request_key), 1);
            hash_map_.emplace(request_key, cache_.begin());

            return;
        }
        
public:
        LFUCache(const size_t size) : size_(size) {}

        template <typename F>
        bool lookup_update(KeyT request_key, F slow_get_page)
        {
            auto hit = hash_map_.find(request_key);

            if (hit == hash_map_.end())
            {
                process_hash_miss(request_key, slow_get_page);
                return false;
            }

            auto list_it = hit->second;
            set_count(list_it, get_count(list_it) + 1);

            process_hash_hit(list_it);

            return true;
        }

        void print() const
        {
            std::cout << "Hash map:" << std::endl << "keys:";
            
            for (auto hash_elem : hash_map_)
                std::cout << " " << hash_elem.first;

            std::cout << std::endl;
            std::cout << "List(Cache):" << std::endl << "keys\tcounter" << std::endl;

            for (auto list_elem : cache_)
                std::cout << get_key(list_elem) << "\t" << get_count(list_elem) << std::endl;

        }
    };
} // namespace caches