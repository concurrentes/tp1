//
// Created by Bruno Liberini on 3/10/18.
//

#include "CLIReader.h"
#include "Logger.h"

void CLIReader::write_help() {
    std::cout << "PRIMER PROYECTO - Lake Concu" << std::endl << std::endl;
    std::cout << "DESCRIPCIÓN" << std::endl << std::endl;
    std::cout << "\t\t\t" << "Proyecto que simula el tráfico marítimo en un lago con diferentes ciudades en su costa";
    std::cout << " y el tráfico de pasajeros en ella" << std::endl << std::endl;
    std::cout << "OPCIONES" << std::endl;
    std::cout << "\t\t\t" << "-h, --help" << std::endl;
    std::cout << "\t\t\t   " << "Ayuda sobre el comando que ejecuta la simulación";
    std::cout << "\t\t\t" << "-b" << std::endl;
    std::cout << "\t\t\t   " << "Cantidad de barcos a simular navegando el lago. Debe ser entero mayor o igual a cero.";
    std::cout << " Por default, 2";
    std::cout << "\t\t\t" << "-c" << std::endl;
    std::cout << "\t\t\t   " << "Capacidad de pasajeros de cada barcos. Debe ser entero mayor o igual a cero.";
    std::cout << " Por default, 25";
    std::cout << std::endl;
    exit(NULL);
}

void CLIReader::parse_cli(const int &argc, const char **argv){

    if (argc == 2 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "-help"))) {
        this->write_help();
        exit(NULL);
    }

    long boatNumber = -1;
    long boatCapacity = -1;
    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-b") && i + 1 < argc) {
            if (strtol(argv[i+1], NULL, 10)) {
                boatNumber = strcmp(argv[i+1], "0") == 0 ? -1 : strtol(argv[i+1], NULL, 10);
                if (boatNumber < 0) {
                    std::cout << "Número de botes inválido. Debe ser un entero >= 0" << std::endl;
                    exit(NULL);
                }
            } else {
                std::cout << "Número de botes inválido. Debe ser un entero >= 0" << std::endl;
                exit(NULL);
            }
        }
        if (!strcmp(argv[i], "-c") && i + 1 < argc) {
            if (strtol(argv[i+1], NULL, 10)) {
                boatCapacity = strcmp(argv[i+1], "0") == 0 ? -1 : strtol(argv[i+1], NULL, 10);
                if (boatCapacity < 0) {
                    std::cout << "Capacidad de botes inválida. Debe ser un entero >= 0" << std::endl;
                    exit(NULL);
                }
            } else {
                std::cout << "Capacidad de botes inválida. Debe ser un entero >= 0" << std::endl;
                exit(NULL);
            }
        }
    }

    this->boat_capacity = boatCapacity < 0 ? this->boat_capacity : boatCapacity;
    this->number_of_boats = boatNumber < 0 ? this->number_of_boats : boatNumber;

    LOG(LOG_INFO, "Se genera simulación con " << this->number_of_boats << " botes con capacidad para " << this->boat_capacity << " pasajeros");
}

CLIReader::CLIReader(const int &argc, const char **argv) {
    this->boat_capacity = 25;
    this->number_of_boats = 2;
    parse_cli(argc, argv);
}

CLIReader::~CLIReader() {}

int CLIReader::get_number_of_boats() {
    return this->number_of_boats;
}

int CLIReader::get_boat_capacity() {
    return this->boat_capacity;
}