#include <gtest/gtest.h>
#include "proxy_server.h"
#include <thread>
#include <pqxx/pqxx>

// Helper function to initialize the database
void initialize_database() {
  system("psql -h localhost -p 5432 -U postgres -f ../init_db.sql");
}

// Test fixture for the ProxyServer tests
class ProxyServerTest : public ::testing::Test {
  protected:
    void SetUp() override {
      // Initialize the database
      initialize_database();

      // Start the proxy server in a separate thread
      proxy_server_thread = std::thread([]() {
        ProxyServer server(5432, "localhost", 5432);
        server.run();
      });

      // Allow some time for the server to start
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    void TearDown() override {
      // Stop the proxy server
      proxy_server_thread.detach();
    }

    std::thread proxy_server_thread;
};

// Test case to verify SQL query logging
TEST_F(ProxyServerTest, SQLQueryLogging) {
  // Connect to the proxy server and execute a query
  try {
    pqxx::connection connection("dbname=testdb user=testuser password=testpassword host=127.0.0.1 port=5432");
    pqxx::work txn(connection);
    txn.exec("SELECT 1;");
    txn.commit();

    // Verify that the query was logged
    std::ifstream log_file("sql_queries.log");
    ASSERT_TRUE(log_file.is_open());

    std::string line;
    bool query_logged = false;
    while (std::getline(log_file, line)) {
      if (line.find("SELECT 1;") != std::string::npos) {
        query_logged = true;
        break;
      }
    }

    ASSERT_TRUE(query_logged);
  } catch (const std::exception& e) {
    FAIL() << "Exception: " << e.what();
  }
}
