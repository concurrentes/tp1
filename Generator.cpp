#include "ProcessFactory.h"
#include "Generator.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

Generator::Generator(unsigned int interval, unsigned int max = 0) {
  this->interval = interval;
  this->max = max;
  this->w_total = 0;
}

void Generator::register_factory(
  const ProcessFactory &factory, unsigned int w) {

  t_process_factory *f = new t_process_factory;
  f->factory = &factory;
  f->weight = w;
  factories.push_back(f);
  this->w_total += w;
}

int Generator::run() {
  for (unsigned int i = 0; max == 0 || i < max; i++) {
    sleep(interval);
    generate_random_process();
  }
  wait_for_children();
  return 0;
}

void Generator::generate_random_process() {
  int w_accumulated = 0;
  int r = rand() % w_total;

  std::vector<t_process_factory*>::iterator it;

  for (it = factories.begin(); it != factories.end(); ++it) {
    w_accumulated += (*it)->weight;

    if (r < w_accumulated) {
      const ProcessFactory *factory = (*it)->factory;
      spawn_child(*factory);
      return;
    }
  }
}

Generator::~Generator() {
  std::vector<t_process_factory*>::iterator it;
  for (it = factories.begin(); it != factories.end(); ++it) {
    delete *it;
  }
}
