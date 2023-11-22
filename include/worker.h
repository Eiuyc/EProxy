#ifndef EPROXY_WORKER_H_
#define EPROXY_WORKER_H_

#include "tunnel.h"
#include <memory.h>

void Leader(std::shared_ptr<TunnelGroup>, IP, Port);
void Worker(std::shared_ptr<TunnelGroup>, std::weak_ptr<FD>);

#endif // EPROXY_WORKER_H_
