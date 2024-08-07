#include "proxy_server.h"
#include <cstring>
#include <iostream>
#include <sqlite3.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../constants.h"
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

  if (bind(server_socket_, (struct sockaddr *) &sockaddr_in_, sizeof(sockaddr_in_)) < 0) {
    perror("Bind failed");
    close(server_socket_);
    exit(EXIT_FAILURE);
  }

  if (sqlite3_open(constants::dbname, &db_) != SQLITE_OK) {
  }
  // todo: handle error
}

ProxyServer::~ProxyServer() {
  // todo: handle return value
  (void) sqlite3_close(db_);
}

void ProxyServer::Start() {
  if (listen(server_socket_, 5) < 0) {
    perror("Listen failed");
    close(server_socket_);
    exit(EXIT_FAILURE);
  }

  std::cout << "Server started on port " << port_ << "\n";

  while (true) {
    int client_socket = accept(server_socket_, nullptr, nullptr);
    if (client_socket < 0) {
      perror("Accept failed");
      continue;
    }
    HandleClient(client_socket);
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
    if (bytes_read <= 0) {
      break;
    }
    buffer[bytes_read] = '\0';
    std::string query(buffer);
    LogSqlQuery(query);

    std::string response = ExecuteQuery(query);
//    std::string response = "Temp response";
    send(client_socket, response.c_str(), response.size(), 0);
  }
  close(client_socket);
}

std::string ProxyServer::LogSqlQuery(const std::string &query) {
  Logger::log(query);
  return query;
}
std::string ProxyServer::ExecuteQuery(const std::string &query){
  char* error_message = nullptr;
  std::string response{};
  sqlite3_exec(db_, query.c_str(), Callback, &response, &error_message);
  if (error_message){
    response = "SQL error: ";
    response += error_message;
    sqlite3_free(error_message);
  }
  return response;
}
int ProxyServer::Callback(void *data, int argc, char **argv, char **column_name) {
  std::string * response = static_cast<std::string*>(data);
  for (int i = 0; i < argc; ++i) {
    *response += column_name[i];
    *response += " = ";
    *response += argv[i] ? argv[i] : "NULL";
    *response += "\n";
  }
  return 0;
}


