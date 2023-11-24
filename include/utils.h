#ifndef EPROXY_UTILS_H_
#define EPROXY_UTILS_H_

#include <netinet/in.h>
#include <memory>


using Port = uint16_t;
using IP = const char *;

class FD {
    int fd_;
    void Close();
    // copy con FD
    FD(const FD &) = delete;
    // copy assign FD
    FD& operator =(const FD &) = delete;
public:
    FD();
    ~FD();
    
    // copy con int
    FD(const int &fd);

    // copy assign int
    FD& operator =(const int &);

    // move con int
    FD(int &&fd);
    // move con FD
    FD(FD &&rhs);
    
    // move assign int
    FD& operator =(int &&);
    // move assign FD
    FD& operator =(FD &&);

    operator int() const;
    bool operator ==(const int &fd) const;
    bool operator !=(const int &fd) const;
};


std::pair<FD, std::shared_ptr<sockaddr_in>>
ListenOn(IP, Port);

FD ConnectTo(IP, Port);

FD GetEpfd();

bool SetFdNonBlock(FD &);

bool AddFd(FD &epfd, FD &fd);

bool DelFd(FD &epfd, FD &fd);


void SignalHandler(int);


#endif // EPROXY_UTILS_H_