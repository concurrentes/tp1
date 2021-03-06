#ifndef BOAT_H_
#define BOAT_H_

#include "Process.h"
#include "ProcessFactory.h"
#include "Person.h"

#include <vector>

class Boat : public Process {

public:

  Boat(unsigned int id, unsigned int capacity);

  unsigned int get_free_seats();
  unsigned int get_passenger_count();

  unsigned int papers_in_order;

  void discharge_passengers_going_to(unsigned int city_id);
  void discharge_passengers(const int city_id=-1);
  void discharge_passengers_without_ticket();
  void receive_passenger(person_t *passenger);
  bool should_quit();
  std::vector<person_t *>::iterator discharge_passenger(
    std::vector<person_t*>::iterator &current_it, const int city_id=-1);

  ~Boat();

private:

  unsigned int id;
  unsigned int capacity;

  std::vector<person_t *> passengers;

  int run();

};

class BoatFactory : public ProcessFactory {

  unsigned int boat_capacity;
  unsigned int boat_count;

public:

  BoatFactory(unsigned int boat_capacity);

  Process *instantiate();

};

#endif
