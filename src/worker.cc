#include "worker.h"

#include <chrono>
#include <thread>


void Leader(
    Bridge* bridge,
    std::weak_ptr<TunnelGroup> tunnel_group,
    IP local_ip,
    Port local_port,
    IP app_ip,
    Port app_port
) {
    printf("[Leader] initializing...\n");
    auto [fd, leader_addr]{ListenOn(local_ip, local_port)};
    bridge->SetLeaderFd(std::move(fd));
    FD &leader_fd{bridge->GetLeaderFd()};
    printf(
        "[Leader%d] running on (%s:%d)\n",
        int(leader_fd), local_ip, local_port
    );

    sockaddr_in addr{*leader_addr};
    socklen_t l = sizeof(addr);
    FD accept_fd;
    for(;;) {
        printf("[Leader%d] start loop\n", int(leader_fd));
        accept_fd = accept(leader_fd, (sockaddr *)&addr, &l);
        printf("[Leader%d] get client [%d]\n", int(leader_fd), int(accept_fd));
        if(accept_fd == -1) {
            printf("[Leader%d] stopping...\n", int(leader_fd));
            break;
        }
        tunnel_group.lock()->Add(
            bridge,
            tunnel_group,
            std::move(accept_fd),
            app_ip, app_port
        );
    }
    printf("[Leader%d] done\n", int(leader_fd));
}


void Worker(std::weak_ptr<TunnelGroup> tunnel_group, FD& worker_fd) {
    printf("[Worker%d] initializing...\n", int(worker_fd));
    
    std::vector<epoll_event> epoll_events_(4096);
    const size_t MAX_MSG_LEN{2048};
    char read_msg[MAX_MSG_LEN + 1];
    read_msg[MAX_MSG_LEN] = 0x00;
    char *buf_ptr{read_msg};
    int total_read_num{0};
    int read_num{0};
    for(;;) {
        printf("[Worker%d] start loop\n", int(worker_fd));
        int event_count = epoll_wait(
            worker_fd,
            &*epoll_events_.begin(),
            epoll_events_.size(),
            12'000
        );
        printf("[Worker%d] event_count=%d\n", int(worker_fd), event_count);
        if(event_count < 0) {
            printf(
                "[Worker%d] epoll wait errorno: %d\n",
                int(worker_fd), errno
            );
            if(errno == EBADF) {
                printf("[Worker%d] already closed\n", int(worker_fd));
                break;
            }
            continue;
        }
        for(int i = 0; i < event_count; ++i) {
            epoll_event cur_event = epoll_events_[i];
            int fd = cur_event.data.fd;
            auto t{tunnel_group.lock()->Get(fd)};
            int peer_fd = t.lock()->GetPeerFd();
            if(peer_fd == fd) {
                peer_fd = t.lock()->GetFd();
            }
            if(cur_event.events & EPOLLERR){
                printf("[Worker%d] event error\n", int(worker_fd));
                t.lock()->Unregister();
                continue;
            }
            printf(
                "[Worker%d] process [%d: events%u]\n",
                int(worker_fd), fd, cur_event.events
            );
            buf_ptr = read_msg;
            total_read_num = 0;
            read_num = 0;
            while((read_num = recv(fd, buf_ptr, MAX_MSG_LEN, 0)) > 0) {
                printf(
                    "[Worker%d] recv [%d]: %d bytes\n",
                    int(worker_fd), fd, read_num
                );
                if(read_num && peer_fd != -1) {
                    // TODO：write error? signal(SIGPIPE, SIG_IGN) errno = EPIPE
                    int send_num = send(peer_fd, buf_ptr, read_num, 0);
                    read_msg[read_num] = 0x00;
                    printf(
                        "[Worker%d] send [%d]: %d bytes\n"
                        "<== msg begin ==>\n%s\n<==msg end==>\n",
                        int(worker_fd), fd, send_num, read_msg
                    );
                }
                total_read_num += read_num;
                buf_ptr = read_msg;
                read_num = 0;
            }
            read_msg[total_read_num] = 0x00;
            printf(
                "[Worker%d] [%d]->[%d] total: %d bytes\n",
                int(worker_fd), fd, peer_fd, total_read_num
            );
            if(total_read_num == 0) {
                t.lock()->Unregister();
            }
        }
    }
    printf("[Worker%d] done\n", int(worker_fd));
}
