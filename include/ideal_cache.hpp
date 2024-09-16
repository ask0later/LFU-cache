#include <list>
#include <unordered_map>
#include <iterator>
#include <vector>
#include <iostream>

#pragma once

namespace caches
{

    template <typename T, typename KeyT = int>
    class IdealCache
    {
        using ListElem_t = typename std::pair<KeyT, T>;
        using ListIt     = typename std::list<ListElem_t>::iterator;

        const size_t                        size_;
        std::list<ListElem_t>               cache_;
        std::unordered_map<KeyT, ListIt>    hash_map_;
        
        std::list<KeyT> request_list_;
        size_t request_num_;

        bool full() const
        {
            return cache_.size() == size_;
        }

        KeyT find_last_seen(KeyT request_key)
        {
            std::list<KeyT> temp_list;
            temp_list.push_back(request_key);

            for (auto elem : cache_)
                temp_list.push_back(elem.first);
            
            auto request_it = request_list_.begin();
            
            while (temp_list.size() > 1 && request_it != request_list_.end())
            {
                for (auto temp_it = temp_list.begin(); temp_it != temp_list.end(); temp_it++)
                {
                    if (*temp_it == *request_it)
                    {
                        temp_list.erase(temp_it);
                        break;
                    }
                }
                
                request_it = std::next(request_it);
            }

            return *temp_list.begin();
        }

        template <typename F>
        bool lookup_update(KeyT request_key, F slow_get_page)        
        {
            request_list_.pop_front();

            if (hash_map_.contains(request_key))
                return true;
                
            if (full())
            {
                KeyT last_seen_key = find_last_seen(request_key);
                
                if (request_key != last_seen_key)
                {
                    auto list_it = hash_map_.find(last_seen_key);

                    hash_map_.erase(last_seen_key);
                    cache_.erase(list_it->second);
                }
                else 
                {
                    return false;
                }
            }

            cache_.emplace_front(request_key, slow_get_page(request_key));
            hash_map_.emplace(request_key, cache_.begin());
            
            return false;    
        }

public:
        IdealCache(size_t size, std::list<KeyT> request_list) : size_(size), request_list_(request_list) {}
        
        template <typename F>
        size_t get_hits(std::vector<int> request_list, F slow_get_page)
        {
            size_t hits = 0;

            for (auto vector_elem : request_list)
                if (lookup_update(vector_elem, slow_get_page))
                    hits++;

            return hits;
        }

        void print() const
        {
            std::cout << "Cache:" << std::endl << "keys:";
            for (auto hash_elem : hash_map_)
            {
                std::cout << " " << get_key(hash_elem);
            }
            std::cout << std::endl;

            std::cout << "Request List:";
            for (auto elem : request_list_)
            {
                std::cout << " " << elem;
            }
            std::cout << std::endl << std::endl;
        }
    };

} // namespace caches