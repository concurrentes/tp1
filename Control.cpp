#include "Control.h"
#include <unistd.h>
#include <fcntl.h>
#define SHM "/usr/bin/base64"
#define LETTER  'a'
#define LOCKFILE "/var/lock/control"
#include "Lock.h"
#include "Semaforo.h"

Control::Control() {
    _shared_mem.crear(SHM, LETTER);
    this->_psem = new Semaforo(SEM);
}

Control::~Control(){
    _shared_mem.liberar();
    delete this->_psem;
}


unsigned int Control::get_next_id() {
    this->_psem->p();
    control_t control = _shared_mem.leer();
    control.last_id += 1;
    _shared_mem.escribir(control);
    this->_psem->v();
    return control.last_id;
}
void Control::initialize() {
    control_t control;
    control.last_id = 0;
    control.captured_ships = 0;
    control.discharged_count = 0;
    _shared_mem.escribir(control);
    this->_psem->v();
}

unsigned int Control::get_captured_ships_count() {
    this->_psem->p();
    control_t control = _shared_mem.leer();
    this->_psem->v();
    return control.captured_ships;
}
void Control::add_captured_ship() {
    this->_psem->p();
    control_t control = _shared_mem.leer();
    control.captured_ships += 1;
    _shared_mem.escribir(control);
    this->_psem->v();
}

unsigned int Control::get_discharged_count() {
    this->_psem->p();
    control_t control = _shared_mem.leer();
    this->_psem->v();
    return control.discharged_count;
}

void Control::add_discharged() {
    this->_psem->p();
    control_t control = _shared_mem.leer();
    control.discharged_count += 1;
    _shared_mem.escribir(control);
    this->_psem->v();
}