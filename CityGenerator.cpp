#include "Logger.h"
#include "CityGenerator.h"

CityGenerator::CityGenerator(unsigned int city_count)
  :
  Generator(0, city_count) {

  register_factory(factory, 1);
}

CityGeneratorFactory::CityGeneratorFactory(unsigned int city_count) {
  this->city_count = city_count;
}

Process *CityGeneratorFactory::instantiate() {
  return new CityGenerator(this->city_count);
}
