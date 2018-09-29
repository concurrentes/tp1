#ifndef TICKETOFFICE_H
#define TICKETOFFICE_H
#include <iostream>

class TicketOffice {
public:
    TicketOffice();
    void sellTickets();

protected:
    std::string city;
    void _sendOkTo(const std::string& people_id);
};


#endif //TICKETOFFICE_H
