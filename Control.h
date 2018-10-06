#ifndef CONTROL_H
#define CONTROL_H

#include "Semaforo.h"
#include "MemoriaCompartida.h"

#define SEM "/usr/bin/base32"

typedef struct {
    uint32_t last_id;
    uint32_t discharged_count;
    uint32_t captured_ships;
} __attribute__((packed)) control_t;

class Control {
public:
    Control();
    ~Control();
    unsigned int get_next_id();
    unsigned int get_captured_ships_count();
    void add_captured_ship();
    unsigned int get_discharged_count();
    void add_discharged();
    void initialize();
private:
    int fd;
    Semaforo* _psem;
    MemoriaCompartida<control_t> _shared_mem;

};


#endif //CONTROL_H
