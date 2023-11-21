#include "tunnel.h"
#include "bridge.h"

    
Tunnel::Tunnel(std::weak_ptr<Bridge> bridge_ptr, FD &&fd, IP ip, Port port):
    fd_(std::forward<FD>(fd)),
    peer_fd_(ConnectTo(ip, port))
{
    if(peer_fd_ == -1) return;
    epfd_ = &bridge_ptr.lock()->SelectWorkerFd();
    
}

// move con
Tunnel::Tunnel(Tunnel &&rhs):
    epfd_(rhs.epfd_),
    fd_(std::move(rhs.fd_)),
    peer_fd_(std::move(rhs.peer_fd_))
{
}

TunnelGroup::TunnelGroup()
{
}

TunnelGroup::~TunnelGroup()
{
}
