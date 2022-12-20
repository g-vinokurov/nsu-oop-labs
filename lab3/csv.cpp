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

std::string csv::read_cell(std::istringstream & iss, char escape_char, char delimiter) {
    std::string out;
    bool is_quoted = (iss.peek() == escape_char);
    bool on_quote = is_quoted;
    int character = iss.get();
    if (is_quoted)
        character = iss.get();
    while (!iss.fail()) {
        if (character == escape_char) {
            if (is_quoted) {
                if (iss.peek() == delimiter) {
                    iss.get();
                    return "\"" + out + "\"";
                }
                if (iss.peek() == escape_char)
                    out.push_back(static_cast<char>(iss.get()));
                else {
                    std::string msg = "impossible to split cell: " + out + "\n";
                    throw csv::ColumnDataTypeError("ColumnDataTypeError: " + msg);
                }
            } else {
                out.push_back('\"');
                on_quote = !on_quote;
            }
        } else if (character == delimiter) {
            if (!on_quote)
                return csv::shrink((is_quoted) ? ("\"" + out + "\"") : out);
            out.push_back(static_cast<char>(character));
        } else
            out.push_back(static_cast<char>(character));
        character = iss.get();
    }
    return csv::shrink((is_quoted) ? ("\"" + out + "\"") : out);
}

std::vector<std::string> csv::split(std::string const & str, char escape_char, char delimiter) {
    std::vector<std::string> out;
    std::istringstream iss(str);
    while (!iss.fail())
        out.push_back(csv::read_cell(iss, escape_char, delimiter));
    return out;
}

std::string csv::shrink(std::string const & str) {
    std::string out = str;
    while (!out.empty() && std::isblank(out.front())) out.erase(out.begin());
    while (!out.empty() && std::isblank(out.back())) out.pop_back();
    return out;
}

void csv::go_to_pos(std::ifstream & file, std::streamoff pos) {
    file.clear(); file.seekg(pos);
}
