#include "BlockingSharedQueue.h"
#include "Lock.h"
#include "HDuplexChannel.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <string>
#include <sstream>
#include <list>

using namespace smstruct;

const std::string BlockingSharedQueue::BASE_PATH = "/tmp/lakeconcu.bsq.";

BlockingSharedQueue::BlockingSharedQueue(unsigned int queue_id) {
  this->queue = new SharedQueue(get_segment_key(queue_id));
}

BlockingSharedQueue::BlockingSharedQueue(unsigned int queue_id, size_t s_size) {
  this->queue = new SharedQueue(get_segment_key(queue_id), s_size);
}

key_t BlockingSharedQueue::get_segment_key(unsigned int queue_id) {
  std::stringstream path_builder;

  // Construimos el path donde se almacenará el archivo que define la cola.
  path_builder << BASE_PATH << queue_id;

  // path = /tmp/lakeconcu.bsq.{id}
  this->queue_path = path_builder.str();

  // Intentamos abrir el archivo, o lo creamos si no existe.
  this->fd = open(this->queue_path.c_str(), O_CREAT);

  // Generamos y retornamos la clave usada para instanciar el segmento.
  return ftok(this->queue_path.c_str(), 0);
}

int BlockingSharedQueue::enqueue(void *data, uint32_t data_size, uint16_t p) {
  Lock(this->fd);

  // Acolamos el nuevo elemento en la estructura.
  this->queue->enqueue(data, data_size, p);
}

unsigned int BlockingSharedQueue::take(unsigned int n, std::list<void *> &l) {
  Lock(this->fd);

  // Determinamos cantidad de elementos a leer.
  uint16_t c = this->queue->count();
  uint16_t m = (n > c) ? c : n;

  if (m > 0) {
    for (unsigned int i = 0; i < m; ++i) {
      l.push_back(this->queue->next());
    }
  }

  return m;
}

BlockingSharedQueue::~BlockingSharedQueue() {
  delete this->queue;
  close(this->fd);
  unlink(this->queue_path.c_str());
}
