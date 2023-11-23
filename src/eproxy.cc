#include "eproxy.h"


EProxy::EProxy(Config &&cfg):
    cfg_{std::move(cfg)}
{
    // auto [fd, eproxy_addr]{ListenOn(cfg_.eproxy_ip, cfg_.eproxy_port)};
    // eproxy_fd_ = std::move(fd);
    // printf("[EProxy] running on (%s:%d) [%d]\n", cfg_.eproxy_ip, cfg_.eproxy_port, int(eproxy_fd_));
    int i = 0;
    for(auto& node: *cfg_.app_cfgs) {
        printf("[EProxy] creating [bridge%d]...\n", i++);
        auto &app_cfg = *node.as_table();
        bridge_group_.Add(
            app_cfg["name"].value_or(""),
            app_cfg["ip"].value_or(""),
            app_cfg["port"].value_or(0),
            app_cfg["local_ip"].value_or(""),
            app_cfg["local_port"].value_or(0)
        );
    }
}


std::shared_ptr<EProxy> eproxy;


void SignalHandler(int signum) {
    printf("[EProxy] signal %d received\n", signum);
    eproxy->Stop();
    printf("[EProxy] ready to quit\n");
    exit(signum);
}


int main(int argc, char *argv[]) {
    signal(SIGINT, SignalHandler);
    eproxy = std::make_shared<EProxy>(Config{argc, argv});
    // std::this_thread::sleep_for(std::chrono::seconds(100));
    eproxy->WaitBridges();
    // SignalHandler(2);
    return 0;
}