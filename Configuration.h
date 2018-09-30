#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

class Configuration {

private:

  unsigned int boat_count;

  unsigned int boat_capacity;

  unsigned int city_count;

  unsigned int city_step_mean_time;

  unsigned int person_generation_mean_time;

  unsigned int boat_generation_interval;

  unsigned int mean_gate_time;

  unsigned int p_tourist;

  unsigned int p_ticket_inspector;

  unsigned int p_naval_prefect;

  unsigned int zombie_cleanup_interval;

  Configuration();

public:

  static const Configuration &get_instance();

  // Cantidad de botes en circulación.
  unsigned int get_boat_count() const;

  // Capacidad de cada bote.
  unsigned int get_boat_capacity() const;

  // Cantidad de ciudades.
  unsigned int get_city_count() const;

  // Tiempo promedio que tarda un barco para ir de una ciudad a otra.
  unsigned int get_city_step_mean_time() const;

  // Tiempo promedio entre generación de personas en las ciudades.
  unsigned int get_person_generation_mean_time() const;

  // Tiempo promedio entre salidas iniciales de botes.
  unsigned int get_boat_generation_interval() const;

  // Tiempo promedio entre subida/bajada de pasajero.
  unsigned int get_mean_gate_time() const;

  // Probabilidad (de 0 a 100) de que una persona sea turista.
  unsigned int get_probability_of_tourist() const;

  // Probabilidad (de 0 a 100) de que una persona sea inspector.
  unsigned int get_probability_of_ticket_inspector() const;

  // Probabilidad (de 0 a 100) de que una persona sea prefecto.
  unsigned int get_probability_of_naval_prefect() const;

  // Tiempo entre limpieza de zombies para los generadores.
  unsigned int get_zombie_cleanup_interval() const;

};

#endif
