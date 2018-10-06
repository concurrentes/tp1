#ifndef WALKING_GENERATOR_H
#define WALKING_GENERATOR_H

#include "ProcessFactory.h"
#include "Generator.h"
#include "Walking.h"

class WalkingGenerator : public Generator {

private:

    WalkingFactory factory;

public:

    WalkingGenerator();

};

class WalkingGeneratorFactory : public ProcessFactory {


public:

    Process *instantiate();

};

#endif