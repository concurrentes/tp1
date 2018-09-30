#include "Logger.h"
#include "Boat.h"

Boat::Boat(unsigned int capacity) {
  this->capacity = capacity;
}

int Boat::run() {
  LOG(LOG_INFO, "Bote " << get_pid() << " en curso.");
  return 0;
}

BoatFactory::BoatFactory(unsigned int boat_capacity) {
  this->boat_capacity = boat_capacity;
}

Process *BoatFactory::instantiate() {
  LOG(LOG_INFO, "Generando bote, capacidad " << this->boat_capacity << ".");
  return new Boat(this->boat_capacity);
}
