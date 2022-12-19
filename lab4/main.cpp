#include <iostream>
#include <fstream>
#include <string>

#include "csv_parser.hpp"
#include "tuple_printer.hpp"

int main(int argc, char ** argv) {
    std::ifstream file("file.csv");
    CSVParser<int, int, int, int, int, int, int, double, int> parser(file, 1);
    for (std::tuple<int, int, int, int, int, int, int, double, int> item : parser)
        std::cout << item << std::endl;
    return 0;
}
