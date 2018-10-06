#include "Configuration.h"
#include "Person.h"

#include <stdlib.h>
#include <strings.h>
#include "MemoriaCompartida.h"
#include "Control.h"

void generate_random_person(person_t &person) {
  Configuration config = Configuration::get_instance();

  // Iniicalizamos la estructura en 0.
  bzero(&person, sizeof(person_t));

  // Generamos aleatoriamente el tipo de persona.
  unsigned int chosen_type = 0;
  person.id = Control().get_next_id();
  unsigned int p_vec[2] = {
    config.get_probability_of_tourist()
  };

  unsigned int r = rand() % 100;

  for (unsigned int i = 0, p = 0; i < 1; ++i) {
    p += p_vec[i];

    if (r < p) {
      chosen_type = i+1;
      break;
    }
  }

  switch(chosen_type) {
    case 0:
      person.type = PERSON_TYPE_WORKER;
      person.destination = rand() % config.get_city_count();
      person.has_ticket = rand() % 2;
      break;
    case 1:
      person.type = PERSON_TYPE_TOURIST;
      person.has_ticket = rand() % 2;
      break;
  }

}


bool is_regular_passenger(person_t &person) {
  return person.type == PERSON_TYPE_WORKER || person.type == PERSON_TYPE_TOURIST;
}

const char *get_description(person_t &person) {
  const char *type;

  switch(person.type) {
    case PERSON_TYPE_WORKER:
      type = PERSON_DESCRIPTION_WORKER;
      break;
    case PERSON_TYPE_TOURIST:
      type = PERSON_DESCRIPTION_TOURIST;
      break;
  }

  return type;
}
