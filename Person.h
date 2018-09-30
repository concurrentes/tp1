#ifndef PERSON_H_
#define PERSON_H_

#include "Configuration.h"

#include <stdint.h>

#define PERSON_DESCRIPTION_WORKER "Trabajador";
#define PERSON_DESCRIPTION_TOURIST "Turista";
#define PERSON_DESCRIPTION_INSPECTOR "Inspector";
#define PERSON_DESCRIPTION_PREFECT "Prefecto";

const uint32_t PERSON_TYPE_WORKER     = 0x0;
const uint32_t PERSON_TYPE_TOURIST    = 0x1;
const uint32_t PERSON_TYPE_PREFECT    = 0x2;
const uint32_t PERSON_TYPE_INSPECTOR  = 0x3;

typedef struct {
  uint32_t id;
  uint32_t type;
  uint32_t destination;
  uint32_t has_ticket;
} __attribute__((packed)) person_t;

void generate_random_person(person_t &person);

bool is_regular_passenger(person_t &person);

const char *get_description(person_t &person);

#endif
