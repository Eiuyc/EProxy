#ifndef EPROXY_UTILS_H_
#define EPROXY_UTILS_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>

#include <cstring>
#include <cstdio>
#include <cerrno>
#include <memory>

using Port = uint16_t;
using IP = const char *;


class FD {
    int fd_;
    void Close() {
        if(fd_ != -1) {
            close(fd_);
            printf("[FD] close [%d]\n", fd_);
        }
    }
    // copy con FD
    FD(const FD &) = delete;
    // copy assign FD
    FD& operator =(const FD &) = delete;
public:
    FD():fd_(-1) {}
    ~FD() { Close(); }
    // copy con int
    FD(const int &fd): fd_(fd) {}

    // copy assign int
    FD& operator =(const int &fd) {
        Close();
        fd_ = fd;
        return *this;
    }

    // move con int
    FD(int &&fd): fd_(fd) {}
    // move con FD

    FD(FD &&rhs): fd_(rhs.fd_) { rhs.fd_ = -1; }
    
    // move assign int
    FD& operator =(int &&fd) {
        Close();
        fd_ = fd;
        return *this;
    }
    
    // move assign FD
    FD& operator =(FD &&rhs) {
        Close();
        fd_ = rhs.fd_;
        return *this;
    }

    operator int() const { return fd_; }

    bool operator ==(const int &fd) const { return fd_ == fd; }
    bool operator !=(const int &fd) const { return fd_ != fd; }
};


std::pair<FD, std::shared_ptr<sockaddr_in>>
ListenOn(IP ip, Port port);

FD ConnectTo(IP ip, Port port);


#endif // EPROXY_UTILS_H_