#include "SharedQueue.h"

#include <sys/types.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

using namespace smstruct;

const int SharedQueue::SMQ_ERROR_NOT_ENOUGH_SPACE = 0x01;
const int SharedQueue::SMQ_ERROR_FRAGMENTATION_ISSUES = 0x02;
const int SharedQueue::SMQ_ERROR_EMPTY_QUEUE = 0x03;
const int SharedQueue::SMQ_ERROR_COULD_NOT_GET_SHM = 0x04;

const uint32_t SharedQueue::DEFAULT_SEGMENT_SIZE = 16*1024; // 16 kB

const uint32_t SharedQueue::DATA_OFFSET = 128; // bytes

SharedQueue::SharedQueue(key_t key) {
  initialize(key, DEFAULT_SEGMENT_SIZE);
}

SharedQueue::SharedQueue(key_t key, size_t segment_size) {
  initialize(key, segment_size);
}

void SharedQueue::initialize(key_t key, size_t segment_size) {

  // Intentamos encontrar el segmento de memoria compartida.
  this->shmid = shmget(key, segment_size, 0644);

  if (this->shmid == -1 && errno == ENOENT) {
    /*
     * El segmento no existe; procedemos a reservarlo y a inicializar la
     * estructura administrativa de la cola en la base del segmento.
     */
    this->shmid = create_segment(key, segment_size);

  } else if (this->shmid == -1) {
    errno = SMQ_ERROR_COULD_NOT_GET_SHM; // TODO: Lanzar exepción.
  } else {
    this->ptr_segment = attach();
  }

}

int SharedQueue::create_segment(key_t key, size_t segment_size) {
  /*
   * TODO: Manejar casos de error, lanzar excepciones, limpiar recursos.
   * TODO: Verificar caso en el que segment_size < DATA_OFFSET.
   */

  // Reservamos el segmento de memoria compartida.
  this->shmid = shmget(key, segment_size, 0644 | IPC_CREAT | IPC_EXCL);

  // Adosamos el proceso al segmento de memoria compartida.
  this->ptr_segment = attach();

  // Limpiamos los datos del segmento por completo.
  bzero(this->ptr_segment, segment_size);

  // Inicializamos los datos administrativos de la estructura.
  queue_header_t *q_header = (queue_header_t *)(this->ptr_segment);

  q_header->count = 0;
  q_header->data_offset = DATA_OFFSET;
  q_header->ptr_tail = 0;
  q_header->ptr_head = 0;

  /*
   * En principio, el espacio consumido en el segmento es el espacio destinado
   * a los datos administrativos de la estructura. El especio entre el eje
   * superior de la estructura queue_header_t y el inicio de los datos es el
   * espacio reservado para la tabla de prioridades.
   */
  q_header->consumed = DATA_OFFSET;
  q_header->capacity = segment_size;

  return this->shmid;
}

uint8_t *SharedQueue::attach() {
  // TODO: Manejar casos de error, lanzar excepciones y limpiar recursos.
  return (uint8_t *)(shmat(this->shmid, NULL, 0));
}

int SharedQueue::enqueue(void *data, uint32_t data_size, uint16_t priority) {
  queue_header_t *ptr_queue_header = (queue_header_t *)(this->ptr_segment);

  if (ptr_queue_header->count == 0) {
    return enqueue_on_empty(data, data_size, priority);
  } else {
    return enqueue_on_non_empty(data, data_size, priority);
  }

}

int SharedQueue::enqueue_on_empty(void *data, uint32_t d_size, uint16_t p) {
  queue_header_t *q_header = (queue_header_t *)(this->ptr_segment);

  /*
   * Calculamos el consumo espacial para almacenar tanto datos de usuario
   * como datos administrativos del nodo que estamos insertando.
   */
  const uint32_t SPACE_CONSUMPTION = SPACE_REQUIREMENTS(d_size);

  /*
   * Determinamos si el espacio disponible en la sección de datos del segmento
   * es suficiente para almacenar SPACE_CONSUMPTION bytes.
   */
  if (SPACE_CONSUMPTION <= SPACE_AVAILABLE(q_header)) {
    enqueue_after(NULL, data, d_size, p);
    errno = 0;
  } else {
    errno = SMQ_ERROR_NOT_ENOUGH_SPACE;
  }

  return errno == 0 ? 1 : -1;
}

int SharedQueue::enqueue_on_non_empty(void *data, uint32_t size, uint16_t p) {
  queue_header_t *q_header = (queue_header_t *)(this->ptr_segment);

  /*
   * Calculamos el consumo espacial para almacenar tanto datos de usuario
   * como datos administrativos del nodo que estamos insertando.
   */
  const uint32_t SPACE_CONSUMPTION = SPACE_REQUIREMENTS(size);

  /*
   * Ya hay algún elemento en la cola. Debemos determinar la posición del
   * último elemento acolado e intentar agregar el siguiente.
   */
  node_header_t *tail = TAIL_NODE(q_header);
  node_header_t *head = HEAD_NODE(q_header);

  /*
   * Efectuamos algunas validaciones para determinar el lugar donde el nodo
   * debería ser insertado, y si es posible hacerlo.
   */

  const uint8_t ENUF_SPACE = 0x01;
  const uint8_t FREE_AFTER = 0x02;
  const uint8_t FREE_START = 0x04;

  uint8_t state = 0x0;

  if (SPACE_CONSUMPTION <= SPACE_AVAILABLE(q_header)) {
    state |= ENUF_SPACE;
  } else {
    errno = SMQ_ERROR_NOT_ENOUGH_SPACE;
    return -1;
  }

  if (head <= tail) {

    if (SPACE_CONSUMPTION <= SPACE_AFTER(tail, q_header)) {
      state |= FREE_AFTER;
    } else if (SPACE_CONSUMPTION <= SPACE_BEFORE(head, q_header)) {
      state |= FREE_START;
    }

  } else if (SPACE_CONSUMPTION <= SPACE_FROM_TO(tail, head)) {
    state |= FREE_AFTER;
  }

  switch(state) {
    case FREE_AFTER | ENUF_SPACE:
      tail->ptr_next = DELTA(tail, NODE_TOP_AXIS(tail));
      break;
    case FREE_START | ENUF_SPACE:
      tail->ptr_next = REWIND(tail, q_header);
      break;
  }

  if (state & FREE_AFTER || state & FREE_START) {
    enqueue_after(tail, data, size, p);
    errno = 0;
  } else {
    errno = SMQ_ERROR_FRAGMENTATION_ISSUES;
  }

  return errno == 0 ? 1 : -1;
}

void SharedQueue::enqueue_after(nh_t *prev, void *data, uint32_t size, uint16_t p) {

  queue_header_t *queue_header = (queue_header_t *)(this->ptr_segment);

  /*
   * Creamos un nuevo nodo con los datos adecuados.
   */
  node_header_t *current;

  if (prev == NULL) {
    current = (node_header_t *)(DATA_SECTION(queue_header));
    current->ptr_prev = 0;
  } else {
    current = NEXT(prev);
    current->ptr_prev = DELTA(current, prev);
  }

  current->priority = p;
  current->flags = 0;
  current->data_size = size;
  current->ptr_next = 0;

  memcpy(NODE_DATA(current), data, size);

  /*
   * Ajustamos los datos administrativos de la estructura.
   */
  queue_header->count++;
  queue_header->consumed += SPACE_REQUIREMENTS(size);

  if (prev == NULL) {
    queue_header->ptr_tail = queue_header->data_offset;
    queue_header->ptr_head = queue_header->data_offset;
  } else {
    queue_header->ptr_tail += prev->ptr_next;
  }

  /*
   * Ajustamos la cuenta de los nodos prioritizados.
   */
  P_TABLE(queue_header)[p]++;
}

void *SharedQueue::next() {
  queue_header_t *q_header = (queue_header_t *)(this->ptr_segment);

  if (q_header->count > 0) {
    return pop_next(q_header);
  } else {
    errno = SMQ_ERROR_EMPTY_QUEUE;
    return NULL;
  }
}

uint16_t SharedQueue::count() {
  queue_header_t *q_header = (queue_header_t *)(this->ptr_segment);
  return q_header->count;
}

void *SharedQueue::pop_next(queue_header_t *queue_header) {
  /*
   * Determinamos la prioridad del elemento prioritario en la cola.
   */
  uint16_t max_priority = find_max_priority(queue_header);

  /*
   * Comenzamos a buscar el primer elemento prioritario desde el frente.
   */
  node_header_t *head = HEAD_NODE(queue_header);

  for (node_header_t *current = head;; current = NEXT(current)) {

    if (current->priority == max_priority) {
      /*
       * Encontramos el nodo que hay que desacolar; lo sacamos y actualizamos
       * los datos administrativos de la estructura.
       */
      return pop_node(queue_header, current);
    }

  }

}

uint16_t SharedQueue::find_max_priority(queue_header_t *q_header) {
  uint16_t max_priority = 0;

  /*
   * Determinamos la posición de la tabla P en la cabecera del segmento.
   */
  uint16_t *p_table = P_TABLE(q_header);

  /*
   * Evaluamos cuántos nodos hay en cada categoría de prioridad.
   */
  for (uint16_t p = 0, seen = 0; seen < q_header->count; seen += p_table[p++]) {
    max_priority = p;
  }

  return max_priority;
}

void *SharedQueue::pop_node(queue_header_t *q_header, node_header_t *node) {

  // Creamos un buffer en el heap local para almacenar los datos en el nodo.
  void *buffer = malloc(node->data_size);

  // Copiamos los datos del nodo al buffer local.
  memcpy(buffer, NODE_DATA(node), node->data_size);

  /*
   * Actualizamos los datos administrativos de la estructura. El procedimiento
   * depende de de la posición relativa del nodo en la estructura.
   */
  if (node == HEAD_NODE(q_header)) {
    NEXT(node)->ptr_prev = 0; q_header->ptr_head += node->ptr_next;
  } else if (node == TAIL_NODE(q_header)) {
    PREV(node)->ptr_next = 0; q_header->ptr_tail += node->ptr_prev;
  } else {
    node_header_t *prev = PREV(node);
    node_header_t *next = NEXT(node);

    if (prev != node) {
      prev->ptr_next += node->ptr_next;
    }
    if (next != node) {
      next->ptr_prev += node->ptr_prev;
    }
  }

  P_TABLE(q_header)[node->priority]--;

  q_header->count--;
  q_header->consumed -= SPACE_REQUIREMENTS(node->data_size);

  return buffer;
}

SharedQueue::~SharedQueue() {
  // TODO: Manejar posibles casos de error; lanzar excepciones o algo.

  // Se desadosa el proceso del segmento de memoria compartida.
  shmdt(this->ptr_segment);

  /*
   * Utilizamos shmctl para consultar la cantidad de procesos todavía
   * adosados al segmento, para saber si debemos o no eliminar el segmento.
   */
  struct shmid_ds stat;

  shmctl(this->shmid, IPC_STAT, &stat);

  // Eliminamos el segmento si la cantidad de procesos adosados es cero.
  if (stat.shm_nattch == 0) {
    shmctl(this->shmid, IPC_RMID, NULL);
  }

}
