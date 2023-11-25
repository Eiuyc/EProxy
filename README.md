# EProxy

A simple proxy implemented in c++.

## Features
### Configuration File
The example configuration file `conf/conf.toml` will help to start two proxies named "app1" and "app2". 
```toml
[eproxy]
ip = "127.0.0.1"
port = 12121

[[app]]
name = "app1"
ip = "127.0.0.1"
port = 80
local_ip = "127.0.0.1"
local_port = 10001

[[app]]
name = "app2"
ip = "127.0.0.1"
port = 8080
local_ip = "127.0.0.1"
local_port = 10002
```

## How to use

```bash
# build
sh build.sh

# run
./build/eproxy conf/conf.toml
```

Then, access "app1" with 127.0.0.1:10001