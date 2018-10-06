#include "Logger.h"
#include "Process.h"
#include "City.h"
#include "Lock.h"
#include "Person.h"
#include "Boat.h"
#include "Control.h"
#include "Configuration.h"
#include "Walking.h"

#include <sstream>
#include <string>
#include <list>
#include <stdlib.h>
#include <unistd.h>

const std::string Walking::BASE_PATH = "/tmp/lakeconcu.dock.";

Process *WalkingFactory::instantiate() {
    return new Walking();
}

int Walking::run() {
    Configuration config = Configuration::get_instance();
    walking_queue = new BlockingSharedQueue(config.get_city_count());


    while (!should_quit_gracefully()) {
        std::list<void *> people;

        sleep(config.get_person_generation_mean_time());
        if (walking_queue->take(walking_queue->count(), people) == 0) {
            LOG(LOG_INFO, "No hay gente caminando");
        }

        std::list<void *>::iterator it;
        for (it = people.begin(); it != people.end(); ++it) {
            person_t *current = (person_t *) (*it);
            LOG(LOG_INFO, "Turista " << current->id << " saca fotos mientras camina");
            LOG(LOG_INFO, "Turista " << current->id << " llega a la ciudad " << current->destination);
            BlockingSharedQueue destination_queue(current->destination);
            destination_queue.enqueue((void*)current, sizeof(person_t), 0);
            delete current;
        }

        people.clear();
        sleep(config.get_mean_walker_time());
    }

    delete walking_queue;
    return 0;
}

Walking::Walking() {
}

Walking::~Walking() {
}
