#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

void sendSQLQuery(const std::string& query) {
  int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (clientSocket < 0) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in serverAddr;
  memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(8080);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
    perror("Connect failed");
    close(clientSocket);
    exit(EXIT_FAILURE);
  }

  send(clientSocket, query.c_str(), query.size(), 0);
  char buffer[1024] = {0};
  ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
  if (bytesRead > 0) {
    buffer[bytesRead] = '\0';
    std::cout << "Server response: " << buffer << std::endl;
  }

  close(clientSocket);
}

int main() {
  sendSQLQuery("SELECT * FROM persons;");
//  sendSQLQuery("INSERT INTO users (name, age) VALUES ('Alice', 30);");
  return 0;
}
