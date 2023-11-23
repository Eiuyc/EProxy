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
        printf("[EProxy] Waiting 30s for workers to quit\n");
        bridge_group_.Stop();
    }
    void WaitBridges() {
        printf("[EProxy] waiting bridges...\n");
        bridge_group_.WaitAll();
    }
};

extern std::shared_ptr<EProxy> eproxy;

void SignalHandler(int);

#endif // EPROXY_H_