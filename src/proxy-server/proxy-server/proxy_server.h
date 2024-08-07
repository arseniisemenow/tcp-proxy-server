#pragma once

#include <netinet/in.h>
#include <sqlite3.h>
#include <string>
#include <vector>

class ProxyServer {
  public:
    ProxyServer(int port);
    ~ProxyServer();
    void Start();
    void Stop();
    std::string ExecuteQuery(const std::string &query);
    //todo: rename
    static int Callback(void* data, int argc, char** argv, char **column_name);
  private:
    void HandleClient(int client_socket);
    std::string LogSqlQuery(const std::string& query);
    int server_socket_;
    int port_;
    std::vector<int> client_sockets_;
    struct sockaddr_in sockaddr_in_;

    sqlite3 *db_;
};
