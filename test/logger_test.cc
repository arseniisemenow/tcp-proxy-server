#include <gtest/gtest.h>
#include "proxy-server/logger.h"

TEST(TCPProxyServerTest, LogSQLQuery) {
  std::string query = "SELECT * FROM unit_tests_table;";
  ASSERT_EQ(Logger::log(query), query);
}
