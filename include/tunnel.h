#ifndef EPROXY_TUNNEL_H_
#define EPROXY_TUNNEL_H_

#include "utils.h"
#include "group.h"


class Bridge;
class TunnelGroup;
class Tunnel {
    std::weak_ptr<TunnelGroup> tunnel_group_;
    FD* epfd_;
    FD fd_;
    FD peer_fd_;
    bool valid_;

    // copy con
    Tunnel(const Tunnel &) = delete;
    // copy assign
    Tunnel& operator =(const Tunnel &) = delete;
    // move assign
    Tunnel& operator =(Tunnel &&) = delete;
public:
    Tunnel(Bridge*, std::weak_ptr<TunnelGroup>, FD &&, IP, Port);
    // move con
    Tunnel(Tunnel &&);

    bool IsValid();

    bool Register();
    bool Unregister();

    FD& GetFd();
    FD& GetPeerFd();
};


class TunnelGroup: public Group<int, Tunnel> {

public:
    bool Add(Bridge*, std::weak_ptr<TunnelGroup>, FD &&, IP, Port);
};

#endif // EPROXY_TUNNEL_H_