#include "config.h"

bool Config::isValid(toml::table &) {
    return true;
}

Config::Config(int argc, char *argv[]) {
    if(argc < 2) {
        std::cerr << "no cfg file provided\n";
    }
    try {
        cfg_ = toml::parse_file(argv[1]);
    }
    catch (const toml::parse_error& err) {
        std::cerr << "Parsing failed: " << err << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Caught an std::exception: " << e.what() << "\n";
    }
    catch (...) {
        std::cerr << "Caught an unknown exception\n";
    }
    if(!isValid(cfg_)) {
        std::cerr << "Config file [" << argv[1] << "] is not valid\n";
    }
    using namespace std::string_view_literals;

    server_ip_ = cfg_.at_path("server.ip").value_or(""sv);
    server_port_ = cfg_.at_path("server.port").value_or(0);
    app_cfgs_ = cfg_.at_path("app").as_array();
    std::cout << "[Config] " << argv[1] << " ok\n";
    std::cout << "[Config] server_ip_ = " << server_ip_ << " \n";
    std::cout << "[Config] server_port_ = " << server_port_ << " \n";
}