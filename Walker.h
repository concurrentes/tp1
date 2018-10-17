#ifndef WALKING_H_
#define WALKING_H_

#include "BlockingSharedQueue.h"
#include "Process.h"
#include "ProcessFactory.h"
#include "Person.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>

using namespace smstruct;

class WalkerFactory : public ProcessFactory {


public:

    Process *instantiate();

};

class Walker : public Process {

private:

    /*
     * Identificador de la ciudad, también utilizado para identificar
     * la cola del muelle de esta ciudad.
     */
    unsigned int id;

    /*
     * Path al archivo para lockear los muelles de la ciudad cuando
     * arriban barcos.
     */
    std::string lock_path;

    int fd;

    BlockingSharedQueue *walking_queue;

    int run();

    void spawn_child(person_t* person);

public:

    static const std::string BASE_PATH;

    Walker();

    ~Walker();

};

#endif
