#include "logger.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iomanip>

std::tm GetCurrentTime() {
  auto now = std::chrono::system_clock::now();
  auto time = std::chrono::system_clock::to_time_t(now);
  std::tm tm = *std::localtime(&time);
  return tm;
}

std::string Logger::log(const std::string &message) {
  try {
    std::ofstream log_file("sql_queries.log", std::ios_base::app);

    if (!log_file.is_open()) {
      throw std::ios_base::failure("Failed to open log file");
    }

    auto tm = GetCurrentTime();

    std::ostringstream timestamp_stream;
    timestamp_stream << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

    log_file << "[" << timestamp_stream.str() << "] " << message << std::endl;

    return message;
  } catch (const std::ios_base::failure &e) {
    std::cerr << "Logging error: " << e.what() << std::endl;
    return "";
  }
}
