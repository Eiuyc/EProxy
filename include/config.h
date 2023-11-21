#ifndef EPROXY_CONFIG_H_
#define EPROXY_CONFIG_H_

#include "utils.h"
#include "toml++/toml.hpp"
#include <iostream>


class Config
{
    toml::table cfg_;
    std::string_view server_ip_;
    Port server_port_;
    toml::v3::array *app_cfgs_;

    bool isValid(toml::table &);
public:
    Config(int, char *[]);
    ~Config() {}
};

#endif // EPROXY_CONFIG_H_