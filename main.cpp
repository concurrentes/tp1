#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>

#include "Logger.h"
#include "Configuration.h"
#include "Engine.h"

int main(int argc, char** argv) {
  if (argc > 1 && strcmp(argv[1], "debug") == 0) {
    std::cout << "Entrando a modo debug. No verá ningún log por pantalla, salvo la configuración inicial ";
    std::cout << "y los que resulten de los comandos 'count' y 'quit'." << std::endl;
    Logger::set_debug();
    std::cout << "Procesando..." << std::endl;
  }

  LOG(LOG_INFO, "Cargando configuración.");
  Configuration configuration = Configuration::get_instance();

  LOG(LOG_INFO, "Iniciando simulación.");
  Process *engine = new Engine(configuration);
  engine->start(NULL);

  LOG(LOG_INFO, "Esperando finalización de la simulación.");
  wait(0);

  delete engine;
  LOG(LOG_INFO, "Simulación finalizada.");
  return 0;
}
