#ifndef BLOCKING_SHARED_QUEUE_H_
#define BLOCKING_SHARED_QUEUE_H_

#include "SharedQueue.h"

#include <sys/types.h>
#include <string>
#include <list>

namespace smstruct {

  /*
   * Wrapper alrededor de la estructura SharedQueue para proveer mecanismos
   * de sincronización entre procesos.
   *
   * La cola bloqueante cuenta con dos mecanismos de sincronización.
   *
   * El primero consiste en un archivo lock que es utilizado tanto para
   * operaciones de lectura y de escritura al segmento de memoria compartido.
   *
   * El segundo es un canal de comunicación tipo FIFO: los procesos que
   * quieran leer de la cola deberán leer primero el FIFO, una cantidad de
   * bytes igual a la cantidad de objetos que querrán tomar de la cola.
   *
   * La lectura del FIFO es no bloqueante; los casos pueden ser los siguientes:
   *
   * 1. No había elementos acolados, por lo que se leerán 0 bytes; en tal caso
   * no se lee nada y el proceso deberá continuar con otra tarea.
   *
   * 2. Se pretende tomar n elementos, pero se leyeron m < n bytes. En tal
   * caso, se toman solo m elementos de la cola.
   *
   * 3. Se pretende tomar n elementos, y se leen n bytes del FIFO. En tal caso,
   * se toman efectivamente n elementos de la cola y el proceso continua.
   *
   * Adicional a la lectura del FIFO, cualquier intento de operar sobre
   * la cola compartida encapsulada será protegido mediante un lock.
   *
   */
  class BlockingSharedQueue {

  private:

    static const std::string BASE_PATH;

    SharedQueue *queue;

    std::string queue_path;

    int fd;

    key_t get_segment_key(unsigned int queue_id);

  public:
    /*
     * A diferencia de la clase SharedQueue, BlockingSharedQueue recibe, en
     * vez de una clave obtenida mediante ftok, un identificador de cola
     * que se utilizará para generar los archivos necesarios para instanciar
     * los mecanismos de sincronización.
     */
    BlockingSharedQueue(unsigned int queue_id);
    BlockingSharedQueue(unsigned int queue_id, size_t segment_size);

    int enqueue(void *data, uint32_t data_size, uint16_t priority);

    /*
     * Reemplaza al next de la cola compartida SharedQueue. La diferencia
     * es que esta operación leerá hasta count elementos y guardará los
     * respectivos punteros en la lista taken.
     *
     * La función retorna la cantidad de elementos leidos, que podría no
     * ser igual a count.
     *
     */
    unsigned int take(unsigned int count, std::list<void *> &taken);

    ~BlockingSharedQueue();

  };

}

#endif
