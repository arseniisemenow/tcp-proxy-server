#include "proxy_server.h"

int main() {
  try {
    boost::asio::io_context io_context;
    ProxyServer server(io_context, 5432, "localhost", 5432);
    io_context.run();
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }
  return 0;
}
