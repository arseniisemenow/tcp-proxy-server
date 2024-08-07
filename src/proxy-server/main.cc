#include "proxy-server/proxy_server.h"

int main() {
  ProxyServer server(8080);
  server.Start();
  return 0;
}
