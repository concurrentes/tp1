#ifndef SHARED_QUEUE_
#define SHARED_QUEUE_

#include <sys/types.h>
#include <stdlib.h>

class SharedStack {

public:

  /*
   * Crea una pila con un buffer compartido de tamaño por defecto.
   */
  SharedStack(key_t key);

  /*
   * Crea una pila con un buffer compartido del tamaño indicado.
   */
  SharedStack(key_t key, size_t size);

  /*
   * push
   *
   * Crea un nuevo nodo en la pila compartida.
   */
  void push(void *data, size_t size);

  /*
   * take
   *
   * Toma el siguiente elemento de la pila y lo desapila.
   * Devuelve un puntero local a los datos, los cuales serán
   * copiados al heap local.
   */
  void *take();

  /*
   * Libera los recursos asociados a la pila.
   */
  ~SharedStack();

private:

  /*
   * Tamaño por defecto del buffer para almacenar la pila.
   * La pila se guarda en memoria compartida.
   */
  const size_t DEFAULT_BUFFER_SIZE = 16384;

  /*
   * Tamaño reservado para datos administrativos en la
   * base de la estructura de datos, en bytes.
   */
  const size_t DATA_OFFSET = 128;

  /*
   * Identificador del segmento de memoria compartida.
   */
  int shmid;

  /*
   * Puntero al segmento de memoria compartida.
   */
  void *segment;

  int initialize(key_t key, size_t size);

};

#endif
