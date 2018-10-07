#ifndef RANDOM_H_
#define RANDOM_H_

#include <stdlib.h>

class Random {

public:

  /*
   * Dado 0 <= p <= 100, bernoulli devuelve true con probabilidad p/100,
   * y devuelve false con probabilidad 1 - p/100.
   */
  static bool bernoulli(unsigned int p) {
    return (((unsigned int)rand() % 100) < p);
  }

  /*
   * Devuelve un entero distribuido uniformemente entre mu - delta
   * y mu + delta, ambos valores incluidos.
   */
  static unsigned int random_wait(unsigned int mu, unsigned int delta) {
    return mu - delta + rand() % (2*delta + 1);
  }

};

#endif
