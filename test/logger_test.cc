#include "proxy-server/logger/logger.h"
#include <gtest/gtest.h>

TEST(TCPProxyServerTest, LogSQLQuery) {
  std::string query = "SELECT * FROM unit_tests_table;";
  ASSERT_EQ(Logger::log(query), query);
}
