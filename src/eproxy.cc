#include "eproxy.h"


Eproxy::Eproxy(Config &&cfg):
    cfg_{std::move(cfg)}
{
    auto [fd, server_addr]{ListenOn(cfg_.eproxy_ip, cfg_.eproxy_port)};
    printf("[Eproxy] running on (%s:%d) [%d]\n", cfg_.eproxy_ip, cfg_.eproxy_port, int(fd));
    for(auto& node: *cfg_.app_cfgs) {
        auto &app_cfg = *node.as_table();
        // bridge_group_.Add(
        //     app_cfg["name"].value_or(""),
        //     app_cfg["ip"].value_or(""),
        //     app_cfg["port"].value_or(0),
        //     app_cfg["local_ip"].value_or(""),
        //     app_cfg["local_port"].value_or(0)
        // );
        Bridge(
            app_cfg["name"].value_or(""),
            app_cfg["ip"].value_or(""),
            app_cfg["port"].value_or(0),
            app_cfg["local_ip"].value_or(""),
            app_cfg["local_port"].value_or(0)
        );
    }
    bridge_group_.WaitAll();
}

int main(int argc, char *argv[]) {
    Eproxy eproxy{Config{argc, argv}};
    return 0;
}