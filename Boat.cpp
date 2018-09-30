#include "Logger.h"
#include "Boat.h"
#include "City.h"
#include "Configuration.h"

#include <unistd.h>

Boat::Boat(unsigned int id, unsigned int capacity) {
  this->id = id;
  this->capacity = capacity;
}

int Boat::run() {
  LOG(LOG_INFO, "Bote " << get_pid() << " en curso.");

  Configuration config = Configuration::get_instance();

  for (unsigned int city_id = 0; !should_quit_gracefully(); ++city_id) {
    sleep(config.get_city_step_mean_time());
    City current_city(city_id % config.get_city_count());
    current_city.receive_boat(*this);
  }

  return 0;
}

void Boat::receive_passenger(person_t *passenger) {
  uint32_t id = passenger->id;
  uint32_t tp = passenger->type;

  const char *description = get_description(*passenger);

  LOG(LOG_INFO, description << " " << id << " sube al bote " << get_pid());
  passengers.push_back(passenger);
}

void Boat::discharge_passengers() {
  Configuration config = Configuration::get_instance();

  std::vector<person_t *>::iterator it;
  for (it = passengers.begin(); it != passengers.end(); ) {
    it = passengers.erase(it);
    sleep(config.get_mean_gate_time());
  }
}

void Boat::discharge_passengers_going_to(unsigned int city_id) {
  Configuration config = Configuration::get_instance();

  LOG(LOG_INFO, "Bote " << get_pid() << " descargando pasajeros en " << city_id);

  std::vector<person_t *>::iterator it;
  for (it = passengers.begin(); it != passengers.end(); ) {
    person_t *current = *it;

    if (current->type == PERSON_TYPE_WORKER && current->destination == city_id) {
      LOG(LOG_INFO, "Bote " << get_pid() << " descargando trabajador " << current->id);
      it = passengers.erase(it);
      sleep(config.get_mean_gate_time());
      continue;
    }

    if (current->type == PERSON_TYPE_TOURIST) {
      LOG(LOG_INFO, "Bote " << get_pid() << " descargando turista " << current->id);
      // TODO generar random y ver si baja.
      // TODO manejar caso en el que se van a pasear y llegan a otra ciudad.
      it = passengers.erase(it);
      sleep(config.get_mean_gate_time());
      continue;
    }

    // No le tocaba bajar; continuamos con el siguiente pasajero.
    it++;
  }

  LOG(LOG_INFO, "Bote " << get_pid() << " ha descargado pasajeros en " << city_id);
}

void Boat::discharge_passengers_without_ticket() {
  Configuration config = Configuration::get_instance();

  std::vector<person_t *>::iterator it;
  for (it = passengers.begin(); it != passengers.end(); ) {
    person_t *current = *it;

    if (current->has_ticket == 0) {
      it = passengers.erase(it);
      sleep(config.get_mean_gate_time());
    } else {
      it++;
    }
  }
}

unsigned int Boat::get_passenger_count() {
  return this->passengers.size();
}

unsigned int Boat::get_free_seats() {
  return this->capacity - this->get_passenger_count();
}

Boat::~Boat() {
  std::vector<person_t *>::iterator it;
  for (it = passengers.begin(); it != passengers.end(); ++it) {
    delete *it;
  }
  passengers.clear();
}

BoatFactory::BoatFactory(unsigned int boat_capacity) {
  this->boat_capacity = boat_capacity;
  this->boat_count = 0;
}

Process *BoatFactory::instantiate() {
  LOG(LOG_INFO, "Generando bote, capacidad " << this->boat_capacity);
  return new Boat(this->boat_count++, this->boat_capacity);
}
