#pragma once 

#include <list>
#include <tuple>
#include <unordered_map>
#include <cstdlib>
#include <iostream>


namespace caches {

template <typename T, typename F, typename KeyT = int>
class Cache
{
protected:
    size_t size_;
    std::list<std::tuple<KeyT, T, size_t>> cache_;

    using ListIt = typename std::list<std::tuple<KeyT, T, size_t>>::iterator;
    std::unordered_map<KeyT, ListIt> hash_map_;

    bool full() const {return cache_.size() == size_;}

public:
    Cache(size_t size) : size_(size) {}

    virtual bool lookup_update(KeyT key, F slow_get_page);

    void print() const
    {
        std::cout << "Hash map:" << std::endl << "keys:";
        for (auto hash_it = hash_map_.begin(); hash_it != hash_map_.end(); hash_it = std::next(hash_it))
        {
            std::cout << " " << std::get<0>(*hash_it);
        }
        std::cout << std::endl;

        std::cout << "List(Cache):" << std::endl << "keys\tcounter" << std::endl;
        for (auto list_it = cache_.begin(); list_it != cache_.end(); list_it = std::next(list_it))
        {
            std::cout << std::get<0>(*list_it) << "\t" << std::get<2>(*list_it) << std::endl;
        }
    }
}; // class Cache


template <typename T, typename F, typename KeyT = int>
class LFUCache : public Cache<T, F, KeyT>
{
public:
    LFUCache(size_t size) : Cache<T, F, KeyT>(size) {}

    bool lookup_update(KeyT key, F slow_get_page)
    {
        auto hit = this->hash_map_.find(key);

        if (hit == this->hash_map_.end()) // key not found
        {
            if (this->full())
            {
                // LFU algoritm

                // find min
                auto it = this->cache_.begin();
                auto min_it = it;

                size_t min_counter = std::get<2>(*it);
                for (; it != this->cache_.end(); it = std::next(it))
                {
                    size_t counter = std::get<2>(*it);
                    if (min_counter > counter)
                    {
                        min_counter = counter;
                        min_it = it;
                    }
                }
            
                this->hash_map_.erase(std::get<0>(*min_it));
                this->cache_.erase(min_it);
            }
            
            this->cache_.emplace_front(key, slow_get_page(key), 1);
            this->hash_map_.emplace(key, this->cache_.begin());

            return false;
        }
        
        // key found
        auto list_it = hit->second;
        std::get<2>(*list_it) += 1;

        // exchange assending
        auto next_it = std::next(list_it);

        if (next_it != this->cache_.begin() && next_it != this->cache_.end())
        {
            if (std::get<2>(*list_it) > std::get<2>(*next_it))
                this->cache_.splice(list_it, this->cache_, next_it, std::next(next_it));
        }

        return true;
    }

}; // LFUCache


template <typename T, typename F, typename KeyT = int>
class IdealCache : public Cache<T, F>
{
private:
    std::list<KeyT> request_list_;
    size_t request_num_;
    
    void inquire_request()
    {
        KeyT key = 0;
        for (size_t i = 0; i < request_num_; i++)
        {
            std::cin >> key;
            request_list_.push_back(key);
        }
    }

    KeyT find_last_seen()
    {
        std::list<KeyT> temp_list;
                
        for (auto it = this->cache_.begin(); it != this->cache_.end(); it = std::next(it))
        {
            temp_list.push_back(std::get<0>(*it));
        }

        auto request_it = request_list_.begin();
        
        auto it = temp_list.begin();
        
        while (temp_list.size() != 0 && request_it != request_list_.end())
        {
            for (it = temp_list.begin(); it != this->temp_list.end(); it = std::next(request_it))
            {
                if (*it == *request_it) // TODO
                {
                    temp_list.erase(it);
                }
            }
            
            request_it = std::next(request_it);
        }

        KeyT last_seen = *it;

        return last_seen;
    }

public:
    IdealCache(size_t size, size_t request_num) : Cache<T, F, KeyT>(size), request_num_(request_num) {inquire_request();}

    bool lookup_update(KeyT key, F slow_get_page)
    {
        auto hit = this->hash_map_.find(key);
        
        if (hit == this->hash_map_.end()) // key not found
        {
            if (this->full())
            {
                KeyT key = find_last_seen();
                
                for (auto it = this->cache_.end(); it != this->cache_.end(); it = std::next(it))
                {
                    if (key == std::get<0>(*it))
                    {
                        this->hash_map_.erase(std::get<0>(*it));
                        this->cache_.erase(it);
                    }
                }
            }
            
            this->cache_.emplace_front(key, slow_get_page(key), 1);
            this->hash_map_.emplace(key, this->cache_.begin());

            return false;
        }
        
        return true;  
    }
}; // IdealCache

} // namespace caches
