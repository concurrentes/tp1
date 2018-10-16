#ifndef WALKINGPERSON_H
#define WALKINGPERSON_H

#include "Process.h"
#include "WalkingPerson.h"
#include "Person.h"

class WalkingPerson: public Process {
public:
    WalkingPerson(person_t* person);
    int run();
    ~WalkingPerson();

private:
    person_t* person;
};


#endif //WALKINGPERSON_H
