#include "Logger.h"
#include "Configuration.h"
#include "Engine.h"
#include "Control.h"
#include "Semaforo.h"

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <iostream>

Engine::Engine(const Configuration &config)
  :
  city_generator_factory(
    config.get_city_count()
  ),
  boat_generator_factory(
    config.get_boat_count(),
    config.get_boat_capacity()
  ),
  semaforo(SEM) {
  semaforo.inicializar(0);
  control.initialize();
}

int Engine::run() {

  LOG(LOG_INFO, "Iniciando generador de ciudades.");
  spawn_child(city_generator_factory);

  LOG(LOG_INFO, "Iniciando generador de botes.");
  spawn_child(boat_generator_factory);

  // Inicializamos el generador del paseador de pasajeros.
  spawn_child(walking_generator_factory);

  LOG(LOG_INFO, "Simulación en curso.");
  receive_commands();

  // El comando 'count' y el comando 'quit' deben verse por pantalla siempre
  std::cout << "Finalizando simulación." << std::endl;
  semaforo.eliminar();
  return 0;

}

void Engine::receive_commands() {
  const uint32_t MAX_COMMAND_LENGTH = 32;
  char command_buffer[MAX_COMMAND_LENGTH];
  while (!should_quit_gracefully()) {
    bzero(command_buffer, MAX_COMMAND_LENGTH);
    read(STDIN_FILENO, &command_buffer, MAX_COMMAND_LENGTH);

    if (strncmp(command_buffer, "quit", 4) == 0) {
      LOG(LOG_INFO, "Received quit command.");
      shutdown();
    } else if (strncmp(command_buffer, "count", 5) == 0) {
      LOG(LOG_INFO, "Received count command.");
      // El comando 'count' y el comando 'quit' deben verse por pantalla siempre
      std::cout << "Pasajeros sin ticket bajados: " << control.get_discharged_count() << std::endl;
      std::cout << "Naves decomisadas: " << control.get_captured_ships_count() << std::endl;
    }
  }
}

Engine::~Engine() {

}
