#include "Logger.h"

#include <iostream>
#include <string>
#include <chrono>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include "Lock.h"

using namespace std::chrono;
using time_stamp = time_point<system_clock, microseconds>;

bool Logger::in_debug = false;
const std::string Logger::LOG_PATH = "/tmp/lakeconcu.log";

const Logger &Logger::get_instance() {
  static const Logger logger(in_debug ? LOG_PATH : "");
  return logger;
}

const Logger &Logger::log(const std::string &tag) const {
  time_stamp ts = time_point_cast<microseconds>(system_clock::now());
  time_t now = system_clock::to_time_t(ts);
  if (in_debug) {
    Lock(this->filename);
    mOutputStream << now << " PID: " << getpid() << " " << tag << "\t: ";
    return *this;
  }
  mOutputStream << now << " PID: " << getpid() << " " << tag << "\t: ";
  return *this;
}

const Logger &Logger::operator<<(const std::string &message) const {
  if (in_debug) {
    Lock(this->filename);
    mOutputStream << message;
    return *this;
  }
  mOutputStream << message;
  return *this;
}

const Logger &Logger::operator<<(const unsigned int &n) const {
  if (in_debug) {
    Lock(this->filename);
    mOutputStream << n;
    return *this;
  }
  mOutputStream << n;
  return *this;
}

void Logger::endl() const {
  if (in_debug) {
    Lock(this->filename);
    mOutputStream << std::endl;
  } else {
    mOutputStream << std::endl;
  }
}

void Logger::set_debug() {
  in_debug = true;
}

Logger::Logger(std::string filename) : mOutputStream((!filename.compare(""))?std::cout:*(new std::ofstream(filename.c_str()))) {
  std::stringstream builder;
  builder << LOG_PATH;

  this->filename = builder.str();

}


Logger::~Logger() {
  if (&mOutputStream != &std::cout) {
    delete(&mOutputStream);
  }
}