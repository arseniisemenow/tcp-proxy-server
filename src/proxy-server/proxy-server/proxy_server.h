#include "../constants.h"
#include "../db_shell/db_shell.h"
#include <netinet/in.h>
#include <sqlite3.h>
#include <string>
#include <vector>

class ProxyServer {
public:
  explicit ProxyServer(int port);
  ~ProxyServer() = default;
  void Start();
  void Stop();

private:
  void HandleClient(int client_socket);
  std::string LogSqlQuery(const std::string &query);

private:
  int server_socket_{};
  int port_{};
  std::vector<int> client_sockets_{};
  struct sockaddr_in sockaddr_in_ {};

  DBShell db_{constants::kDbname};
};
