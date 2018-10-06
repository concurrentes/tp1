#ifndef WALKING_H_
#define WALKING_H_

#include "BlockingSharedQueue.h"
#include "Process.h"
#include "ProcessFactory.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>

using namespace smstruct;

class WalkingFactory : public ProcessFactory {


public:

    Process *instantiate();

};

class Walking : public Process {

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

public:

    static const std::string BASE_PATH;

    Walking();

    ~Walking();


};

#endif