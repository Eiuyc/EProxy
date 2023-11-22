#ifndef EPROXY_H_
#define EPROXY_H_

#include "config.h"
#include "bridge.h"


class Eproxy
{
    Config cfg_;
    BridgeGroup bridge_group_;
public:
    Eproxy(Config &&);
};



#endif // EPROXY_H_