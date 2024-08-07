#pragma once

#include <string>
#include <vector>
#include <netinet/in.h>

class ProxyServer {
  public:
    ProxyServer(int port);
    void Start();
    void Stop();
  private:
    void HandleClient(int client_socket);
    std::string LogSqlQuery(const std::string &query);
    int server_socket_;
    int port_;
    std::vector<int> client_sockets_;
    struct sockaddr_in sockaddr_in_;
};
