#ifndef PEOPLE_H
#define PEOPLE_H
#include <iostream>


class People {
public:
    People(int id);
    void buyTicket(const std::string& destination);
    bool embark(const std::string& dock);
    bool land(const std::string& dock);
    std::string serialize();
protected:
    int id;
    bool has_ticket;
    std::string destination;
    std::string origin;
    bool _waitForOk();
};


#endif //PEOPLE_H
