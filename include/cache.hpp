#pragma once

#include <list>
#include <unordered_map>
#include <iterator>
#include <vector>

#include <iostream>

namespace caches
{
// 
// Base Cache Class 
//
    template <typename T, typename F, typename ListElem_t, typename KeyT = int>
    class Cache
    {
public:
        using ListIt    = typename std::list<ListElem_t>::iterator;
        using HashMapIt = typename std::unordered_map<KeyT, ListIt>::iterator;

protected:
        size_t size_;
        std::list<ListElem_t> cache_;
        std::unordered_map<KeyT, ListIt> hash_map_;

        bool full() { return cache_.size() == size_; }

public:
        Cache(size_t size) : size_(size) {}

        virtual bool lookup_update(KeyT key, F slow_get_page) = 0;
    };

// 
// LFU Cache Class 
//
    template <typename T, typename F, typename KeyT = int>
    class LFUCache : public Cache<T, F, std::tuple<KeyT, T, size_t>, KeyT>
    {
        using ListElem_t = typename std::tuple<KeyT, T, size_t>;

        using caches::Cache<T, F, ListElem_t>::Cache;
        using caches::Cache<T, F, ListElem_t>::full;
        using caches::Cache<T, F, ListElem_t>::hash_map_;
        using caches::Cache<T, F, ListElem_t>::cache_;

        using ListIt    = typename std::list<ListElem_t>::iterator;
        using HashMapIt = typename std::unordered_map<KeyT, ListIt>::iterator;

        KeyT      get_key(ListIt it) { return std::get<0>(*it); }
        T        get_data(ListIt it) { return std::get<1>(*it); }
        size_t& get_count(ListIt it) { return std::get<2>(*it); }

        KeyT        get_key(HashMapIt it) { return std::get<0>(*it); }
        ListIt   get_ListIt(HashMapIt it) { return std::get<1>(*it); }

    public:
        bool lookup_update(KeyT key, F slow_get_page) override
        {
            auto hit = hash_map_.find(key);

            if (hit == hash_map_.end()) // key not found
            {
                if (this->full())
                {
                    hash_map_.erase(get_key(cache_.begin()));
                    cache_.erase(cache_.begin());
                }

                cache_.emplace_front(key, slow_get_page(key), 1);
                hash_map_.emplace(key, cache_.begin());

                return false;
            }

            // key found
            auto list_it = hit->second;
            get_count(list_it) += 1;

            // exchange assending
            auto next_it = std::next(list_it);

            if (next_it != cache_.begin() && next_it != cache_.end())
            {
                if (get_count(list_it) > get_count(next_it))
                    cache_.splice(list_it, cache_, next_it, std::next(next_it));
            }

            return true;
        }

        void print() 
        {
            std::cout << "Hash map:" << std::endl << "keys:";
            for (HashMapIt hash_it = hash_map_.begin(); hash_it != hash_map_.end(); hash_it = std::next(hash_it))
            {
                std::cout << " " << get_key(hash_it);
            }
            std::cout << std::endl;
            std::cout << "List(Cache):" << std::endl << "keys\tcounter" << std::endl;
            for (auto list_it = cache_.begin(); list_it != cache_.end(); list_it = std::next(list_it))
            {
                std::cout << get_key(list_it) << "\t" << get_count(list_it) << std::endl;
            }
        }
    };


// 
// Ideal Cache Class 
//
    template <typename T, typename F, typename KeyT = int>
    class IdealCache : public Cache<T, F, std::pair<KeyT, T>, KeyT>
    {
        using ListElem_t = typename std::pair<KeyT, T>;
        
        using caches::Cache<T, F, ListElem_t>::Cache;
        using caches::Cache<T, F, ListElem_t>::full;
        using caches::Cache<T, F, ListElem_t>::hash_map_;
        using caches::Cache<T, F, ListElem_t>::cache_;

        std::vector<KeyT> request_list_;
        size_t request_num_;

        KeyT find_last_seen()
        {
            std::list<KeyT> temp_list;
                    
            for (auto it = cache_.begin(); it != cache_.end(); it = std::next(it))
            {
                KeyT key = it->first;
                temp_list.push_back(key);
            }

            auto request_it = request_list_.begin();
            
            auto it = temp_list.begin();
            
            while (temp_list.size() > 1 && request_it != request_list_.end())
            {
                for (it = temp_list.begin(); it != temp_list.end(); it = std::next(it))
                {
                    if (*it == *request_it)
                    {
                        temp_list.erase(it);
                        break;
                    }
                }
                
                request_it = std::next(request_it);
            }

            return *it;
        }
    public:
        IdealCache(size_t size, std::vector<KeyT> request_list) : Cache<T, F, ListElem_t, KeyT>(size), request_list_(request_list) {}

        bool lookup_update(KeyT key, F slow_get_page) override        
        {
            auto hit = hash_map_.find(key);
            
            if (hit == hash_map_.end()) // key not found
            {
                if (full())
                {
                    KeyT key = find_last_seen();
                    
                    auto hit = hash_map_.find(key);

                    hash_map_.erase(key);
                    cache_.erase(hit->second);
                }
                
                cache_.emplace_front(key, slow_get_page(key));
                hash_map_.emplace(key, cache_.begin());

                return false;
            }
            
            return true;  
        }

    }; // Ideal Cache class


// 
// LRU Cache Class 
//
    template <typename T, typename F, typename KeyT = int>
    class LRUCache : public Cache<T, F, std::pair<KeyT, T>, KeyT>
    {
        using ListElem_t = typename std::pair<KeyT, T>;
        
        using caches::Cache<T, F, ListElem_t>::Cache;
        using caches::Cache<T, F, ListElem_t>::full;
        using caches::Cache<T, F, ListElem_t>::hash_map_;
        using caches::Cache<T, F, ListElem_t>::cache_;

    public:
        bool lookup_update(KeyT key, F slow_get_page) override 
        { 
            auto hit = hash_map_.find(key);

            if (hit == hash_map_.end())
            {
                if (full())
                {
                    hash_map_.erase(cache_.back().first);
                    cache_.pop_back();
                }

                cache_.emplace_front(key, slow_get_page(key));
                hash_map_.emplace(key, cache_.begin());

                return false;
            }

            auto list_it = hit->second;
            if (list_it != cache_.begin())
                cache_.splice(cache_.begin(), cache_, list_it, std::next(list_it));
            
            return true;
        }
    }; // LRUCache class

} // namespace cache
