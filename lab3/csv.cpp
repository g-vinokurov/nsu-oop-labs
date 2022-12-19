#include "csv.hpp"

std::streamoff csv::skip_lines(std::ifstream & file, size_t n, char delimiter) {
    file.clear(); file.seekg(0, std::ios_base::beg);
    for (size_t i = 0; i != n; ++i)
        file.ignore(std::numeric_limits<std::streamsize>::max(), file.widen(delimiter));
    return file.tellg();
}

std::string csv::read_line(std::ifstream & file, char delimiter) {
    std::string line;
    std::getline(file, line, delimiter);
    return line;
}

std::vector<std::string> csv::split(std::string const & str, char delimiter) {
    std::vector<std::string> out;
    std::istringstream iss(str);
    while (!iss.fail()) {
        std::string tmp;
        std::getline(iss, tmp, iss.widen(delimiter));
        out.push_back(tmp);
    }
    if (!out.empty())
        out.pop_back();
    return out;
}

std::string csv::shrink(std::string const & str) {
    std::string out = str;
    while (!out.empty() && std::isblank(out.front())) out.erase(0);
    while (!out.empty() && std::isblank(out.back())) out.pop_back();
    return out;
}

void csv::go_to_pos(std::ifstream & file, std::streamoff pos) {
    file.clear(); file.seekg(pos);
}
