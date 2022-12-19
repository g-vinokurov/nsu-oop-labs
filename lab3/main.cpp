#include <iostream>
#include <string>
#include <vector>

#include "csv.hpp"

int main(int argc, char ** argv) {
    std::vector<std::string> filenames = { "biostats.csv", "addresses.csv" };
    std::string filename = filenames[1];

    std::ifstream file(filename);
    csv::CSVParser<std::string, std::string, std::string, std::string, std::string, int> parser(file, 0);
    for (std::tuple<std::string, std::string, std::string, std::string, std::string, int> & item : parser)
        std::cout << item << std::endl;

    return 0;
}
