#ifndef EPROXY_H_
#define EPROXY_H_

#include "config.h"


class Eproxy
{
    Config cfg_;
public:
    Eproxy(Config &&);
};



#endif // EPROXY_H_