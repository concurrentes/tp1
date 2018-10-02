#include "Lock.h"

#include <sys/file.h>

Lock::Lock(int fd) {
  // TODO: Verificar que el file descriptor sea válido.

  this->fd = fd;

  // Bloqueamos el archivo; el lock durará mientras exista el objeto.
  flock(fd, LOCK_EX);
}

Lock::~Lock() {
  flock(this->fd, LOCK_UN);
}
