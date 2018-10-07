#include "Configuration.h"
#include <map>
#include <iostream>
#include <sstream>
#include <fstream>

const Configuration &Configuration::get_instance() {
  static Configuration configuration;
  return configuration;
}

Configuration::Configuration() {
  std::map<std::string, std::string> config = this->_get_config_from_file();
  this->boat_count = this->get_or_default(config, "BOAT_COUNT", 2);
  this->boat_capacity = this->get_or_default(config, "BOAT_CAPACITY", 5);
  this->city_count = this->get_or_default(config, "CITY_COUNT", 2);
  this->city_step_mean_time = this->get_or_default(config, "CITY_STEP_MEAN_TIME", 25);
  this->person_generation_mean_time = this->get_or_default(config, "PERSON_GENERATION_MEAN_TIME", 15);
  this->boat_generation_interval = this->get_or_default(config, "BOAT_GENERATION_INTERVAL", 25);
  this->mean_gate_time = this->get_or_default(config, "MEAN_GATE_TIME", 1);
  this->p_tourist = this->get_or_default(config, "P_TOURIST", 25);
  this->p_ticket_inspector = this->get_or_default(config, "P_TICKET_INSPECTOR", 10);
  this->p_naval_prefect = this->get_or_default(config, "P_NAVAL_PREFECT", 1);
  this->p_tourist_leaving_ship = this->get_or_default(config, "P_TOURIST_LEAVING_SHIP", 50);
  this->mean_walker_time = this->get_or_default(config, "MEAN_WALKER_TIME", 20);
  this->p_tourist_going_for_a_walk = get_or_default(config, "P_TOURIST_WALKING", 50);
}

int Configuration::get_or_default(
        std::map<std::string, std::string>& config,
        const std::string& key,
        int default_value) {
  try {
    return std::stoi(config[key]);
  } catch (const std::exception &ex){
    return default_value;
  }
}

std::map<std::string, std::string> Configuration::_get_config_from_file() {
  std::string line;
  std::ifstream config_file("config.ini");
  std::map <std::string, std::string> config;
  if (config_file.is_open()) {
    while (getline(config_file, line)) {
      std::stringstream ss(line);
      std::string parameter, value;
      std::getline(ss, parameter, '=');
      std::getline(ss, value, '=');
      config[parameter] = value;
    }
    config_file.close();
  }
  return config;
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

unsigned int Configuration::get_probability_of_tourist_leaving_ship() const {
  return this->p_tourist_leaving_ship;
}

unsigned int Configuration::get_mean_walker_time() const {
  return this->mean_walker_time;
}

unsigned int Configuration::get_probability_of_tourist_going_walking() const {
  return this->p_tourist_going_for_a_walk;
}
