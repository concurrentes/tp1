#include "Logger.h"
#include "WalkerGenerator.h"

#include "BlockingSharedQueue.h"
#include "Configuration.h"

WalkerGenerator::WalkerGenerator() :
    Generator(0, 1),
    walking_queue(Configuration::get_instance().get_city_count()) {

    LOG(LOG_DEBUG, "Iniciando generador de paseadores");
    register_factory(factory, 1);
}


Process *WalkerGeneratorFactory::instantiate() {
    return new WalkerGenerator();
}
