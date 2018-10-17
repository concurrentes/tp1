#include "Logger.h"
#include "Process.h"
#include "City.h"
#include "Lock.h"
#include "Person.h"
#include "Boat.h"
#include "Control.h"
#include "Configuration.h"
#include "Walker.h"
#include "Random.h"
#include "WalkingPerson.h"

#include <sstream>
#include <string>
#include <list>
#include <stdlib.h>
#include <unistd.h>

const std::string Walker::BASE_PATH = "/tmp/lakeconcu.dock.";

Process *WalkerFactory::instantiate() {
    return new Walker();
}

int Walker::run() {
    Configuration config = Configuration::get_instance();
    walking_queue = new BlockingSharedQueue(config.get_city_count());


    while (!should_quit_gracefully()) {
        std::list<void *> people;

        unsigned int mu = config.get_mean_walker_time();
        unsigned int dt = mu/2;
        sleep(Random::random_wait(mu, dt));
        this->clean_zombies();

        if (walking_queue->take(walking_queue->count(), people) == 0) {
            LOG(LOG_INFO, "No hay turistas paseando");
        }

        std::list<void *>::iterator it;
        for (it = people.begin(); it != people.end() && !should_quit_gracefully(); ++it) {
            spawn_child((person_t*)(*it));
        }

        people.clear();
    }

    delete walking_queue;
    return 0;
}

void Walker::spawn_child(person_t* person) {
    if (!should_quit_gracefully()) {
        WalkingPerson *p = new WalkingPerson(person);
        p->start(p, this);
        children.push_back(p);
    }
}


Walker::Walker() {
  LOG(LOG_INFO, "Instanciado paseador de turistas");
}

Walker::~Walker() {
}
