#ifndef EPROXY_BRIDGE_H_
#define EPROXY_BRIDGE_H_

#include "tunnel.h"

#include "BS_thread_pool/BS_thread_pool_light.hpp"

#include <deque>
#include <shared_mutex>


class Bridge {
    const char * app_name_;
    IP app_ip_;
    Port app_port_;
    IP local_ip_;
    Port local_port_;
    std::string key_;
    
    FD leader_fd_;
    std::thread leader_thread_;
    std::deque<std::shared_ptr<FD>> worker_fds_;
    size_t worker_idx_;
    std::shared_ptr<BS::thread_pool_light> thread_pool_;
    std::mutex worker_mtx_;
    
    std::shared_ptr<TunnelGroup> tunnel_group_;

    // copy con
    Bridge(const Bridge &) = delete;
    // move con
    Bridge(Bridge &&) = delete;
    // copy assign
    Bridge& operator =(const Bridge &) = delete;
    // move assign
    Bridge& operator =(Bridge &&) = delete;

public:
    Bridge(const char*, IP, Port, IP, Port);
    FD& SelectWorkerFd();
    std::string GetKey();
    std::weak_ptr<TunnelGroup> GetTunnelGroup();
    void Wait();
    void Stop();
    void SetLeaderFd(FD &&fd);
    FD& GetLeaderFd();
};


class BridgeGroup: public Group<std::string, Bridge> {

public:
    bool Add(const char*, IP, Port, IP, Port);
    void WaitAll();
    void Stop();
};


#endif // EPROXY_BRIDGE_H_