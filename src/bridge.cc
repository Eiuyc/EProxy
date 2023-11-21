#include "bridge.h"


Bridge::Bridge()
{
}

Bridge::~Bridge()
{
}

FD& Bridge::SelectWorkerFd() {
    std::lock_guard lock(worker_mtx_);
    worker_idx_ = (worker_idx_ + 1) % worker_fds_.size();
    return worker_fds_[worker_idx_];
}


BridgeGroup::BridgeGroup()
{
}

BridgeGroup::~BridgeGroup()
{
}
