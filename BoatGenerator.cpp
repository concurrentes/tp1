#include "Logger.h"
#include "BoatGenerator.h"
#include "Configuration.h"

const unsigned int BoatGenerator::GENERATION_INTERVAL =
  Configuration::get_instance().get_boat_generation_interval();

BoatGenerator::BoatGenerator(
  unsigned int boat_count,
  unsigned int boat_capacity)
  :
  Generator(GENERATION_INTERVAL, boat_count),
  factory(boat_capacity) {

  LOG(LOG_INFO, "Iniciando generador de " << boat_count << " botes.");
  LOG(LOG_INFO, "Intervalo de generación: " << GENERATION_INTERVAL << "s.");
  register_factory(factory, 1);
}

BoatGeneratorFactory::BoatGeneratorFactory(
  unsigned int boat_count,
  unsigned int boat_capacity) {

  this->boat_count = boat_count;
  this->boat_capacity = boat_capacity;
}

Process *BoatGeneratorFactory::instantiate() {
  return new BoatGenerator(this->boat_count, this->boat_capacity);
}
