#ifndef EPROXY_TUNNEL_H_
#define EPROXY_TUNNEL_H_

#include "utils.h"
#include "group.h"

#include <list>
#include <memory>

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

    bool IsValid() {return valid_;}

    bool Register();
    bool Unregister();

    FD& GetFd();
    FD& GetPeerFd();
};


class TunnelGroup: public Group<int, Tunnel> {

public:
    template<typename A0, typename A1, typename A2, typename A3, typename A4>
    bool Add(A0 &&a0, A1 &&a1, A2 &&a2, A3 &&a3, A4 &&a4) {
        auto sp{std::make_shared<Tunnel>(
            std::forward<A0>(a0),
            std::forward<A1>(a1),
            std::forward<A2>(a2),
            std::forward<A3>(a3),
            std::forward<A4>(a4)
        )};
        {
            std::lock_guard lock(item_mtx_);
            items_.insert(std::make_pair(int(sp->GetFd()), sp));
            items_.insert(std::make_pair(int(sp->GetPeerFd()), sp));
        }
        sp->Register();
        return true;
    }
};

#endif // EPROXY_TUNNEL_H_