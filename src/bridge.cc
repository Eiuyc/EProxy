#include "bridge.h"


Bridge::Bridge(
        const char* app_name,
        IP app_ip,
        Port app_port,
        IP local_ip,
        Port local_port
    ):
    app_name_(app_name),
    app_ip_(app_ip),
    app_port_(app_port),
    local_ip_(local_ip),
    local_port_(local_port),
    key_{std::to_string(app_port) + std::to_string(local_port) + app_name + app_ip + local_ip},
    
    worker_fds_(2),
    worker_idx_(0),
    thread_pool_{std::make_shared<BS::thread_pool_light>(worker_fds_.size()+1)},
    tunnel_group_{std::make_shared<TunnelGroup>()}
{
    printf("[Bridge] initializing workers...\n");
    for(auto &worker_fd: worker_fds_) {
        worker_fd = std::make_shared<FD>(GetEpfd());
        if(*worker_fd == -1) {
            perror("create epfd failed");
            exit(1);
        }
        thread_pool_->push_task(std::bind(Worker, tunnel_group_, worker_fd));
    }
    printf("[Bridge] initializing leader...\n");
    thread_pool_->push_task(std::bind(Leader, tunnel_group_, local_ip, local_port));
}

FD& Bridge::SelectWorkerFd() {
    std::lock_guard lock(worker_mtx_);
    worker_idx_ = (worker_idx_ + 1) % worker_fds_.size();
    return *worker_fds_[worker_idx_];
}

std::string Bridge::GetKey() {
    return key_;
}

void Bridge::Wait() {
    thread_pool_->wait_for_tasks();
}

void BridgeGroup::WaitAll() {
    for(auto &[k, sp]: items_) {
        sp->Wait();
    }
}