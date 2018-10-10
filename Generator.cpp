#include "Logger.h"
#include "ProcessFactory.h"
#include "Generator.h"
#include "Configuration.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

Generator::Generator(unsigned int interval, unsigned int max = 0) {
  LOG(LOG_DEBUG, "Instanciando generador (T=" << interval << ")");
  this->interval = interval;
  this->max = max;
  this->w_total = 0;
}

void Generator::register_factory(
  ProcessFactory &factory, unsigned int w) {

  t_process_factory *f = new t_process_factory;
  f->factory = &factory;
  f->weight = w;
  factories.push_back(f);
  this->w_total += w;
}

int Generator::run() {
  LOG(LOG_DEBUG, "Ejecutando generador (T=" << interval << ")");

  for (unsigned int i = 0; !should_quit_gracefully() && (max == 0 || i < max); i++) {
    sleep(interval);
    generate_random_process();
    clean_zombies();
  }

  while (!should_quit_gracefully()) {
    sleep(Configuration::get_instance().get_zombie_cleanup_interval());
    clean_zombies();
  }

  return 0;
}

void Generator::generate_random_process() {
  int w_accumulated = 0;
  int r = rand() % w_total;

  std::vector<t_process_factory*>::iterator it;

  for (it = factories.begin(); it != factories.end(); ++it) {
    w_accumulated += (*it)->weight;

    if (r < w_accumulated) {
      ProcessFactory *factory = (*it)->factory;
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
