#pragma once

#include <exception>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>

#include "utils.hpp"

#include "LifeConstants.hpp"
#include "LifeConfig.hpp"

namespace life {
    class LifeParserError : public std::exception {
    public:
        explicit LifeParserError(std::string const & msg) : _msg(msg) {}
        virtual const char * what() const noexcept override { return _msg.c_str(); }
    private:
        std::string _msg;
    };
}

namespace life {
    class LifeFileParser final {
    public:
        static life::LifeConfig parse(std::string const & filename, std::ostream & err = std::cerr);
    private:
        static void parse_rule(std::string const & rule, life::LifeConfig & config, std::ostream & err = std::cerr);
        static void parse_field_size(std::string const & line, life::LifeConfig & config);
        static void parse_cells(std::ifstream & file, life::LifeConfig & config, std::ostream & err = std::cerr);
        static std::pair<long long, long long> parse_xy_value(std::string const & line);
    };
}
