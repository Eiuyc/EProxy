#include "config.h"


bool Config::IsValid() {
    if(cfg_.empty()) return false;
    return true;
}

Config::Config(int argc, char *argv[]): cfg_file("") {
    if(argc < 2) {
        printf("[Config] no cfg file provided\n");
        exit(-1);
        return;
    }
    try {
        cfg_file = argv[1];
        cfg_ = toml::parse_file(cfg_file);

        if(!IsValid()) {
            printf("[Config] Config file [%s] is not valid\n", cfg_file);
            exit(-1);
            return;
        }

        using namespace std::string_view_literals;
        eproxy_ip = cfg_.at_path("eproxy.ip").value_or(""sv).data();
        eproxy_port = cfg_.at_path("eproxy.port").value_or(0);
        app_cfgs = cfg_.at_path("app").as_array();
        printf("[Config] parse %s ok\n", cfg_file);
        printf("[Config] eproxy_ip = %s\n", eproxy_ip);
        printf("[Config] eproxy_port = %d\n", eproxy_port);
    }
    catch (const toml::parse_error& e) {
        printf("[Config] Parsing failed: %s\n", e.what());
    }
    catch (const std::exception& e) {
        printf("[Config] Caught an std::exception: %s\n", e.what());
    }
    catch (...) {
        printf("[Config] Caught an unknown exception\n");
    }
}