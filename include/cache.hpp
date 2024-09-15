#pragma once

#include <list>
#include <unordered_map>
#include <iterator>
#include <vector>

#include <iostream>

namespace caches
{
    std::pair<size_t, std::vector<int>> get_input(std::istream &is)
    {
        size_t cache_size = 0, request_nums = 0;
        std::vector<int> request_list{};

        is >> cache_size >> request_nums;
        
        int request_key = 0;
        for (size_t i = 0; i < request_nums; i++)
        {
            is >> request_key;
            request_list.push_back(request_key);
        }

        std::pair<size_t, std::vector<int>> pair{cache_size, request_list};

        return pair;
    }
// 
// Base Cache Class 
//
    template <typename T, typename F, typename ListElem_t, typename KeyT = int>
    class Cache
    {
protected:
        using ListIt    = typename std::list<ListElem_t>::iterator;
        using HashMapIt = typename std::unordered_map<KeyT, ListIt>::iterator;

        const size_t size_;
        std::list<ListElem_t> cache_;
        std::unordered_map<KeyT, ListIt> hash_map_;

        bool full() const { return cache_.size() == size_; }
public:
        Cache(const size_t size) : size_(size) {}

        virtual bool lookup_update(KeyT key, F slow_get_page) = 0;

        size_t get_hits(std::vector<int> request_list, F slow_get_page)
        {
            size_t hits = 0;

            for (auto vector_elem : request_list)
            {
                if (lookup_update(vector_elem, slow_get_page))
                    hits++;
            }

            return hits;
        }
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

        static  KeyT get_key(ListIt it) { return std::get<0>(*it); }
        static    T get_data(ListIt it) { return std::get<1>(*it); }
        
        static size_t  get_count(ListIt it) { return std::get<2>(*it); }
        static void    set_count(ListIt it, const size_t count) { std::get<2>(*it) = count;}

        void sort_list_in_ascending_order(ListIt list_it)
        {
            auto next_it = std::next(list_it);

            if (next_it != cache_.begin() && next_it != cache_.end())
            {
                if (get_count(list_it) > get_count(next_it))
                    cache_.splice(list_it, cache_, next_it, std::next(next_it));
            }

            return;
        }

        void replace_cache(KeyT request_key, F slow_get_page)
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
        bool lookup_update(KeyT request_key, F slow_get_page) override
        {
            auto hit = hash_map_.find(request_key);

            if (hit == hash_map_.end()) // key not found
            {
                replace_cache(request_key, slow_get_page);

                return false;
            }

            // key found
            auto list_it = hit->second;
            set_count(list_it, get_count(list_it) + 1);

            sort_list_in_ascending_order(list_it);

            return true;
        }

        void print() 
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

        std::list<KeyT> request_list_;
        size_t request_num_;

        using ListIt = typename std::list<ListElem_t>::iterator;

        static KeyT get_key(ListIt it) { return std::get<0>(*it); }

        KeyT find_last_seen(KeyT request_key)
        {
            std::list<KeyT> temp_list;
            temp_list.push_back(request_key);

            for (auto temp_it = cache_.begin(); temp_it != cache_.end(); temp_it = std::next(temp_it))
            {
                KeyT key = temp_it->first;
                temp_list.push_back(key);
            }
            
            auto temp_it = temp_list.begin();
            auto request_it = request_list_.begin();
            
            while (temp_list.size() > 1 && request_it != request_list_.end())
            {
                for (temp_it = temp_list.begin(); temp_it != temp_list.end(); temp_it = std::next(temp_it))
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
    public:
        IdealCache(size_t size, std::list<KeyT> request_list) : Cache<T, F, ListElem_t, KeyT>(size), request_list_(request_list) {}
        
        bool lookup_update(KeyT request_key, F slow_get_page) override        
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

        void print() 
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
        bool lookup_update(KeyT request_key, F slow_get_page) override 
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
    }; // LRUCache class

} // namespace cache


