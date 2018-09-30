#include "Configuration.h"

const Configuration &Configuration::get_instance() {
  static Configuration configuration;
  return configuration;
}

Configuration::Configuration() {
  // TODO: Inicializar esto cargando algún archivo o algo.
  this->boat_count = 2;
  this->boat_capacity = 5;
  this->city_count = 2;
  this->city_step_mean_time = 25;
  this->person_generation_mean_time = 15;
  this->boat_generation_interval = 25;
  this->mean_gate_time = 1;
  this->p_tourist = 25;
  this->p_ticket_inspector = 10;
  this->p_naval_prefect = 1;
}

unsigned int Configuration::get_boat_count() const {
  return this->boat_count;
}

unsigned int Configuration::get_boat_capacity() const {
  return this->boat_capacity;
}

unsigned int Configuration::get_city_count() const {
  return this->city_count;
}

unsigned int Configuration::get_city_step_mean_time() const {
  return this->city_step_mean_time;
}

unsigned int Configuration::get_person_generation_mean_time() const {
  return this->person_generation_mean_time;
}

unsigned int Configuration::get_boat_generation_interval() const {
  return this->boat_generation_interval;
}

unsigned int Configuration::get_mean_gate_time() const {
  return this->mean_gate_time;
}

unsigned int Configuration::get_probability_of_tourist() const {
  return this->p_tourist;
}

unsigned int Configuration::get_probability_of_ticket_inspector() const {
  return this->p_ticket_inspector;
}

unsigned int Configuration::get_probability_of_naval_prefect() const {
  return this->p_naval_prefect;
}

unsigned int Configuration::get_zombie_cleanup_interval() const {
  return this->zombie_cleanup_interval;
}
