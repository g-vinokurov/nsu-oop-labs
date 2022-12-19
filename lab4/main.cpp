#include <iostream>
#include <string>

#include "csv.hpp"

int main(int argc, char ** argv) {
    std::ifstream file("file.csv");
    csv::CSVParser<int, int, int, int, int, int, int, double, int> parser(file, 1);
    for (std::tuple<int, int, int, int, int, int, int, double, int> item : parser)
        std::cout << item << std::endl;
    return 0;
}
