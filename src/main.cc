#include "proxy_server.h"

int main() {
  ProxyServer server(5432, "localhost", 5432);
  server.run();
  return 0;
}
