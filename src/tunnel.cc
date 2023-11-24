#include "tunnel.h"
#include "bridge.h"

    
Tunnel::Tunnel(
    Bridge* bridge,
    std::weak_ptr<TunnelGroup> tunnel_group,
    FD &&fd, IP ip, Port port
):
    tunnel_group_{tunnel_group},
    fd_{std::move(fd)},
    peer_fd_{ConnectTo(ip, port)}
{
    printf("[Tunnel(%d,%d)] initializing...\n", int(fd_), int(peer_fd_));
    if(peer_fd_ == -1){
        printf("[Tunnel] ConnectTo (%s:%d) failed\n", ip, port);
        return;
    }
    epfd_ = &bridge->SelectWorkerFd();
}

// move con
Tunnel::Tunnel(Tunnel &&rhs):
    epfd_{std::move(rhs.epfd_)},
    fd_{std::move(rhs.fd_)},
    peer_fd_{std::move(rhs.peer_fd_)},
    valid_{std::move(rhs.valid_)}
{
}


bool Tunnel::Register() {
    printf(
        "[Tunnel] try Registering [%d] [%d] to [%d]...\n",
        int(fd_), int(peer_fd_), int(*epfd_)
    );
    
    bool result{AddFd(*epfd_, fd_)};
    if (result and !(result = AddFd(*epfd_, peer_fd_))) {
        DelFd(*epfd_, fd_);
        if(auto sp{tunnel_group_.lock()}) {
            sp->Del(int(fd_));
            sp->Del(int(peer_fd_));
        }
    }
    return result;
}

bool Tunnel::Unregister() {
    printf(
        "[Tunnel] try Unregistering (%d, %d) from [%d]...\n",
        int(fd_), int(peer_fd_), int(*epfd_)
    );
    DelFd(*epfd_, fd_);
    DelFd(*epfd_, peer_fd_);
    if(auto sp{tunnel_group_.lock()}) {
        sp->Del(int(fd_));
        sp->Del(int(peer_fd_));
    }
    printf(
        "[Tunnel] Unregister (%d, %d) from [%d] success\n",
        int(fd_), int(peer_fd_), int(*epfd_)
    );
    return true;
}


FD& Tunnel::GetFd() {
    return fd_;
}


FD& Tunnel::GetPeerFd() {
    return peer_fd_;
}