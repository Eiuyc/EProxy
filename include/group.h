#ifndef EPROXY_GROUP_H_
#define EPROXY_GROUP_H_

#include <unordered_map>
#include <shared_mutex>
#include <memory>


template<typename K, typename T>
class Group
{
    std::unordered_multimap<K, T> items_;
    std::shared_mutex mtx_;
    
public:
    
    template<typename... Args>
    bool Add(Args&&... args);

    std::weak_ptr<T> Get(K);
    
    bool Del(K);
};


#endif // EPROXY_GROUP_H_