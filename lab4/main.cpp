#include <iostream>
#include <fstream>
#include <string>

#include "csv_parser.hpp"

int main(int argc, char ** argv) {
    std::ifstream file("file.csv");
    CSVParser<int, std::string> parser(file, 0);
    CSVParser<int, std::string>::iterator p = parser.begin();
    CSVParser<int, std::string>::iterator q = parser.end();
    for (; p != q; ++p);
    return 0;
}
