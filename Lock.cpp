#include "Lock.h"

#include <sys/file.h>
#include <unistd.h>
#include "Logger.h"
#include <string.h>
#include <errno.h>

Lock::Lock(std::string name) {
  this->name = name + ".lock";
  this->lock.l_len = 0;
  this->lock.l_start = 0;
  this->lock.l_whence = SEEK_SET;

  this->fd = open(this->name.c_str(), O_CREAT|O_WRONLY, 0777);
  if (fd < 0) {
    LOG(LOG_ERROR, "Error tratando de crear archivo de lock " + this->name);
    exit(0);
  }
  std::string message = "Intento de tomar lock";
  LOG(LOG_INFO, message)

  this->lock.l_type = F_WRLCK;

  // Intento de tomar lock sobre el archivo .lock
  if (fcntl(this->fd, F_SETLKW, &(this->lock)) < 0) {
    LOG(LOG_ERROR, "Error tomando lock, errno: " + std::to_string(errno));
    exit(0);
  }
}

Lock::~Lock() {

  // Intento de liberar el lock sobre el archivo .lock
  this->lock.l_type = F_UNLCK;
  std::string message = "Intentando liberar lock";
  LOG(LOG_INFO, message)
  if (fcntl(this->fd, F_SETLKW, &(this->lock)) < 0) {
    LOG(LOG_ERROR, "Error liberando lock, errno: " + std::to_string(errno));
    exit(0);
  }

  // Cierre del archivo
  close(this->fd);
}
