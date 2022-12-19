#pragma once

#include <iterator>
#include <sstream>
#include <fstream>
#include <limits>
#include <string>
#include <tuple>
#include <vector>

#include "tuple.hpp"

namespace csv {
    std::streamoff skip_lines(std::ifstream & file, size_t n, char delimiter = '\n');
    void go_to_pos(std::ifstream & file, std::streamoff pos);
    std::string read_line(std::ifstream & file, char delimiter = '\n');
    std::vector<std::string> split(std::string const & str, char delimiter);
}

namespace csv {
    template <typename ValueT, typename... Types>
    class CSVIt final : public std::iterator<std::input_iterator_tag, ValueT, ssize_t> {
    public:
        static ssize_t const EOF_POS = -1;

        explicit CSVIt(std::ifstream & file, ssize_t pos = EOF_POS);
        CSVIt(CSVIt const & src);
        ~CSVIt();

        CSVIt & operator++();
        bool operator!=(CSVIt const & other) const;
        bool operator==(CSVIt const & other) const;
        typename CSVIt::reference operator*() const;
        typename CSVIt::pointer operator->() const;

        CSVIt() = delete;
        CSVIt & operator=(CSVIt const &) = delete;
    private:
        std::tuple<Types...> * parse_line(std::string const & line);

        std::ifstream & file_;
        ssize_t item_pos_;
        ValueT * item_;
    };

    template <typename... Types>
    class CSVParser final {
    public:
        using value_type = std::tuple<Types...>;
        using iterator = CSVIt<value_type, Types...>;
        using const_iterator = CSVIt<value_type const, Types...>;

        CSVParser(std::ifstream & file, size_t line_offset);
        ~CSVParser() = default;

        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;

        CSVParser() = delete;
        CSVParser(CSVParser const &) = delete;
        CSVParser & operator=(CSVParser const &) = delete;
    private:
        std::ifstream & file_;
        size_t beg_pos_;
    };
}

#include "csv_impl.hpp"
