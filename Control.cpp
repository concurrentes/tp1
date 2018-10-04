#include "Control.h"
#include <unistd.h>
#include <fcntl.h>
#define SHM "/usr/bin/base64"
#define LETTER  'a'
#define LOCKFILE "/var/lock/control"
#include "Lock.h"

Control::Control() {
    _shared_mem.crear(SHM, LETTER);
    this->fd = open (LOCKFILE, O_CREAT|O_WRONLY, 0777);
}

Control::~Control(){
    _shared_mem.liberar();
    close(this->fd);
}


unsigned int Control::get_next_id() {
    Lock lock(this->fd);
    control_t control = _shared_mem.leer();
    control.last_id += 1;
    _shared_mem.escribir(control);
    return control.last_id;
}
void Control::initialize() {
    Lock lock(this->fd);
    control_t control;
    control.last_id = 0;
    control.captured_ships = 0;
    control.discharged_count = 0;
    _shared_mem.escribir(control);
}

unsigned int Control::get_captured_ships_count() {
    Lock lock(this->fd);
    control_t control = _shared_mem.leer();
    return control.captured_ships;
}
void Control::add_captured_ship() {
    Lock lock(this->fd);
    control_t control = _shared_mem.leer();
    control.captured_ships += 1;
    _shared_mem.escribir(control);
}

unsigned int Control::get_discharged_count() {
    Lock lock(this->fd);
    control_t control = _shared_mem.leer();
    return control.discharged_count;
}

void Control::add_discharged() {
    Lock lock(this->fd);
    control_t control = _shared_mem.leer();
    control.discharged_count += 1;
    _shared_mem.escribir(control);
}