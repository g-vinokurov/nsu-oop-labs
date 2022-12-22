#include "utils.hpp"

std::string utils::shrink(std::string const & str) {
    std::string out = str;
    while (!out.empty() && std::isblank(out.front())) out.erase(out.begin());
    while (!out.empty() && std::isblank(out.back())) out.pop_back();
    return out;
}
