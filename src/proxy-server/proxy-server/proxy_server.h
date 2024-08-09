#ifndef PROXY_SERVER_H
#define PROXY_SERVER_H

#include "../constants.h"
#include "../db_shell/db_shell.h"
#include <csignal>
#include <netinet/in.h>
#include <sqlite3.h>
#include <string>
#include <sys/socket.h>
#include <vector>

class ProxyServer {
public:
  explicit ProxyServer(int port, int backlog_size = 10);
  ~ProxyServer() {
      close(server_socket_);
};
  void Start();
  void Stop();

private:
  void HandleClient(int client_socket);
  std::string LogSqlQuery(const std::string &query);

private:
  int server_socket_{};
  int port_{};
  int backlog_size_{10};
  std::vector<int> client_sockets_{};
  struct sockaddr_in sockaddr_in_{};

  DBShell db_{constants::kDbname};
};

#endif // PROXY_SERVER_H
