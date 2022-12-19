#include <iostream>
#include <string>

#include "csv.hpp"

int main(int argc, char ** argv) {
    std::ifstream file("file.csv");
    csv::CSVParser<std::string, std::string, int, int, int> parser(file, 1);
    parser.set_col_delimiter(';');
    for (std::tuple<std::string, std::string, int, int, int> item : parser)
        std::cout << item << std::endl;
    return 0;
}
