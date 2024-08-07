#include "proxy-server/proxy_server.h"
#include <gtest/gtest.h>
#include "proxy-server/logger.h"

TEST(TCPProxyServerTest, LogSQLQuery) {
  ProxyServer server(8080);
  std::string query = "SELECT * FROM users;";
  ASSERT_EQ(Logger::log(query), query);
}
