#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "Logger.h"
#include "Configuration.h"
#include "Engine.h"
#include "CLIReader.h"

int main(int argc, const char** argv) {
  LOG(LOG_INFO, "Cargando configuración.");
  CLIReader cli = CLIReader(argc, argv);
  Configuration configuration = Configuration::get_instance();
  configuration.set_number_of_boats(cli.get_number_of_boats());
  configuration.set_capacity_of_boats(cli.get_boat_capacity());

  LOG(LOG_INFO, "Iniciando simulación.");
  Engine engine(configuration);
  engine.start();

  LOG(LOG_INFO, "Esperando finalización de la simulación.");
  wait(0);

  LOG(LOG_INFO, "Simulación finalizada.");
  return 0;
}
