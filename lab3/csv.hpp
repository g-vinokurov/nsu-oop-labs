#pragma once

#include <iterator>
#include <sstream>
#include <fstream>
#include <limits>
#include <string>
#include <tuple>
#include <vector>

#include <iostream>

#include "utils.hpp"
#include "tuple.hpp"

namespace csv {
    std::streamoff skip_lines(std::ifstream & file, size_t n, char delimiter);
    std::string read_line(std::ifstream & file, char delimiter);
    std::string read_cell(std::istringstream & iss, char escape_char, char delimiter);
    std::vector<std::string> split(std::string const & str, char escape_char, char delimiter);
    void go_to_pos(std::ifstream & file, std::streamoff pos);
}

namespace csv {
    struct CSVFormatError : public std::exception {
        explicit CSVFormatError(std::string const & msg) : _msg(msg) {}
        virtual const char * what() const noexcept override { return _msg.c_str(); }
    private:
        std::string _msg;
    };

    struct ColumnDataTypeError : public CSVFormatError {
        explicit ColumnDataTypeError(std::string const & msg) : CSVFormatError(msg) {}
    };

    struct StringDataTypeError : public CSVFormatError {
        explicit StringDataTypeError(std::string const & msg) : CSVFormatError(msg) {}
    };
}

namespace csv {
    struct CSVConfig {
        virtual ~CSVConfig() = default;
        char str_delimiter_ = '\n';
        char col_delimiter_ = ',';
        char escape_char_ = '\"';
    };

    template <typename ValueT, typename... Types>
    class CSVIt final : public std::iterator<std::input_iterator_tag, ValueT, ssize_t> {
    public:
        static ssize_t const EOF_POS = -1;

        explicit CSVIt(std::ifstream & file, CSVConfig config, ssize_t pos = EOF_POS);
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
        CSVConfig config_;
        ssize_t item_pos_;

        ValueT * item_ = nullptr;
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

        void set_str_delimiter(char delimiter = '\n');
        void set_col_delimiter(char delimiter = ',');
        void set_escape_char(char character = '\"');

        char get_str_delimiter();
        char get_col_delimiter();
        char get_escape_char();

        CSVParser() = delete;
        CSVParser(CSVParser const &) = delete;
        CSVParser & operator=(CSVParser const &) = delete;
    private:
        std::ifstream & file_;
        size_t beg_pos_;

        CSVConfig config_;
    };
}

#include "csv_impl.hpp"
