#ifndef EPROXY_BRIDGE_H_
#define EPROXY_BRIDGE_H_

#include "utils.h"
#include "tunnel.h"
#include "group.h"
#include "worker.h"

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
    std::deque<std::shared_ptr<FD>> worker_fds_;
    size_t worker_idx_;
    std::shared_ptr<BS::thread_pool_light> thread_pool_;
    std::mutex worker_mtx_;
    
    std::shared_ptr<TunnelGroup> tunnel_group_;

public:
    Bridge(const char*, IP, Port, IP, Port);

    FD& SelectWorkerFd();
    std::string GetKey();
    void Wait();
};


class BridgeGroup: public Group<std::string, Bridge> {

public:
    template<typename A0, typename A1, typename A2, typename A3, typename A4>
    bool Add(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {
        auto sp{std::make_shared<Bridge>(a0, a1, a2, a3, a4)};
        items_.insert(std::make_pair(sp->GetKey(), sp));
        return true;
    }
    void WaitAll();
};


#endif // EPROXY_BRIDGE_H_