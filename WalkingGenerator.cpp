#include "Logger.h"
#include "WalkingGenerator.h"

WalkingGenerator::WalkingGenerator():Generator(0, 1) {
    LOG(LOG_DEBUG, "Iniciando generador de caminatas");
    register_factory(factory, 1);
}


Process *WalkingGeneratorFactory::instantiate() {
    return new WalkingGenerator();
}
