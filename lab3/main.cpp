#include <iostream>
#include <string>
#include <vector>

#define T0 std::string, std::string, int, int, int
#define T1 std::string, std::string, std::string, std::string, std::string, int
#define T2 int, int, int, std::string, int, int, int, std::string, std::string, std::string

#include "csv.hpp"

int main(int argc, char ** argv) {
    std::vector<std::string> filenames = { "biostats.csv", "addresses.csv", "cities.csv" };
    std::string filename = filenames[1];

    std::ifstream file(filename);
    csv::CSVParser<T1> parser(file, 0);
    // parser.set_col_delimiter(';');
    parser.set_escape_char('\\');
    for (std::tuple<T1> & item : parser)
        std::cout << item << std::endl;

    return 0;
}
