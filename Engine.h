#ifndef ENGINE_H_
#define ENGINE_H_

#include "Configuration.h"
#include "Process.h"
#include "BoatGenerator.h"
#include "CityGenerator.h"
#include "WalkerGenerator.h"
#include "Semaforo.h"
#include "Control.h"

class Engine : public Process {

private:

  CityGeneratorFactory city_generator_factory;
  BoatGeneratorFactory boat_generator_factory;
  WalkerGeneratorFactory walking_generator_factory;

  Semaforo semaforo;
  Control control;
  int run();

public:

  Engine(const Configuration &config);

  void receive_commands();

  ~Engine();

};

#endif
