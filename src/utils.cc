#include "utils.h"


auto ListenOn(IP ip, Port port) {
    auto server_addr{std::make_shared<sockaddr_in>()};
    FD fd{socket(AF_INET, SOCK_STREAM, 0)};
    memset(server_addr.get(), 0x00, sizeof(server_addr));
    server_addr->sin_family = AF_INET;
    server_addr->sin_addr.s_addr = inet_addr(ip);
    server_addr->sin_port = htons(port);
    if (bind(fd, (sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind err");
        fd = -1;
    }
    if (listen(fd, 2048) < 0) {
        perror("listen err");
        fd = -1;
    }
    if(fd != -1)
        printf("[UTILS] success listen on:\n%s:%d| fd=[%d]\n", ip, port, static_cast<int>(fd));
    return std::pair(std::move(fd), server_addr);
}

auto ConnectTo(IP ip, Port port) {
    FD sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_addr;
    memset(&server_addr, 0x00, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(port);
    if (connect(sock_fd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("[UTILS] connect err: %s\n", strerror(errno));
        sock_fd = -1;
    };
    printf("[UTILS] success connect to server (%s:%d)\n", ip, port);
    return sock_fd;
}
