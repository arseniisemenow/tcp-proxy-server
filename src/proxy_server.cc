#include "proxy_server.h"

ProxyServer::ProxyServer(short local_port, const std::string& db_host, short db_port)
    : db_host_(db_host), db_port_(db_port), log_file_("sql_queries.log", std::ios_base::app) {
  local_socket_ = setup_socket(local_port);
}

int ProxyServer::setup_socket(short port) {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  int opt = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(port);

  if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  if (listen(sockfd, SOMAXCONN) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  return sockfd;
}

void ProxyServer::run() {
  fd_set master_set, read_set;
  FD_ZERO(&master_set);
  FD_SET(local_socket_, &master_set);
  int max_fd = local_socket_;

  while (true) {
    read_set = master_set;

    if (select(max_fd + 1, &read_set, nullptr, nullptr, nullptr) < 0) {
      perror("select");
      exit(EXIT_FAILURE);
    }

    for (int fd = 0; fd <= max_fd; ++fd) {
      if (FD_ISSET(fd, &read_set)) {
        if (fd == local_socket_) {
          sockaddr_in client_addr;
          socklen_t client_len = sizeof(client_addr);
          int client_socket = accept(local_socket_, (struct sockaddr*)&client_addr, &client_len);
          if (client_socket < 0) {
            perror("accept");
          } else {
            fcntl(client_socket, F_SETFL, O_NONBLOCK);
            FD_SET(client_socket, &master_set);
            if (client_socket > max_fd) {
              max_fd = client_socket;
            }
          }
        } else {
          handle_client(fd);
          close(fd);
          FD_CLR(fd, &master_set);
        }
      }
    }
  }
}

void ProxyServer::handle_client(int client_socket) {
  char buffer[4096];
  int bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
  if (bytes_read <= 0) {
    return;
  }
  buffer[bytes_read] = '\0';

  log_query(buffer);

  int server_socket = setup_socket(0);
  sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(db_port_);
  inet_pton(AF_INET, db_host_.c_str(), &server_addr.sin_addr);

  if (connect(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    perror("connect");
    close(server_socket);
    return;
  }

  send(server_socket, buffer, bytes_read, 0);

  bytes_read = recv(server_socket, buffer, sizeof(buffer) - 1, 0);
  if (bytes_read > 0) {
    send(client_socket, buffer, bytes_read, 0);
  }

  close(server_socket);
}

void ProxyServer::log_query(const std::string& query) {
  log_file_ << query << std::endl;
}
