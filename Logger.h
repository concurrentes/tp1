#ifndef LOGGER_H_
#define LOGGER_H_

#define LOG_INFO "INFO"
#define LOG_WARN "WARN"
#define LOG_ERROR "ERROR"
#define LOG_DEBUG "DEBUG"

#define LOG(tag, message) {                                           \
  Logger::get_instance().log(tag) << __FILE__ << " - " << message;    \
  Logger::get_instance().endl();                                      \
}

#include <string>
#include <iostream>
#include <fstream>

class Logger {

public:

  static void set_debug();

  static const Logger &get_instance();

  const Logger &log(const std::string &tag) const;

  const Logger &operator<<(const unsigned int &n) const;

  const Logger &operator<<(const std::string &message) const;

  void endl() const;

private:

  Logger(std::string filename);

  Logger(Logger const &);

  ~Logger();

  void operator=(Logger const &);

  static bool in_debug;

  std::ostream& mOutputStream;

  static const std::string LOG_PATH;

  std::string filename;
};

#endif
