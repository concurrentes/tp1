#include "Configuration.h"

Configuration::Configuration(int argc, char **argv) {
  this->boat_count = 5;
  this->boat_capacity = 20;
  this->city_count = 4;
}

unsigned int Configuration::get_boat_count() const {
  return this->boat_count;
}

unsigned int Configuration::get_boat_capacity() const {
  return this->boat_capacity;
}

unsigned int Configuration::get_city_count() const {
  return this->city_count;
}
