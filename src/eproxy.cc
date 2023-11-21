#include "eproxy.h"


Eproxy::Eproxy(Config &&cfg): cfg_(cfg)
{
    auto [fd, server_addr]{ListenOn(cfg_.eproxy_ip, cfg_.eproxy_port)};
    printf("[Eproxy] running on (%s:%d) [%d]\n", cfg.eproxy_ip, cfg.eproxy_port, int(fd));

}

int main(int argc, char *argv[]) {
    Eproxy eproxy{Config{argc, argv}};
    return 0;
}