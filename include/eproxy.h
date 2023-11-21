#ifndef EPROXY_H_
#define EPROXY_H_

#include "config.h"
#include "bridge.h"

#include <atomic>
#include <csignal>


class EProxy
{
    FD eproxy_fd_;
    Config cfg_;
    BridgeGroup bridge_group_;

public:
    EProxy(Config &&);
    void Stop() {
        printf("[EProxy] stopping...\n");
        bridge_group_.Stop();
    }
    void WaitBridges() {
        printf("[EProxy] waiting bridges...\n");
        bridge_group_.WaitAll();
    }
};


#endif // EPROXY_H_