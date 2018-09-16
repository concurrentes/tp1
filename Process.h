#ifndef PROCESS_H_
#define PROCESS_H_

#include "ProcessFactory.h"

#include <sys/types.h>
#include <unistd.h>
#include <vector>

class Process {

private:

  pid_t pid;

  typedef std::vector<Process*> p_vec;

  /*
   * p_vec children
   *
   * Sirve para mantener referencias a los hijos, para esperar
   * en caso de finalización.
   *
   */
  p_vec children;

  /*
   * run
   *
   * Las funcionalidades propias de cada proceso deben ser definidas por
   * las implementaciones concretas en el método run.
   *
   * Retorna el estado de finalización del proceso.
   *
   */
  virtual int run() = 0;

  /*
   * _finalize
   *
   * Realiza tareas de finalización y limpieza de memoria.
   * No llamar si no sabés lo que estás haciendo.
   */
  void _finalize();

  /*
   * _clear_children
   *
   * Limpia la lista de hijos del proceso.
   * No llamar si no sabés lo que estás haciendo.
   *
   */
  void _clear_children();

  /*
   * _shutdown_children
   *
   * Manda la señal de terminación a todos los hijos.
   *
   */
  void _shutdown_children();

protected:

  /*
   * clean_zombies
   *
   * Limpia de la memoria los procesos que hayan finalizado.
   *
   */
  void clean_zombies();

public:

  /*
   * start
   *
   * Inicia el proceso. Se puede utilizar, por ejemplo, de la siguiente forma:
   *
   * class MyProcess : public Process {
   * public:
   *   int run() {
   *     printf("Hello, world!\n");
   *     return 0;
   *   }
   * };
   *
   * ...
   *
   * // Instanciamos un proceso ejecutable.
   * MyProcess my_process;
   *
   * // Iniciamos el proceso usando start.
   * my_process.start();
   *
   */
  void start();

  /*
   * get_pid
   *
   * Si se trata del proceso padre, devuelve el PID del hijo.
   *
   * Si se trata del proceso actual (e.g. se llama al método desde run)
   * devuelve el PID actual.
   *
   * Se asume que el proceso ya ha sido inicializado mediante
   * una llamada a start.
   */
  pid_t get_pid();

  /*
   * wait_for_exit
   *
   * Si se trata del proceso padre, espera a que el proceso finalice.
   *
   * Si se trata del mismo proceso, no hace nada.
   *
   * Se asume que el proceso ya ha sido inicializado mediante
   * una llamada a start.
   *
   * Devuelve el estado de finalización del proceso.
   *
   */
  int wait_for_exit();

  /*
   * wait_for_children
   *
   * Si se trata del mismo proceso que ejecuta el código,
   * espera a que todos los hijos finalicen para luego continuar.
   */
  void wait_for_children();

  /*
   * wait_for_child
   *
   * Si el objeto representa al mismo proceso que ejecuta el
   * código, espera a que finalice algún hijo. En caso de no
   * haber hijos, no hace nada.
   *
   */
  int wait_for_child();

  /*
   * is_self
   *
   * Devuelve true si el proceso ejecutando el código es efectivamente
   * el proceso que este objeto representa.
   *
   */
  bool is_self();

  /*
   * shutdown
   *
   * Finaliza el proceso, primero finalizando a todos los procesos
   * hijos y esperando la finalización de cada uno.
   *
   */
  void shutdown();

  /*
   * spawn_child
   *
   * Instancia un proceso hijo y lo registra en la estructura
   * de datos para eventual finalización.
   *
   * El tipo de proceso instanciado depende del objeto factory
   * recibido como parámetro.
   *
   */
  void spawn_child(const ProcessFactory &factory);

  virtual ~Process();

};

#endif
