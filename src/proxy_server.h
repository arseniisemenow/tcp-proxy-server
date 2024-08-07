#ifndef PROXY_SERVER_H
#define PROXY_SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>

class ProxyServer {
  public:
    ProxyServer(short local_port, const std::string& db_host, short db_port);
    void run();

  private:
    int setup_socket(short port);
    void handle_client(int client_socket);
    void log_query(const std::string& query);

    int local_socket_;
    std::string db_host_;
    short db_port_;
    std::ofstream log_file_;
};

#endif // PROXY_SERVER_H
