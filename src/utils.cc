#include "utils.h"


std::pair<FD, std::shared_ptr<sockaddr_in>>
ListenOn(IP ip, Port port) {
    auto server_addr{std::make_shared<sockaddr_in>()};
    FD fd{socket(AF_INET, SOCK_STREAM, 0)};
    memset(server_addr.get(), 0x00, sizeof(sockaddr_in));
    server_addr->sin_family = AF_INET;
    server_addr->sin_addr.s_addr = inet_addr(ip);
    server_addr->sin_port = htons(port);
    if (bind(fd, (sockaddr *)server_addr.get(), sizeof(sockaddr_in)) < 0) {
        perror("bind err");
        fd = -1;
    }
    if (listen(fd, 2048) < 0) {
        perror("listen err");
        fd = -1;
    }
    if(fd != -1)
        printf(
            "[UTILS] listen on (%s:%d) [%d] success\n",
            ip, port, static_cast<int>(fd)
        );
    return std::pair(std::move(fd), server_addr);
}


FD ConnectTo(IP ip, Port port) {
    FD sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_addr;
    memset(&server_addr, 0x00, sizeof(sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(port);
    if (connect(sock_fd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("[UTILS] connect err: %s\n", strerror(errno));
        sock_fd = -1;
    };
    printf("[UTILS] connect to server (%s:%d) ok\n", ip, port);
    return sock_fd;
}


bool SetFdNonBlock(FD &fd) {
    int flag{fcntl(fd, F_GETFL, 0)};
    bool result{flag != -1};
    if (result) {
        flag |= O_NONBLOCK;
        result = (fcntl(fd, F_SETFL, flag) != -1);
    }
    const char * t[]{"failed", "ok"};
    printf("[UTILS] set [%d] %s\n", int(fd), t[result?1:0]);
    return result;
};


bool AddFd(FD &epfd, FD &fd) {
    bool result{true};
    SetFdNonBlock(fd);
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event) < 0) {
        perror("[AddFd] error:");
        printf("[AddFd] add fd[%d] to epfd[%d] failed\n", int(epfd), int(fd));
        result = false;
    }
    return result;
}


bool DelFd(FD &epfd, FD &fd) {
    bool result{true};
    result = (epoll_ctl(epfd, EPOLL_CTL_DEL, fd, nullptr) == 0);
    return result;
}


FD GetEpfd() {
    int epfd{epoll_create1(EPOLL_CLOEXEC)};
    printf("[UTILS] created epfd[%d] ok\n", epfd);
    return epfd;
}