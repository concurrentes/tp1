#include "Logger.h"
#include "Configuration.h"
#include "Engine.h"

Engine::Engine(const Configuration &config)
  :
  boat_generator_factory(
    config.get_boat_count(),
    config.get_boat_capacity()) {
}

int Engine::run() {

  LOG(LOG_INFO, "Iniciando generador de botes.");
  spawn_child(boat_generator_factory);

  LOG(LOG_INFO, "Simulación en curso.");
  wait_for_children();

  LOG(LOG_INFO, "Finalizando simulación.")
  return 0;
}
