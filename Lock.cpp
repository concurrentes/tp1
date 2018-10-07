#include "Lock.h"

#include <sys/file.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

Lock::Lock(std::string name) {
  this->name = name + ".lock";
  this->lock.l_len = 0;
  this->lock.l_start = 0;
  this->lock.l_whence = SEEK_SET;

  this->fd = open(this->name.c_str(), O_CREAT|O_WRONLY, 0777);
  if (fd < 0) {
    exit(0);
  }

  this->lock.l_type = F_WRLCK;

  // Intento de tomar lock sobre el archivo .lock
  if (fcntl(this->fd, F_SETLKW, &(this->lock)) < 0) {
    exit(0);
  }
}

Lock::~Lock() {

  // Intento de liberar el lock sobre el archivo .lock
  this->lock.l_type = F_UNLCK;
  if (fcntl(this->fd, F_SETLKW, &(this->lock)) < 0) {
    exit(0);
  }

  // Cierre del archivo
  close(this->fd);
}
