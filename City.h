#ifndef CITY_H_
#define CITY_H_

#include "BlockingSharedQueue.h"
#include "Process.h"
#include "ProcessFactory.h"

using namespace smstruct;

/*
 * Esta clase instancia objetos Factory, cada uno con su propio identificador.
 * Adicionalmente, mantiene una lista estática con los identificadores de
 * las ciudades instanciadas.
 */
class CityFactory : public ProcessFactory {

private:

  static unsigned int city_count;

public:

  Process *instantiate();

};

class City : public Process {

private:

  /*
   * Identificador de la ciudad, también utilizado para identificar
   * la cola del muelle de esta ciudad.
   */
  unsigned int id;

  /*
  * Cola que contendrá los pasajeros en espera de un barco.
  */
  BlockingSharedQueue *dock_queue;

  int run();

public:

  City(unsigned int id);

};

#endif
