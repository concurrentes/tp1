#include <stdlib.h>
#include <sstream>
#include "People.h"
#include "../Fifos/FifoEscritura.h"
#include "../Fifos/FifoLectura.h"
#include "../consts.h"
#include <iostream>
#include "../Lock/LockFile.h"

People::People(int id) {
    this->id = id;
    this->destination = "bolivia";
    this->origin = "bsas";
    this->has_ticket = false;
}

bool People::_waitForOk() {
    std::stringstream ss;
    ss << this->id;
    char buff[BUFFSIZE];
    std::string id = ss.str();
    FifoLectura people_fifo ("/tmp/people_" + id);
    people_fifo.abrir();
    ssize_t readBytes = people_fifo.leer(static_cast<void*>(buff), BUFFSIZE);
    people_fifo.cerrar();
    std::string message = buff;
    message.resize (readBytes);
    if (message.compare("ok")) return true;
    return false;
}

void People::buyTicket(const std::string &destination) {
    std::stringstream ss;
    ss << this->id;
    std::string id = ss.str();

    std::string message = id + "," + this->destination;
    FifoEscritura ticket_office("/tmp/" + this->origin + "_ticket_office");
    ticket_office.abrir();
    int size = message.length();
    LockFile lock("/tmp/" + this->origin + "_ticket_office");
    lock.tomarLock();
    ticket_office.escribir((char*)&size, sizeof(size));
    ticket_office.escribir(
            static_cast<const void*>(
                    message.c_str()),message.length()
    );
    ticket_office.cerrar();
    lock.liberarLock();
    if (this->_waitForOk()) {
        this->has_ticket = true;
    }

}
