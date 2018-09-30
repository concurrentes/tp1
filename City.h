#ifndef CITY_H_
#define CITY_H_

#include "BlockingSharedQueue.h"
#include "Process.h"
#include "ProcessFactory.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>

using namespace smstruct;

/*
 * Declaramos la clase Boat por adelantado.
 */
class Boat;

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
   * Path al archivo para lockear los muelles de la ciudad cuando
   * arriban barcos.
   */
  std::string lock_path;

  /*
   * Descriptor de un archivo para lockear los muelles de la ciudad
   * cuando arriban barcos.
   */
  int fd;

  /*
  * Cola que contendrá los pasajeros en espera de un barco.
  */
  BlockingSharedQueue *dock_queue;

  /*
   * Crea una nueva persona y manda los datos a la cola.
   */
  void push_new_person();

  int run();

public:

  static const std::string BASE_PATH;

  City(unsigned int id);

  ~City();

  void receive_boat(Boat &boat);

  void load_passengers_into(Boat &boat, unsigned int free_seats);

};

#endif
