#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

class Configuration {

private:

  unsigned int boat_count;

  unsigned int boat_capacity;

  unsigned int city_count;

public:

  Configuration(int argc, char **argv);

  unsigned int get_boat_count() const;

  unsigned int get_boat_capacity() const;

  unsigned int get_city_count() const;

};

#endif
