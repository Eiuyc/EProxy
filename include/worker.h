#ifndef EPROXY_WORKER_H_
#define EPROXY_WORKER_H_

#include "tunnel.h"
#include "bridge.h"

#include <memory>


void Leader(Bridge*, std::weak_ptr<TunnelGroup>, IP, Port, IP, Port);
void Worker(std::weak_ptr<TunnelGroup>, FD&);

#endif // EPROXY_WORKER_H_
