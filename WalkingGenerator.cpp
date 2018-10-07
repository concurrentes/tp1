#include "Logger.h"
#include "WalkingGenerator.h"

#include "BlockingSharedQueue.h"
#include "Configuration.h"

WalkingGenerator::WalkingGenerator() :
    Generator(0, 1),
    walking_queue(Configuration::get_instance().get_city_count()) {

    LOG(LOG_DEBUG, "Iniciando generador de caminatas");
    register_factory(factory, 1);
}


Process *WalkingGeneratorFactory::instantiate() {
    return new WalkingGenerator();
}
