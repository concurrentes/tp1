//
// Created by Bruno Liberini on 3/10/18.
//

#ifndef TP_CLIREADER_H
#define TP_CLIREADER_H

#include <string>
#include <iostream>

class CLIReader {
    private:
        int number_of_boats;
        int boat_capacity;
        void parse_cli(const int &argc, const char* argv[]);
        void write_help();
    public:
        CLIReader(const int &argc, const char* argv[]);
        ~CLIReader();
        int get_number_of_boats();
        int get_boat_capacity();
};

#endif //TP_CLIREADER_H
