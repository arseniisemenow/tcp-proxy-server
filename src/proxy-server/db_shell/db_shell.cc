#include "db_shell.h"
#include "../constants.h"
#include <iostream>
#include <sqlite3.h>

DBShell::DBShell() { SetUp(constants::kDbname); }
DBShell::DBShell(const std::string& dbname) { SetUp(dbname); }

DBShell::~DBShell() { TearDown(); }

int DBShell::SetUp(const std::string &dbname) {
  int error_code = sqlite3_open(dbname.c_str(), &db_);
  return error_code;
}

int DBShell::TearDown() {
  int error_code = sqlite3_close(db_);
  return error_code;
}

std::string DBShell::ExecuteQuery(const std::string &query) {
  char *error_message{nullptr};
  std::string response{};
  sqlite3_exec(db_, query.c_str(), Callback, &response, &error_message);
  response = HandleQueryError(error_message, response);
  response = HandleEmptyResponse(response);
  return response;
}
std::string &DBShell::HandleQueryError(char *error_message,
                                       std::string &response) const {
  if (error_message) {
    response = "SQL error: ";
    response += error_message;
    sqlite3_free(error_message);
  }
  return response;
}
std::string &DBShell::HandleEmptyResponse(std::string &response) const {
  if (response.empty()) {
    response += "NULL";
  }
  return response;
}
int DBShell::Callback(void *data, int argc, char **argv, char **column_name) {
  std::string *response = static_cast<std::string *>(data);
  for (int i = 0; i < argc; ++i) {
    *response += column_name[i];
    *response += " = ";
    *response += argv[i] ? argv[i] : "NULL";
    *response += "\n";
  }
  return 0;
}