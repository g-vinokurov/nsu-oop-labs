#include <iostream>
#include <string>

#include "Life.hpp"

void print_help(std::ostream & out, std::string const & msg);

int main(int argc, char ** argv) {
    if (argc == 3 or argc > 6) {
        print_help(std::cout, "Invalid number of arguments");
        return 0;
    }
    if (argc >= 4) {
        print_help(std::cout, "In development");
        return 0;
    }
    std::string filename = (argc == 1) ? "default.life" : argv[1];
    life::Life & life_instance = life::Life::init(filename);

    life::Life::Field field = life_instance.get_field();
    for (auto & row : field) {
        for (auto cell: row)
            std::cout << cell << " ";
        std::cout << std::endl;
    }
    return 0;
}

void print_help(std::ostream & out, std::string const & msg) {
    out << msg << std::endl;
}
