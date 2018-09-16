#ifndef ENGINE_H_
#define ENGINE_H_

#include "Configuration.h"
#include "Process.h"
#include "BoatGenerator.h"

class Engine : public Process {

private:

  BoatGeneratorFactory boat_generator_factory;

  int run();

public:

  Engine(const Configuration &config);

};

#endif
