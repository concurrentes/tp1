#include "Logger.h"
#include "Process.h"
#include "City.h"

unsigned int CityFactory::city_count = 0;

Process *CityFactory::instantiate() {
  return new City(city_count++);
}

City::City(unsigned int id) {
  this->id = id;
}

int City::run() {
  dock_queue = new BlockingSharedQueue(this->id);

  LOG(LOG_INFO, "Iniciando ciudad " << this->id << ".");

  delete dock_queue;
  return 0;
}
