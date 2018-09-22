#ifndef SHARED_QUEUE_H_
#define SHARED_QUEUE_H_

#include "SharedQueueTypes.h"

#include <sys/types.h>
#include <stdint.h>

namespace smstruct {

  class SharedQueue {

  private:

    /*
     * Distancia en bytes desde el inicio del segmento de memoria compartida
     * hasta el inicio de la sección de datos de la cola.
     */
    const static uint32_t DATA_OFFSET;

    /*
     * Identificador del segmento de memoria compartida.
     */
    int shmid;

    /*
     * Puntero al inicio del segmento de memoria compartida.
     */
    uint8_t *ptr_segment;

    /*
     * Reserva o se adosa al segmento de memoria compartida, y en caso de
     * tener que crear el segmento, también inicializa la estructura de datos.
     */
    void initialize(key_t key, size_t segment_size);

    /*
     * Crea el segmento de memoria compartida e inicializa los datos
     * administrativos en la base de la estructura.
     */
    int create_segment(key_t key, size_t segment_size);

    /*
     * Se adosa al segmento de memoria compartida. Se asume que el atributo
     * shmid ya ha sido inicializado. Devuelve el puntero al segmento.
     */
    uint8_t *attach();

    /*
     * Acola un nuevo elemento, con datos de tamaño size, con prioridad p,
     * cuando la cola está vacía.
     */
    int enqueue_on_empty(void *data, uint32_t size, uint16_t p);

    /*
     * Acola un nuevo elemento, con datos de tamaño size, con prioridad p,
     * cuando la cola NO está vacía.
     */
    int enqueue_on_non_empty(void *data, uint32_t size, uint16_t p);

    /*
     * Crea un nuevo nodo a partir de la dirección indicada. Se asume
     * que la creación de la cabecera y la escritura de los datos a partir
     * de prev->next no estará pisando datos críticos.
     */
     void enqueue_after(node_header_t *prev, void *data, uint32_t size, uint16_t p);

     /*
      * Saca el siguiente elemento de la cola y actualiza los datos
      * administrativos de la estructura; devuelve un puntero a un bloque
      * en el heap local, donde los datos del nodo habrán sido copiados.
      */
     void *pop_next(queue_header_t *queue_header);

     void *pop_node(queue_header_t *queue_header, node_header_t *node);

     /*
      * Devuelve la prioridad del nodo más prioritario en la cola.
      */
     uint16_t find_max_priority(queue_header_t *queue_header);

  public:

    /*
     * Error: cuando se intenta realizar una inserción y el espacio total
     * disponible en el segmento es efectivamente suficiente para almacenar
     * los datos, pero por motivos de fragmentación no es posible
     * realizar la inserción.
     */
    const static int SMQ_ERROR_FRAGMENTATION_ISSUES;

    /* Error: cuando se intenta acolar un nuevo elemento pero el espacio
     * necesario no será suficiente para almacenarlo debido a que efectivamente
     * no queda tal cantidad de espacio en el segmento.
     */
    const static int SMQ_ERROR_NOT_ENOUGH_SPACE;

    /* Error: cuando se intenta extraer un elemento de la cola, a pesar de
     * que ésta está vacía.
     */
    const static int SMQ_ERROR_EMPTY_QUEUE;

    /*
     * Error: no se pudo obtener un segmento de memoria compartida adecuado.
     */
    const static int SMQ_ERROR_COULD_NOT_GET_SHM;

    /*
     * Tamaño por defecto para el segmento de memoria compartida que contendrá
     * datos de usuario y datos administrativos de la cola compartida.
     */
    const static uint32_t DEFAULT_SEGMENT_SIZE;

    /*
     * Constructores.
     *
     * El constructor más específico crea un segmento de memoria compartida
     * del tamaño dado. El primer constructor utiliza el tamaño por defecto.
     */
    SharedQueue(key_t key);
    SharedQueue(key_t key, size_t segment_size);

    /*
     * Intenta agregar un nuevo elemento a la cola, almacenando en memoria
     * compartida una cantidad data_size de bytes desde la dirección data.
     *
     * De no poder agregar el nodo por falta de espacio en el segmento de
     * memoria compartida, la función devuelve -1.
     *
     */
    int enqueue(void *data, uint32_t data_size, uint16_t priority);

    /*
     * Toma el siguiente elemento de la cola y lo desacola.
     *
     * Los datos del elemento desacolado son copiados a un buffer en
     * el heap del proceso que ejecuta la función; el puntero devuelto
     * apunta a este buffer.
     *
     * De no haber elementos en la cola, la función devuelve NULL.
     *
     */
    void *next();

    /*
     * Destructor. Libera todos los recursos asociados a la estructura,
     * solo si no hay otros procesos utilizándolos.
     */
    ~SharedQueue();

  };

}

#endif
