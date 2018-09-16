#ifndef GENERATOR_H_
#define GENERATOR_H_

#include "ProcessFactory.h"
#include "Process.h"

#include <vector>

class Generator : public Process {

private:
  unsigned int interval;
  unsigned int max;

  unsigned int w_total;

  typedef struct {
    const ProcessFactory *factory;
    unsigned int weight;
  } t_process_factory;

  std::vector<t_process_factory*> factories;

  /*
   * generate_random_process
   *
   * Genera un proceso seleccionando un objecto factory
   * al azar, tal como se documenta en los otros métodos.
   *
   */
  void generate_random_process();

  /*
   * run
   *
   * Comienza a generar hasta "max" procesos cada "interval" segundos.
   * Los procesos generados son instanciados en base a los objetos
   * tipo factory registrados mediante register_factory.
   * 
   */
  virtual int run();

public:

  /*
   * Generator
   *
   * Instancia un generador que genera transacciones
   * en forma periódica, una nueva cada "interval" segundos,
   * siendo "max" la cantidad máxima de transacciones a generar.
   *
   */
  Generator(unsigned int interval, unsigned int max);

  /*
   * register_factory
   *
   * Registra un objeto de tipo ProcessFactory con el generador.
   *
   * En cada ciclo el generador selecciona un objeto factory de
   * entre todos los registrados. La selección es aleatoria;
   * la probabilidad de seleccionar un factory con peso w_i es 
   *
   * p_i = w_i/sum_j(w_j), j != i.
   *
   */
  void register_factory(const ProcessFactory &factory, unsigned int w);

  virtual ~Generator();

};

#endif
