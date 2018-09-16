#ifndef BOAT_GENERATOR_H_
#define BOAT_GENERATOR_H_

#include "ProcessFactory.h"
#include "Generator.h"
#include "Boat.h"

class BoatGenerator : public Generator {

private:

  static const unsigned int GENERATION_INTERVAL; // segundos

  BoatFactory factory;

public:

  BoatGenerator(unsigned int boat_count, unsigned int boat_capacity);

};

class BoatGeneratorFactory : public ProcessFactory {

  unsigned int boat_count;
  unsigned int boat_capacity;

public:

  BoatGeneratorFactory(unsigned int boat_count, unsigned int boat_capacity);

  Process *instantiate() const;

};

#endif
