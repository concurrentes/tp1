#ifndef PROCESS_FACTORY_H_
#define PROCESS_FACTORY_H_

class Process;

class ProcessFactory {

public:

  /*
   * instantiate
   *
   * Instancia un proceso de un cierto tipo.
   *
   */
  virtual Process *instantiate() const = 0;

  virtual ~ProcessFactory() {};

};

#endif
