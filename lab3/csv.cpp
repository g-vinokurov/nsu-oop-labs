#include "csv.hpp"

void csv::CSVConfig::set_str_delimiter(char delimiter) {
    this->str_delimiter_ = delimiter;
}

void csv::CSVConfig::set_col_delimiter(char delimiter){
    this->col_delimiter_ = delimiter;
}

void csv::CSVConfig::set_escape_char(char character) {
    this->escape_char_ = character;
}

char csv::CSVConfig::get_str_delimiter() const {
    return this->str_delimiter_;
}

char csv::CSVConfig::get_col_delimiter() const {
    return this->col_delimiter_;
}

char csv::CSVConfig::get_escape_char() const {
    return this->escape_char_;
}

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

std::string csv::read_cell(std::istringstream & iss, char escape_char, char delimiter) {
    std::string out;
    while (!iss.fail() && std::isblank(iss.peek())) iss.get();
    bool inside_quote = false;
    int character = iss.get();
    while (!iss.fail()) {
        if (character == escape_char) {
            if (inside_quote) {
                if (iss.peek() == escape_char) {
                    out.push_back(static_cast<char>(iss.get()));
                } else {
                    out.push_back(static_cast<char>(character));
                    inside_quote = false;
                }
            } else {
                out.push_back(static_cast<char>(character));
                inside_quote = true;
            }
        } else if (character == delimiter) {
            if (inside_quote) {
                out.push_back(static_cast<char>(character));
            } else
                break;
        } else
            out.push_back(static_cast<char>(character));
        character = iss.get();
    }
    out = utils::shrink(out);
    return out;
}

std::vector<std::string> csv::split(std::string const & str, char escape_char, char delimiter) {
    std::vector<std::string> out;
    std::istringstream iss(str);
    while (!iss.fail())
        out.push_back(csv::read_cell(iss, escape_char, delimiter));
    return out;
}

void csv::go_to_pos(std::ifstream & file, std::streamoff pos) {
    file.clear(); file.seekg(pos);
}
