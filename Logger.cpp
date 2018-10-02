#include "Logger.h"

#include <iostream>
#include <string>
#include <chrono>
#include <unistd.h>


using namespace std::chrono;
using time_stamp = time_point<system_clock, microseconds>;

const Logger &Logger::get_instance() {
  static const Logger logger;
  return logger;
}

const Logger &Logger::log(const std::string &tag) const {
  time_stamp ts = time_point_cast<microseconds>(system_clock::now());
  time_t now = system_clock::to_time_t(ts);
  std::cout << now << " PID: " << getpid() << " " << tag << "\t: ";
  return *this;
}

const Logger &Logger::operator<<(const std::string &message) const {
  std::cout << message;
  return *this;
}

const Logger &Logger::operator<<(const unsigned int &n) const {
  std::cout << n;
  return *this;
}

void Logger::endl() const {
  std::cout << std::endl;
}
