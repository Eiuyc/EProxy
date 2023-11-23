#ifndef EPROXY_GROUP_H_
#define EPROXY_GROUP_H_

#include <unordered_map>
#include <shared_mutex>
#include <memory>


template<typename K, typename T>
class Group
{
protected:
    std::unordered_map<K, std::shared_ptr<T>> items_;
    std::shared_mutex item_mtx_;
    
public:
    
    template<typename... Args>
    bool Add(Args&&... args);

    std::weak_ptr<T> Get(K key) {
        std::shared_lock lock(item_mtx_);
        auto it{items_.find(key)};
        if(it == items_.end()) {
            return std::weak_ptr<T>();
        }
        return it->second;
    }
    
    size_t Del(K key) {
        std::lock_guard lock(item_mtx_);
        return items_.erase(key);
    }
};


#endif // EPROXY_GROUP_H_