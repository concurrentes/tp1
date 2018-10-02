#include "HDuplexChannel.h"
#include "Lock.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>

#include <sstream>

const std::string HDuplexChannel::BASE_PATH = "/tmp/lakeconcu.hdc.";

HDuplexChannel::HDuplexChannel(unsigned int id, Mode mode) {
  std::stringstream path_builder;

  // Construimos el path donde se almacenará el FIFO.
  path_builder << BASE_PATH << id;

  // path = /tmp/lakeconcu.hdc.{id}
  this->path = path_builder.str();

  // Creamos el FIFO en el directorio indicado.
  int result = mknod(this->path.c_str(), 0600 | S_IFIFO, 0);

  if (result == -1 && errno != EEXIST) {
    this->fd = -1;
    // TODO: ERROR
  } else {
    open_file(this->path.c_str(), (mode == READ) ? O_RDONLY : O_WRONLY);
  }

}

void HDuplexChannel::open_file(const char *path, int mode) {
  this->fd = open(path, mode);
}

int HDuplexChannel::push(void *data, ssize_t count) {
  return write(this->fd, data, count);
}

void HDuplexChannel::recv(void *buff, ssize_t count) {
  Lock(this->fd);

  ssize_t r = 0;
  while (r < count) {
    r += read(this->fd, buff, count);
  }
}

ssize_t HDuplexChannel::recv_or_continue(void *buff, ssize_t count) {
  Lock(this->fd);

  // Obtenemos los flags asociados al descriptor, sin nonblock.
  int flags = fcntl(this->fd, F_GETFL) & (~O_NONBLOCK);

  // Ponemos el FIFO en modo no bloqueante mientras dure la operación.
  fcntl(this->fd, F_SETFL, flags | O_NONBLOCK);

  // Leemos del FIFO en forma no bloqueante.
  ssize_t r = read(this->fd, buff, count);

  // Ponemos al FIFO nuevamente en modo bloqueante.
  fcntl(this->fd, F_SETFL, flags);

  // Retornamos la cantidad de bytes leídos.
  return r;
}

void HDuplexChannel::shutdown() {
  if (this->fd != -1) {
    close(this->fd); this->fd = -1;
  }
}

HDuplexChannel::~HDuplexChannel() {
  shutdown();
  unlink(this->path.c_str());
}
