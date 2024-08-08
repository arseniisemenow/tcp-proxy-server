#ifndef PROXY_SERVER_SRC_PROXY_SERVER_QUERY_EXECUTOR_QUERY_EXECUTOR_H_
#define PROXY_SERVER_SRC_PROXY_SERVER_QUERY_EXECUTOR_QUERY_EXECUTOR_H_

#include <iostream>
#include <sqlite3.h>

class DBShell {
public:
  DBShell();
  explicit DBShell(const std::string &dbname);
  ~DBShell();
  int SetUp(const std::string &);
  int TearDown();

public:
  std::string ExecuteQuery(const std::string &query);

private:
  static int Callback(void *data, int argc, char **argv, char **column_name);

private:
  sqlite3 *db_{nullptr};
};

#endif // PROXY_SERVER_SRC_PROXY_SERVER_QUERY_EXECUTOR_QUERY_EXECUTOR_H_
