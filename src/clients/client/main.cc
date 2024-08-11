#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

void SendSqlQuery(const short port, const char *address,
                  const std::string &query) {
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

  send(client_socket, query.c_str(), query.size(), 0);
  char buffer[1024] = {0};
  ssize_t bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
  if (bytes_read > 0) {
    buffer[bytes_read] = '\0';
    std::cout << "Server response: " << buffer << std::endl;
  }

  close(client_socket);
}

int main() {
  const short server_port = 8080;
  const char *server_address = "127.0.0.1";

  const char *drop_table_sql = "DROP TABLE IF EXISTS users;";
  const char *create_table_sql = "CREATE TABLE IF NOT EXISTS users(id INTEGER PRIMARY KEY AUTOINCREMENT,name TEXT NOT NULL,age INTEGER NOT NULL);";
  const char *insert_data_sql = R"(
        INSERT INTO users (name, age) VALUES ('Oliver', 25);
        INSERT INTO users (name, age) VALUES ('Wladimir', 30);
        INSERT INTO users (name, age) VALUES ('Kris', 35);
    )";
  const char *select_data_sql = R"(SELECT * FROM users;)";
  const char *select_sqlite_version_sql = R"(SELECT sqlite_version();)";


  //  SendSqlQuery(server_port, server_address, drop_table_sql);
  SendSqlQuery(server_port, server_address, select_data_sql);
  SendSqlQuery(server_port, server_address, create_table_sql);
  SendSqlQuery(server_port, server_address, select_data_sql);
  SendSqlQuery(server_port, server_address, insert_data_sql);
  SendSqlQuery(server_port, server_address, select_data_sql);

  SendSqlQuery(server_port, server_address, select_sqlite_version_sql);
  return 0;
}
