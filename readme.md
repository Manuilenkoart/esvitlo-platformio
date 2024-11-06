# setup project

>create src/env_m.h file with structure

```sh
#ifndef  WIFI_CONFIG
#define  WIFI_CONFIG

#define  STASSID  ""
#define  STAPSK  ""

#define  APSSID  ""
#define  APPSK  ""

#define  M_DNS_NAME ""
#endif
```

```sh
#ifndef PORTS_CONFIG
#define PORTS_CONFIG

#define PORT_HTTP 00 

#endif
```

```sh
#ifndef WS_CLIENTS__CONFIG
#define WS_CLIENTS__CONFIG

#define WS_CLIENTS_CONNECTED_MAX 0 
#define WS_CLIENTS_TIMEOUT 1000 * 60 * 3
#endif
```
