#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

class Configuration {

private:

  unsigned int boat_count;

  unsigned int boat_capacity;

public:

  Configuration(int argc, char **argv);

  unsigned int get_boat_count() const;

  unsigned int get_boat_capacity() const;

};

#endif
