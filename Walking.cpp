#include "Logger.h"
#include "Process.h"
#include "City.h"
#include "Lock.h"
#include "Person.h"
#include "Boat.h"
#include "Control.h"
#include "Configuration.h"

#include <sstream>
#include <string>
#include <list>
#include <stdlib.h>
#include "Walking.h"

const std::string Walking::BASE_PATH = "/tmp/lakeconcu.dock.";

Process *WalkingFactory::instantiate() {
    return new Walking();
}

Walking::Walking() {
    this->id = Configuration::get_instance().get_city_count();

    std::stringstream path_builder;

    path_builder << BASE_PATH << id;

    this->lock_path = path_builder.str();
    this->fd = open(this->lock_path.c_str(), O_CREAT, 0644);
}

int Walking::run() {
    walking_queue = new BlockingSharedQueue(this->id);

    LOG(LOG_INFO, "Iniciando Caminata " << this->id << ".");

    Configuration config = Configuration::get_instance();


    while (!should_quit_gracefully()) {
        std::list<void *> people;
        sleep(config.get_person_generation_mean_time());
        if (walking_queue->take(walking_queue->count(), people) == 0) {
            LOG(LOG_INFO, "No hay gente caminando");
        }
        std::list<void *>::iterator it;
        for (it = people.begin(); it != people.end(); ++it) {
            person_t *current = (person_t *) (*it);
            BlockingSharedQueue destination_queue(current->destination);
            LOG(LOG_INFO, current->id << " saca fotos mientras camina");
            destination_queue.enqueue((void*)current, sizeof(person_t), 0);
        }
    }

    delete walking_queue;
    return 0;
}

Walking::~Walking() {
    close(this->fd);
}
