#ifndef WALKING_GENERATOR_H
#define WALKING_GENERATOR_H

#include "ProcessFactory.h"
#include "Generator.h"
#include "Walker.h"

class WalkerGenerator : public Generator {

private:
    BlockingSharedQueue walking_queue;
    WalkerFactory factory;

public:

    WalkerGenerator();

    virtual ~WalkerGenerator();

};

class WalkerGeneratorFactory : public ProcessFactory {


public:

    Process *instantiate();

};

#endif
