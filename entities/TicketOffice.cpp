#include "TicketOffice.h"
#include "../Fifos/FifoLectura.h"
#include "../Fifos/FifoEscritura.h"
#include "../consts.h"
#include <sstream>



void TicketOffice::_sendOkTo(const std::string &people_id) {
    std::string mensaje = "ok";
    FifoEscritura persona ("/tmp/people_" + people_id);
    persona.abrir();
    persona.escribir(
            static_cast<const void*>(
                    mensaje.c_str()),mensaje.length()
    );
}

void TicketOffice::sellTickets() {
    FifoLectura ticket_office_fifo ("/tmp/" + this->city + "_ticket_office");
    ticket_office_fifo.abrir();
    char buffer[BUFFSIZE];
    int size = 0;
    ticket_office_fifo.leer(&size, sizeof(int));
    ssize_t read_bytes = ticket_office_fifo.leer(static_cast<void*>(buffer), size);
    while (read_bytes > 0) {
        std::string message = buffer;
        message.resize (read_bytes);
        std::cout <<"Ticket Office: " + message << std::endl;
        std::istringstream f(message);
        std::string id;
        getline(f, id, ',');
        if (! id.empty()) {
            this->_sendOkTo(id);
        }
        size = 0;
        ticket_office_fifo.leer(&size, sizeof(int));
        read_bytes = ticket_office_fifo.leer(static_cast<void*>(buffer), size);
    }
}


TicketOffice::TicketOffice() {
    this->city = "bsas";
}