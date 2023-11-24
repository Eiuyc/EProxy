#ifndef EPROXY_CONFIG_H_
#define EPROXY_CONFIG_H_

#include "utils.h"

#include "toml++/toml.hpp"


class Config
{
    bool IsValid();
public:
    toml::table cfg_;
    const char *cfg_file;
    IP eproxy_ip;
    Port eproxy_port;
    toml::v3::array *app_cfgs;
    Config(int, char *[]);
};

#endif // EPROXY_CONFIG_H_