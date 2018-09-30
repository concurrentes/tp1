#include "Logger.h"
#include "Process.h"
#include "City.h"

unsigned int CityFactory::city_count = 0;

Process *CityFactory::instantiate() {
  return new City(city_count++);
}

City::City(unsigned int id)
  :
  dock_queue(id) {
  this->id = id;
}

int City::run() {
  LOG(LOG_INFO, "Iniciando ciudad " << this->id << ".");
  return 0;
}
