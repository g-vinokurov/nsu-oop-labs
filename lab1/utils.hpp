#pragma once

#include <string>
#include <vector>

namespace utils {
    std::string shrink(std::string const & str);
    bool starts_with(std::string const & str, std::string const & pattern);
    std::vector<std::string> split(std::string const & str, char delimiter);
    int digit_to_int(char digit);
}
