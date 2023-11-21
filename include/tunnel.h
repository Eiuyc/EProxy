#ifndef EPROXY_TUNNEL_H_
#define EPROXY_TUNNEL_H_

#include "utils.h"
#include "group.h"

#include <list>
#include <memory>

class Bridge;
class Tunnel {
    FD* epfd_;
    FD fd_;
    FD peer_fd_;

    // copy con
    Tunnel(const Tunnel &) = delete;
    // copy assign
    Tunnel& operator =(const Tunnel &) = delete;
    // move assign
    Tunnel& operator =(Tunnel &&) = delete;
public:
    Tunnel(std::weak_ptr<Bridge>, FD &&, IP, Port);
    // move con
    Tunnel(Tunnel &&);
};

class TunnelGroup
{
private:
    /* data */
public:
    TunnelGroup(/* args */);
    ~TunnelGroup();
};


#endif // EPROXY_TUNNEL_H_