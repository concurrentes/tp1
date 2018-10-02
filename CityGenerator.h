#ifndef CITY_GENERATOR_H_
#define CITY_GENERATOR_H_

#include "ProcessFactory.h"
#include "Generator.h"
#include "City.h"

class CityGenerator : public Generator {

private:

  CityFactory factory;

public:

  /*
   * Instancia un generador que generará instantaneamente city_count ciudades.
   * Adicionalmente, los recursos necesarios para que las ciudades funcionen
   * serán mantenidos vivos por el generador.
   */
  CityGenerator(unsigned int city_count);

};

class CityGeneratorFactory : public ProcessFactory {

private:

  unsigned int city_count;

public:

  CityGeneratorFactory(unsigned int city_count);

  Process *instantiate();

};

#endif
