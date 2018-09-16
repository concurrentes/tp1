#ifndef BOAT_H_
#define BOAT_H_

#include "Process.h"
#include "ProcessFactory.h"

class Boat : public Process {

public:

  Boat(unsigned int capacity);

private:

  unsigned int capacity;

  int run();

};

class BoatFactory : public ProcessFactory {

  unsigned int boat_capacity;

public:

  BoatFactory(unsigned int boat_capacity);

  Process *instantiate() const;

};

#endif
