#include "SharedStack.h"

#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(push, 1)

/*
 * sq_base_t
 *
 * Registro que representa la base de la pila.
 * Guarda datos administrativos que se utilizan para
 * gestionar la estructura de datos.
 */
typedef struct {

  /*
   * Cantidad de elementos en la pila.
   */
  uint32_t count;

  /*
   * Cantidad total de bytes consumidos para almacenar datos,
   * incluyendo datos administrativos de la pila.
   */
  uint32_t usage;

  /*
   * Espacio total en el buffer compartido.
   */
  uint32_t segment_size;

  /*
   * Byte en la región de memoria compartida a partir del
   * cuál comienzan a guardarse datos en el stack; offset
   * hasta el primer elemento del stack.
   */
  uint32_t data_offset;

  /*
   * Offset hasta el inicio del último elemento del stack.
   */
  uint32_t end_offset;

} __attribute__((packed)) sq_base_t;

/*
 * sq_node_header_t
 *
 * Datos administrativos correspondientes a un nodo de la pila.
 */
typedef struct {

  /*
   * Tamaño de los datos (no administrativos) almacenados en el nodo.
   */
  uint32_t data_size;

  /*
   * Offset hasta el siguiente elemento de la pila.
   */
  uint32_t next;

  /*
   * Cantidad de bytes hasta el elemento anterior de la pila.
   * Debe ser un número positivo.
   */
  uint32_t prev;

} __attribute__((packed)) sq_node_header_t;

#pragma pack(pop)



SharedStack::SharedStack(key_t key) {
  this->initialize(key, DEFAULT_BUFFER_SIZE);
}

SharedStack::SharedStack(key_t key, size_t size) {
  this->initialize(key, size);
}

/*
 * initialize
 *
 * TODO: manejar excepciones y casos de error.
 *
 */
int SharedStack::initialize(key_t key, size_t size) {

  // Crea el segmento de memoria compartida, si no existe.
  this->shmid = shmget(key, size, 0644 | IPC_CREAT | IPC_EXCL);

  // Adosa al proceso al segmento de memoria compartida.
  this->segment = shmat(this->shmid, NULL, 0);

  // Inicializa la estructura de datos.

  sq_base_t *base = (sq_base_t*)this->segment;

  base->count = 0;
  base->segment_size = size;
  base->usage = DATA_OFFSET;
  base->data_offset = DATA_OFFSET;
  base->end_offset = DATA_OFFSET;

  // Inicializa un nodo sin datos.
  sq_node_header_t *null_node =
    (sq_node_header_t*)((uint8_t *)this->segment + base->data_offset);

  null_node->data_size = 0;
  null_node->next = 0;
  null_node->prev = 0;
}

void SharedStack::push(void *data, size_t size) {
  sq_base_t *base = (sq_base_t*)this->segment;

  // Se determina el tamaño total del nodo, incluido el header.
  uint32_t node_size = sizeof(sq_node_header_t) + size;

  if (base->segment_size < base->usage + node_size) {
    /*
     * TODO: no se puede apilar; espacio insuficiente
     * Lanzar excepcion o retornar algún tipo de error.
     */
  }

  // Se determina la posición del último nodo en la pila.
  sq_node_header_t *last =
    (sq_node_header_t *)((uint8_t *)this->segment + base->end_offset);

  // Puntero al nuevo nodo de la estructura.
  sq_node_header_t *node =
    (sq_node_header_t *)((uint8_t *)last + last->next);

  // Inicializa los datos del nuevo nodo.
  node->prev = last->next;
  node->next = node_size;
  node->data_size = size;

  // Copia los datos al nodo compartido.
  memcpy((uint8_t *)node + sizeof(sq_node_header_t), data, size);

  // Actualiza los datos administrativos en la base de la estructura.

  if (base->count > 0) {
    base->end_offset += last->next;
  }

  base->usage += node_size;
  base->count++;
}

void *SharedStack::take() {
  sq_base_t *base = (sq_base_t*)this->segment;

  // Se determina la posición del último nodo en la pila.
  sq_node_header_t *last =
    (sq_node_header_t*)((uint8_t *)this->segment + base->end_offset);

  // Se reserva espacio en el heap local para guardar los datos.
  void *local_buffer = malloc(last->data_size);

  // Se copian los datos al heap local.
  void *data_pointer = (uint8_t *)last + sizeof(sq_node_header_t);

  memcpy(local_buffer, data_pointer, last->data_size);

  // Se desapila el último elemento.
  sq_node_header_t *prev =
    (sq_node_header_t *)((uint8_t *)last - last->prev);

  base->usage -= last->next;
  base->count--;

  if (base->count > 0) {
    base->end_offset -= prev->next;
  }

  return local_buffer;
}

/*
 * ~SharedStack
 *
 * TODO: manejar excepciones y casos de error.
 *
 */
SharedStack::~SharedStack() {
  int result = -1;

  // Separa al proceso del segmento compartido.
  result = shmdt(this->segment);

  // Consulta la cantidad de usuarios adosados al segmento.
  struct shmid_ds stat;

  result = shmctl(this->shmid, IPC_STAT, &stat);

  // Si no quedan usuarios del segmento, lo elimina.
  if (stat.shm_nattch == 0) {
    result = shmctl(this->shmid, IPC_RMID, NULL);
  }
}
