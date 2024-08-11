#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

int ConnectToServer(const short port, const char *address);
void SendSqlQuery(const int client_socket,
                  const std::string &query) {
  send(client_socket, query.c_str(), query.size(), 0);
  char buffer[1024] = {0};
  ssize_t bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
  if (bytes_read > 0) {
    buffer[bytes_read] = '\0';
    std::cout << "Server response: " << buffer << std::endl;
  }

}
int ConnectToServer(const short port, const char *address) {
  int client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket < 0) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in server_addr {};
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = inet_addr(address);

  if (connect(client_socket, (struct sockaddr *)&server_addr,
              sizeof(server_addr)) < 0) {
    perror("Connect failed");
    close(client_socket);
    exit(EXIT_FAILURE);
  }
  return client_socket;
}

int main() {
  const short server_port = 8080;
  const char *server_address = "127.0.0.1";

  std::string user_query;

  int client_socket = ConnectToServer(server_port, server_address);

  std::cout << "Connected to the server on port " << server_port << "\n";
  std::cout << "Type your SQL query and press Enter to execute (type 'exit' to "
               "quit):\n";

  while (true) {
    std::cout << "> ";
    std::getline(std::cin, user_query);
    if (user_query.empty()) {
      continue;
    }
    if (user_query == "exit") {
      break;
    }

    SendSqlQuery(client_socket, user_query);
  }
  close(client_socket);

  std::cout << "Exiting client...\n";
  return 0;
}
