#include <iostream>
#include "Fifos/FifoLectura.h"
#include "Fifos/FifoEscritura.h"
#include "entities/People.h"
#include <sys/wait.h>
#include "entities/TicketOffice.h"

int main() {
    for (int x = 0; x < 10; x++) {
        pid_t pid = fork();
        if (pid == 0) {
            std::cout << "Persona: " << getpid();
            People people(getpid());
            people.buyTicket("florianopolis");
            std::cout << " Compro ticket" << std::endl;
            exit(0);
        }
    }
    pid_t pid = fork();
    if (pid == 0) {
        TicketOffice ticket_office;
        ticket_office.sellTickets();
        exit(0);
    }
    for (int x = 0; x < 11; x++) {
        wait(NULL);
    }
}