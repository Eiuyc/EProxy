#include <unistd.h>

#include "bridge.h"
#include "worker.h"


Bridge::Bridge(
        const char* app_name,
        IP app_ip,
        Port app_port,
        IP local_ip,
        Port local_port
    ):
    app_name_{app_name},
    app_ip_{app_ip},
    app_port_{app_port},
    local_ip_{local_ip},
    local_port_{local_port},
    key_{
        std::to_string(app_port) + std::to_string(local_port) + 
        app_name + app_ip + local_ip
    },
    
    worker_fds_(2),
    worker_idx_{0},
    thread_pool_{
        std::make_shared<BS::thread_pool_light>(worker_fds_.size() + 1)
    },
    tunnel_group_{std::make_shared<TunnelGroup>()}
{
    printf("[Bridge] initializing workers...\n");
    for(auto &worker_fd: worker_fds_) {
        worker_fd = std::make_shared<FD>(GetEpfd());
        if(*worker_fd == -1) {
            perror("create epfd failed");
            exit(1);
        }
        printf("[Bridge] initializing worker...\n");
        thread_pool_->push_task(std::bind(
            Worker, tunnel_group_, std::ref(*worker_fd)
        ));
    }
    printf("[Bridge] initializing leader...\n");
    thread_pool_->push_task(std::bind(
        Leader, this, tunnel_group_, local_ip, local_port, app_ip, app_port
    ));
}

FD& Bridge::SelectWorkerFd() {
    std::lock_guard lock(worker_mtx_);
    worker_idx_ = (worker_idx_+1) % worker_fds_.size();
    return *worker_fds_[worker_idx_];
}

std::string Bridge::GetKey() {
    return key_;
}

std::weak_ptr<TunnelGroup> Bridge::GetTunnelGroup() {
    return tunnel_group_;
}

void Bridge::Wait() {
    printf("[Bridge] wait_for_tasks...\n");
    thread_pool_->wait_for_tasks();
    printf("[Bridge] tasks done\n");
}

void Bridge::Stop() {
    printf("[Bridge] stopping...\n");
    auto ShutdownFd{
        [](FD &fd, const char *title) {
            shutdown(fd, SHUT_RDWR);
            printf("[Bridge] %s%d shutdown\n", title, int(fd));
            close(fd);
            printf("[Bridge] %s%d closed\n", title, int(fd));
        }
    };
    ShutdownFd(leader_fd_, "leader");
    for(auto &worker_fd: worker_fds_) {
        ShutdownFd(*worker_fd, "worker");
    }
}

void Bridge::SetLeaderFd(FD &&fd) {leader_fd_ = std::move(fd);}
FD& Bridge::GetLeaderFd() {return leader_fd_;}


bool BridgeGroup::Add(
    const char* app_name,
    IP app_ip,
    Port app_port,
    IP local_ip,
    Port local_port
) {
    auto sp{std::make_shared<Bridge>(
        app_name, app_ip, app_port, local_ip, local_port
    )};
    items_.insert(std::make_pair(sp->GetKey(), sp));
    return true;
}

void BridgeGroup::WaitAll() {
    int i = 0;
    for(auto &[k, sp]: items_) {
        printf("[BridgeGroup] Waiting bridge%d...\n", i);
        sp->Wait();
        printf("[BridgeGroup] bridge%d done\n", i++);
    }
}

void BridgeGroup::Stop() {
    for(auto &[k, sp]: items_) {
        sp->Stop();
    }
}