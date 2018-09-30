#include "Logger.h"
#include "Process.h"
#include "City.h"
#include "Lock.h"
#include "Person.h"
#include "Boat.h"

#include <sstream>
#include <string>
#include <list>
#include <stdlib.h>

unsigned int CityFactory::city_count = 0;

const std::string City::BASE_PATH = "/tmp/lakeconcu.dock.";

Process *CityFactory::instantiate() {
  return new City(city_count++);
}

City::City(unsigned int id) {
  this->id = id;

  std::stringstream path_builder;

  path_builder << BASE_PATH << id;

  this->lock_path = path_builder.str();
  this->fd = open(this->lock_path.c_str(), O_CREAT, 0644);
}

int City::run() {
  dock_queue = new BlockingSharedQueue(this->id);

  LOG(LOG_INFO, "Iniciando ciudad " << this->id << ".");

  Configuration config = Configuration::get_instance();

  while (!should_quit_gracefully()) {
    sleep(config.get_person_generation_mean_time());
    push_new_person();
  }

  delete dock_queue;
  return 0;
}

void City::push_new_person() {
  person_t person;

  // TODO: generar algún identificador; poner algún contador compartido en Person.h
  generate_random_person(person);

  uint16_t priority;

  if (is_regular_passenger(person)) {
    priority = 0;
  } else {
    priority = 1;
  }

  dock_queue->enqueue(&person, sizeof(person_t), priority);

  // Obtenemos una etiqueta para describir a la persona.
  const char *type = get_description(person);

  LOG(LOG_INFO, type << " " << person.id << " espera en puerto " << this->id);
}

void City::receive_boat(Boat &boat) {
  Lock(this->fd);

  LOG(LOG_INFO, "Bote " << boat.get_pid() << " llega a ciudad " << this->id);

  // Bajamos a los pasajeros que vienen a esta ciudad.
  boat.discharge_passengers_going_to(this->id);

  // Determinamos la cantidad de espacio en el bote.
  unsigned int free_seats = boat.get_free_seats();

  LOG(LOG_INFO, "Quedan " << free_seats << " asientos en bote " << boat.get_pid());

  if (free_seats > 0) {
    dock_queue = new BlockingSharedQueue(this->id);
    load_passengers_into(boat, free_seats);
    delete dock_queue;
  }

  LOG(LOG_INFO, "Bote " << boat.get_pid() << " sale de ciudad " << this->id << ".");
}

void City::load_passengers_into(Boat &boat, unsigned int free_seats) {
  Configuration config = Configuration::get_instance();

  // Creamos una lista para contener a la gente esperando.
  std::list<void *> people;

  if (dock_queue->take(free_seats, people) == 0) {
    LOG(LOG_INFO, "No hay gente en el muelle " << this->id);
    return;
  }

  /*
   * Ahora contamos con una lista de personas que quieren subir al bote.
   * La idea es iterar por la lista y por cada persona realizar una acción.
   *
   * - Si la persona es un turista o un trabajador, agregarlos al bote.
   *
   * - Si la persona es un inspector de tickets, bajar a todos los pasajeros
   *   que estén en el bote y que no tengan ticket.
   *
   * - Si es prefectura naval, generar un número aleatorio. Si esta dentro
   *   de cierto rango, bajar a todas las personas y terminar el bote.
   */
  std::list<void *>::iterator it;

  for (it = people.begin(); it != people.end(); ++it) {
    person_t *current = (person_t *)(*it);

    const char *t = get_description(*current);

    LOG(LOG_INFO, t << " " << current->id << " está por subir al bote en " << this->id);

    if (is_regular_passenger(*current)) {
      boat.receive_passenger(current);
      sleep(config.get_mean_gate_time());
    } else {
      const char *type = get_description(*current);
      // TODO: verificar tipo y hacer cosas.
      delete current;
    }

  }

  free_seats = boat.get_free_seats();
  LOG(LOG_INFO, "Quedan " << free_seats << " asientos en bote " << boat.get_pid());

  unsigned int count = dock_queue->count();

  if (count == 0) {
    LOG(LOG_INFO, "No queda más gente en el muelle " << this->id);
  } else {
    LOG(LOG_INFO, "Quedan " << count << " personas en muelle " << this->id << ", pero el bote debe partir");
  }
}

City::~City() {
  close(this->fd);
}
