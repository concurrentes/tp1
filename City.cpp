#include "Logger.h"
#include "Process.h"
#include "City.h"
#include "Lock.h"
#include "Person.h"
#include "Boat.h"
#include "Control.h"
#include "Configuration.h"
#include "Random.h"

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

    unsigned int mu = config.get_person_generation_mean_time();
    unsigned int dt = mu/2;
    sleep(Random::random_wait(mu, dt));

    push_new_person();
  }

  delete dock_queue;
  return 0;
}

void City::push_new_person() {
  person_t person;
  generate_random_person(person);

  // Le asignamos prioridad a la persona según su tipo.
  uint16_t priority = is_regular_passenger(person) ? 0 : 1;

  // Ponemos a la persona a esperar en el muelle.
  dock_queue->enqueue(&person, sizeof(person_t), priority);

  // Obtenemos una etiqueta para describir a la persona.
  const char *type = get_description(person);

  LOG(LOG_INFO, type << " " << person.id << " espera en puerto " << this->id);
}

void City::receive_boat(Boat &boat) {

  LOG(LOG_DEBUG, "Bote intentando tomar lock con PID " << boat.get_pid() << " muelle " << this->id);
  Lock(this->lock_path);
  if (this->should_quit_gracefully() || boat.should_quit()) {
    LOG(LOG_DEBUG, "SHOULD QUIT GRACEFULLY RECEIVE_BOAT " << getpid());
    return;
  }
  LOG(LOG_DEBUG, "Bote tomando lock para ciudad con PID " << boat.get_pid() << " muelle " << this->id);
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

  try {
     Configuration config = Configuration::get_instance();

     if (Random::bernoulli(config.get_probability_of_naval_prefect())) {
         this->inspect_boat(boat);
     }
     if (Random::bernoulli(config.get_probability_of_ticket_inspector())) {
         boat.discharge_passengers_without_ticket();
     }

     LOG(LOG_INFO, "Bote " << boat.get_pid() << " sale de ciudad " << this->id);
  } catch (const std::string& message) {
      LOG(LOG_INFO, message);
  }

}

void City::inspect_boat(Boat &boat){
  LOG(LOG_INFO, "Prefectura inspecciona bote " << boat.get_pid() << " en ciudad " << this->id);
  if (boat.papers_in_order == 0) {
    std::string message = "Decomisando Bote " + std::to_string(boat.get_pid());
    LOG(LOG_INFO, "Bote " << boat.get_pid() << " esta flojo de papeles");
    LOG(LOG_INFO, "Bajando pasajeros de Bote " << boat.get_pid());
    boat.discharge_passengers();
    boat.shutdown();
    Control().add_captured_ship();
    throw "Decomisando bote " + std::to_string(boat.get_pid());
  } else {
    LOG(LOG_INFO, "Bote " << boat.get_pid() << " papeles en orden");
  }
}

void City::load_passengers_into(Boat &boat, unsigned int free_seats) {
  Configuration config = Configuration::get_instance();

  // Creamos una lista para contener a la gente esperando.
  std::list<void *> people;

  LOG(LOG_DEBUG, "Tomando lock de muelle con PID " << getpid() << " muelle " << this->id);
  int taken = dock_queue->take(free_seats, people);
  if (this->should_quit_gracefully() || boat.should_quit()) {
    LOG(LOG_DEBUG, "SHOULD QUIT GRACEFULLY LOAD_PASSENGERS_INTO " << getpid());
    std::list<void *>::iterator it;
    for (it = people.begin(); it != people.end(); ++it) {
      free(*it);
    }
    people.clear();
    return;
  }
  if (taken == 0) {
    LOG(LOG_INFO, "No hay gente en el muelle " << this->id);
    return;
  }
  LOG(LOG_DEBUG, "Lock de muelle tomado con PID " << getpid() << " muelle " << this->id);
  /*
   * Ahora contamos con una lista de personas que quieren subir al bote.
   * La idea es iterar por la lista y por cada persona realizar una acción.
   *
   * - Si la persona es un turista o un trabajador, agregarlos al bote.
   *
   */
  std::list<void *>::iterator it;

  for (it = people.begin(); it != people.end(); ++it) {
    person_t *current = (person_t *)(*it);

    const char *t = get_description(*current);

    LOG(LOG_INFO, t << " " << current->id << " está por subir al bote en " << this->id);

    boat.receive_passenger(current);
    sleep(config.get_mean_gate_time());
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
  LOG(LOG_DEBUG, "City shutting down with PID " << getpid());
  close(this->fd);
}
