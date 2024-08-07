#include "logger.h"
#include <fstream>
#include <iostream>

std::string Logger::log(const std::string &message) {
  std::ofstream logFile("sql_queries.log", std::ios_base::app);
  if (logFile.is_open()) {
    logFile << message << std::endl;
    return message;
  }
  logFile.close();
  return "";
}
