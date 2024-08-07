#include "proxy_server.h"
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include "../logger/logger.h"

ProxyServer::ProxyServer(int port) : port_(port) {
  server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket_ < 0) {
    perror("Failed to create socket");
    exit(EXIT_FAILURE);
  }

  memset(&sockaddr_in_, 0, sizeof(sockaddr_in_));
  sockaddr_in_.sin_family = AF_INET;
  sockaddr_in_.sin_addr.s_addr = INADDR_ANY;
  sockaddr_in_.sin_port = htons(port);

  if (bind(server_socket_, (struct sockaddr*)&sockaddr_in_, sizeof(sockaddr_in_)) < 0) {
    perror("Bind failed");
    close(server_socket_);
    exit(EXIT_FAILURE);
  }
}

void ProxyServer::Start() {
  if (listen(server_socket_, 5) < 0) {
    perror("Listen failed");
    close(server_socket_);
    exit(EXIT_FAILURE);
  }

  std::cout << "Server started on port " << port_ << std::endl;

  while (true) {
    int clientSocket = accept(server_socket_, nullptr, nullptr);
    if (clientSocket < 0) {
      perror("Accept failed");
      continue;
    }
    HandleClient(clientSocket);
  }
}

void ProxyServer::Stop() {
  for (int client_socket : client_sockets_) {
    close(client_socket);
  }
  close(server_socket_);
}

void ProxyServer::HandleClient(int client_socket) {
  char buffer[1024];
  while (true) {
    ssize_t bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read <= 0) break;
    buffer[bytes_read] = '\0';
    std::string query(buffer);
    LogSqlQuery(query);

    std::string response = "Query received: " + query;
    send(client_socket, response.c_str(), response.size(), 0);
  }
  close(client_socket);
}

std::string ProxyServer::LogSqlQuery(const std::string &query) {
  Logger::log(query);
  return query;
}
