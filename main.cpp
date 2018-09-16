#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "Logger.h"
#include "Configuration.h"
#include "Engine.h"

int main(int argc, char** argv) {
  LOG(LOG_INFO, "Cargando configuración.");
  Configuration configuration(argc, argv);

  LOG(LOG_INFO, "Iniciando simulación.");
  Engine engine(configuration);
  engine.start();

  LOG(LOG_INFO, "Esperando finalización de la simulación.");
  wait(0);

  LOG(LOG_INFO, "Simulación finalizada.");
  return 0;
}
