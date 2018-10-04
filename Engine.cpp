#include "Logger.h"
#include "Configuration.h"
#include "Engine.h"
#include "Control.h"

#include <stdio.h>
#include <string.h>
#include <strings.h>

Engine::Engine(const Configuration &config)
  :
  city_generator_factory(
    config.get_city_count()
  ),
  boat_generator_factory(
    config.get_boat_count(),
    config.get_boat_capacity()
  ) {
}

int Engine::run() {
  /* TODO por algun motivo en esta clase si no lo deleto a mano
   * queda la memoria compartida volando */
  Control* pcontrol = new Control();
  pcontrol->initialize();
  delete pcontrol;
  LOG(LOG_INFO, "Iniciando generador de ciudades.");
  spawn_child(city_generator_factory);

  LOG(LOG_INFO, "Iniciando generador de botes.");
  spawn_child(boat_generator_factory);

  LOG(LOG_INFO, "Simulación en curso.");
  receive_commands();

  LOG(LOG_INFO, "Finalizando simulación.")
  return 0;
}

void Engine::receive_commands() {
  const uint32_t MAX_COMMAND_LENGTH = 32;
  char command_buffer[MAX_COMMAND_LENGTH];
  Control* pcontrol = new Control();
  while (!should_quit_gracefully()) {
    bzero(command_buffer, MAX_COMMAND_LENGTH);
    read(STDIN_FILENO, &command_buffer, MAX_COMMAND_LENGTH);

    if (strncmp(command_buffer, "quit", 4) == 0) {
      LOG(LOG_INFO, "Received quit command.");
      delete pcontrol;
      pcontrol = NULL;
      shutdown();
    } else if (strncmp(command_buffer, "count", 5) == 0) {
      LOG(LOG_INFO, "Received count command.");
      if (pcontrol) {
        LOG(LOG_INFO, pcontrol->get_discharged_count());
        LOG(LOG_INFO, pcontrol->get_captured_ships_count());
      }
    }
  }
}
