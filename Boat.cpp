#include "Logger.h"
#include "Boat.h"
#include "City.h"
#include "Configuration.h"
#include "Control.h"
#include "Random.h"

#include <unistd.h>
#include <stdlib.h>

Boat::Boat(unsigned int id, unsigned int capacity) {
  this->id = id;
  this->capacity = capacity;
  this->papers_in_order = rand() % 2;
}

int Boat::run() {
  LOG(LOG_INFO, "Bote " << get_pid() << " en curso.");

  Configuration config = Configuration::get_instance();

  for (unsigned int city_id = 0; !should_quit_gracefully(); ++city_id) {
    sleep(config.get_city_step_mean_time());

    City current_city(city_id % config.get_city_count());
    current_city.set_parent(NULL);
    current_city.receive_boat(*this);
  }

  return 0;
}

void Boat::receive_passenger(person_t *passenger) {
  uint32_t id = passenger->id;
  uint32_t tp = passenger->type;

  const char *description = get_description(*passenger);
  const char *has_ticket = passenger->has_ticket? "si" : "no";

  LOG(LOG_INFO, description << " " << id << " sube al bote " << get_pid() << " (Boleto: " << has_ticket << ")");
  passengers.push_back(passenger);
}

void Boat::discharge_passengers() {
  Configuration config = Configuration::get_instance();

  std::vector<person_t *>::iterator it;
  for (it = passengers.begin(); it != passengers.end(); ) {
    person_t *current = *it;

    const char *description = get_description(*current);

    LOG(LOG_INFO, "Bote " << get_pid() << " descargando " << description << " " << current->id);
    it = discharge_passenger(it);
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
      it = discharge_passenger(it);
      continue;
    }

    if (current->type == PERSON_TYPE_TOURIST) {
      if (Random::bernoulli(config.get_probability_of_tourist_leaving_ship())) {
        LOG(LOG_INFO, "Al turista " << current->id << " se le antojó bajar en " << city_id);
        LOG(LOG_INFO, "Bote " << get_pid() << " descargando turista " << current->id);

        if (Random::bernoulli(config.get_probability_of_tourist_going_walking())) {
          LOG(LOG_INFO, "Turista " << current->id << " decidió ir a pasear");
          current->destination = (rand() % config.get_city_count());
          BlockingSharedQueue walking_queue(config.get_city_count());
          walking_queue.enqueue(current, sizeof(person_t), 0);
        }

        it = discharge_passenger(it);
      }

      continue;
    }

    // No le tocaba bajar; continuamos con el siguiente pasajero.
    it++;
  }

  LOG(LOG_INFO, "Bote " << get_pid() << " ha descargado pasajeros en " << city_id);
}

void Boat::discharge_passengers_without_ticket() {
  Configuration config = Configuration::get_instance();

  LOG(LOG_INFO, "Un inspector de pasajes ha subido al bote " << get_pid());

  unsigned int count = 0;

  std::vector<person_t *>::iterator it;
  for (it = passengers.begin(); it != passengers.end(); ) {
    person_t *current = *it;

    if (current->has_ticket == 0) {
      LOG(LOG_INFO, get_description(*current) << " " << current->id << " no tiene boleto, por lo que deberá bajarse");
      LOG(LOG_INFO, get_description(*current) << " " << current->id << " está bajando del bote " << get_pid() << " por no tener boleto");
      it = discharge_passenger(it);
      Control().add_discharged();
      count++;
    } else {
      it++;
    }
  }

  if (count > 0) {
    LOG(LOG_INFO, "El inspector ha bajado " << count << " pasajeros; satisfecho, baja del bote y se va caminando");
  } else {
    LOG(LOG_INFO, "El inspector observa que todos los pasajeros tienen boleto, por lo que baja del bote y se va caminando");
  }
}

std::vector<person_t *>::iterator Boat::discharge_passenger(
  std::vector<person_t*>::iterator &it) {

  person_t *passenger = *it;

  sleep(Configuration::get_instance().get_mean_gate_time());
  free(passenger);
  return passengers.erase(it);
}

unsigned int Boat::get_passenger_count() {
  return this->passengers.size();
}

unsigned int Boat::get_free_seats() {
  return this->capacity - this->get_passenger_count();
}

bool Boat::should_quit() {
  return this->should_quit_gracefully();
}

Boat::~Boat() {
  LOG(LOG_DEBUG, "Boat shutting down with PID " << getpid());
  std::vector<person_t *>::iterator it;
  for (it = passengers.begin(); it != passengers.end(); ++it) {
    free(*it);
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
