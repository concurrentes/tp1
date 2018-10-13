#include "WalkingPerson.h"
#include "Configuration.h"
#include "BlockingSharedQueue.h"
#include "Random.h"
#include "Logger.h"

using namespace smstruct;

WalkingPerson::WalkingPerson(person_t* person):Process() {
    this->person = person;
}

WalkingPerson::~WalkingPerson() {
    if (person) free(person);
}

int WalkingPerson::run() {
    Configuration config = Configuration::get_instance();
    LOG(LOG_INFO, "Turista " << person->id << " saca fotos mientras camina");
    unsigned int mu = config.get_mean_walker_time();
    unsigned int dt = mu/2;
    sleep(Random::random_wait(mu, dt));
    LOG(LOG_INFO, "Turista " << person->id << " llega a la ciudad " << person->destination);
    BlockingSharedQueue destination_queue(person->destination);
    destination_queue.enqueue(person, sizeof(person_t), 0);
}