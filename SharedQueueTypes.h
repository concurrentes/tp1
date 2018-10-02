#ifndef SHARED_QUEUE_TYPES_H_
#define SHARED_QUEUE_TYPES_H_

#include <stdint.h>

namespace smstruct {

  typedef struct {

    /*
     * Capacidad del segmento en bytes, y cantidad de bytes utilizados,
     * incluyendo aquellos utilizados para almacenar datos administrativos.
     */
    uint32_t capacity;
    uint32_t consumed;

    /*
     * Punteros relativos al inicio del segmento, que apuntan al último
     * elemento de la cola, y al primero, respectivamente.
     */
    int64_t ptr_tail;
    int64_t ptr_head;

    /*
     * Cantidad de elementos almacenados en la cola.
     */
    uint16_t count;

    /*
     * Puntero relativo al inicio del segmento, que indica el comienzo
     * de la sección de datos de la cola.
     */
    uint16_t data_offset;

  } __attribute__((packed)) queue_header_t;


  typedef struct {

    /*
     * Prioridad del nodo en la cola. Los nodos de prioridad más alta
     * serán desacolados primero.
     */
    uint16_t priority;

    /*
     * Flags, reservado.
     */
    uint16_t flags;

    /*
     * Tamaño de los datos almacenados en el nodo.
     */
    uint32_t data_size;

    /*
     * Punteros al nodo siguiente, y al anterior, respectivamente.
     * Solo serán no nulos si estos elementos existen, efectivamente.
     *
     * Son relativos a la posición del nodo actual.
     */
    uint64_t ptr_next;
    uint64_t ptr_prev;

  } __attribute__((packed)) node_header_t;

}

/*
 * Alias para la estructura de la cabecera del segmento.
 */
#define qh_t queue_header_t

/*
 * Alias para la estructura de la cabecera del nodo.
 */
#define nh_t node_header_t

/*
 * Tamaño de la estructura en la cabecera de la cola.
 */
#define QUEUE_HEADER_SIZE sizeof(queue_header_t)

/*
 * Determina el puntero al inicio de la sección de datos del segmento.
 */
#define DATA_SECTION(p) ((uint8_t *)p + ((queue_header_t *)p)->data_offset)

/*
 * Determina el puntero al inicio de la tabla P en la cabecera del segmento.
 */
#define P_TABLE(p) ((uint16_t *)((uint8_t *)p + QUEUE_HEADER_SIZE))

/*
 * Determina el puntero al último nodo de la cola.
 */
#define TAIL_NODE(p) ((node_header_t *)((uint8_t *)p + ((qh_t *)p)->ptr_tail))

/*
 * Determina el puntero al nodo frontal de la cola.
 */
#define HEAD_NODE(p) ((node_header_t *)((uint8_t *)p + ((qh_t *)p)->ptr_head))

/*
 * Determina la posición del eje superior del segmento, dado el puntero
 * a los datos administrativos de la cola.
 */
#define SEGMENT_TOP_AXIS(p) ((uint8_t *)p + ((queue_header_t *)p)->capacity)

/*
 * Determina el espacio total disponible en el segmento de memoria.
 */
#define SPACE_AVAILABLE(p) (((qh_t *)p)->capacity - ((qh_t *)p)->consumed)

/*
 * Tamaño de la estructura en la cabecera de cada nodo.
 */
#define HEADER_SIZE sizeof(node_header_t)

/*
 * Determina el puntero al siguiente elemento en la cola.
 */
#define NEXT(p) ((node_header_t *)((uint8_t *)p + ((nh_t *)p)->ptr_next))

/*
 * Determina el puntero al elemento anterior en la cola.
 */
#define PREV(p) ((node_header_t *)((uint8_t *)p + ((nh_t *)p)->ptr_prev))

/*
 * Determina el puntero al inicio de la sección de datos del nodo.
 */
#define NODE_DATA(p) ((uint8_t *)p + HEADER_SIZE)

/*
 * Determina la cantidad de espacio requerida para almacenar data_size bytes.
 */
#define SPACE_REQUIREMENTS(data_size) (HEADER_SIZE + data_size)

/*
 * Determina la distancia en bytes desde la dirección a hasta la dirección b.
 */
#define DELTA(a, b) ((int64_t)((uint8_t *)b - (uint8_t *)a))

/*
 * Determina la posición del eje superior del nodo.
 */
#define NODE_TOP_AXIS(p) ((uint8_t *)p + HEADER_SIZE + ((nh_t *)p)->data_size)

/*
 * Determina la distancia desde el eje superior hasta el tope del segmento.
 */
#define SPACE_AFTER(p, q) (size_t)(SEGMENT_TOP_AXIS(q) - NODE_TOP_AXIS(p))

/*
 * Determina la distancia desde el eje infeerior hasta el inicio de los datos
 * de la cola, el eje superior de los datos administrativos de la cola.
 */
#define SPACE_BEFORE(p, q) (size_t)((uint8_t *)p - DATA_SECTION(q))

/*
 * Determina la distancia desde el eje superior del nodo p hasta el inicio
 * del nodo v. Se asume v > p.
 */
#define SPACE_FROM_TO(p, v) (size_t)((uint8_t *)v - NODE_TOP_AXIS(p))

/*
 * Determina el offset desde el nodo p hasta el inicio del segmento de
 * datos de la cola q.
 */
#define REWIND(p, q) (int64_t)(DATA_SECTION(q) - (uint8_t *)p);

#endif
