#ifndef PROXY_SERVER_H
#define PROXY_SERVER_H

#include "../constants.h"
#include "../db_shell/db_shell.h"
#include <netinet/in.h>
#include <sqlite3.h>
#include <string>
#include <vector>

inline constexpr int kMaxConnections = 10;

class ProxyServer {
public:
  explicit ProxyServer(int port, int max_connections = kMaxConnections);
  ~ProxyServer() = default;
  void Start();
  void Stop();

private:
  void HandleClient(int client_socket);
  std::string LogSqlQuery(const std::string &query);

private:
  int server_socket_{};
  int port_{};
  int backlog_size_{5};
  std::vector<int> client_sockets_{};
  struct sockaddr_in sockaddr_in_{};

  DBShell db_{constants::kDbname};
};

#endif // PROXY_SERVER_H
