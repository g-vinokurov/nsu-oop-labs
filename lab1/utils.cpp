#include "utils.hpp"

std::string utils::shrink(std::string const & str) {
    std::string out = str;
    while (!out.empty() && std::isblank(out.front())) out.erase(out.begin());
    while (!out.empty() && std::isblank(out.back())) out.pop_back();
    return out;
}

bool utils::starts_with(std::string const & str, std::string const & pattern) {
    size_t str_len = str.length();
    size_t pattern_len = pattern.length();
    if (str_len < pattern_len)
        return false;
    for (size_t i = 0; i != pattern_len; ++i)
        if (pattern[i] != str[i])
            return false;
    return true;
}

std::vector<std::string> utils::split(std::string const & str, char delimiter) {
    std::vector<std::string> values;
    size_t len = str.length();
    for (size_t i = 0; i < len; ++i) {
        std::string tmp;
        while (i < len && str[i] != delimiter)
            tmp.push_back(str[i++]);
        values.push_back(tmp);
    }
    return values;
}

int utils::digit_to_int(char digit) {
    switch (digit) {
        case '0': return 0;
        case '1': return 1;
        case '2': return 2;
        case '3': return 3;
        case '4': return 4;
        case '5': return 5;
        case '6': return 6;
        case '7': return 7;
        case '8': return 8;
        case '9': return 9;
        default : return 0;
    }
}
