#ifndef EPROXY_BRIDGE_H_
#define EPROXY_BRIDGE_H_

#include "utils.h"
#include "tunnel.h"
#include "group.h"

#include "BS_thread_pool/BS_thread_pool_light.hpp"

#include <deque>
#include <shared_mutex>

class Bridge {
    IP app_ip_;
    Port app_port_;
    IP local_ip_;
    Port local_port_;

    BS::thread_pool_light thread_pool_;
    std::deque<FD> worker_fds_;
    size_t worker_idx_;
    std::mutex worker_mtx_;
    
    TunnelGroup tunnel_;
    

public:
    Bridge();
    ~Bridge();

    FD& SelectWorkerFd();
};

class BridgeGroup
{
private:
    /* data */
public:
    BridgeGroup(/* args */);
    ~BridgeGroup();
};


#endif // EPROXY_BRIDGE_H_