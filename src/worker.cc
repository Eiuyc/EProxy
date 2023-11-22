#include "worker.h"

#include <chrono>
#include <thread>

void Leader(std::shared_ptr<TunnelGroup> tunnel_group, IP local_ip, Port local_port) {
    printf("[Leader] initializing...\n");
    auto [fd, sever_addr]{ListenOn(local_ip, local_port)};
    printf("[Leader] running on (%s:%d) [%d]\n", local_ip, local_port, int(fd));
    std::this_thread::sleep_for(std::chrono::seconds(4));
    printf("[Leader] done with %d\n", int(fd));
}

void Worker(std::shared_ptr<TunnelGroup> tunnel_group, std::weak_ptr<FD> worker_fd) {
    printf("[Worker] initializing...\n");
    
    std::this_thread::sleep_for(std::chrono::seconds(4));
    printf("[Worker] done with %d\n", int(*worker_fd.lock()));
}
