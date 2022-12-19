#pragma once

#include <iterator>
#include <sstream>
#include <fstream>
#include <limits>
#include <string>
#include <tuple>
#include <vector>

#include "tuple_printer.hpp"

template <typename Type>
std::tuple<Type> convert_to_tuple(std::vector<std::string> & data) {
    std::istringstream str(data.front());
    data.erase(data.begin());
    Type tmp;
    str >> tmp;
    return std::make_tuple(tmp);
}

template <typename FirstType, typename SecondType, typename... Types>
std::tuple<FirstType, SecondType, Types...> convert_to_tuple(std::vector<std::string> & data) {
    std::istringstream str(data.front());
    data.erase(data.begin());
    FirstType tmp;
    str >> tmp;
    return std::tuple_cat(std::make_tuple(tmp), convert_to_tuple<SecondType, Types...>(data));
}

std::streamoff skip_lines(std::ifstream & file, size_t n) { // need testing on another platform
    file.clear(); file.seekg(0, std::ios_base::beg);
    for (size_t i = 0; i != n; ++i)
        file.ignore(std::numeric_limits<std::streamsize>::max(), file.widen('\n'));
    return file.tellg();
}

void skip(std::ifstream & file, std::streamoff pos) {
    file.clear(); file.seekg(pos);
}

template <typename ValueT, typename... Types>
class CSVIterator final : public std::iterator<std::input_iterator_tag, ValueT, ssize_t> {
public:
    static ssize_t const EOF_POS = -1;

    CSVIterator(std::ifstream & file, ssize_t pos) : file_(file), item_pos_(pos), item_(nullptr) {
        if (this->item_pos_ != this->EOF_POS) {
            skip(this->file_, this->item_pos_);
            std::string line;
            std::getline(this->file_, line);
            this->item_ = this->parse_line(line);
        }
    }

    CSVIterator(CSVIterator const & src) : file_(src.file_), item_pos_(src.item_pos_) {
        this->item_ = (src.item_ == nullptr) ? nullptr : new ValueT(*(src.item_));
    }

    ~CSVIterator() {
        delete this->item_;
    }

    CSVIterator & operator++() { // need testing on another platform
        if (this->item_pos_ == this->EOF_POS)
            return *this;
        skip(this->file_, this->item_pos_);
        std::string curr_line, next_line;
        std::getline(this->file_, curr_line);
        this->item_pos_ += curr_line.length() + 1;

        std::getline(this->file_, next_line);
        this->item_pos_ = (this->file_.eof()) ? this->EOF_POS : this->item_pos_;

        delete this->item_;

        if (this->item_pos_ != this->EOF_POS)
            this->item_ = this->parse_line(next_line);
        else
            this->item_ = nullptr;
        return *this;
    }

    bool operator!=(CSVIterator const & other) const {
        return this->item_pos_ != other.item_pos_;
    }

    bool operator==(CSVIterator const & other) const {
        return this->item_pos_ == other.item_pos_;
    }

    typename CSVIterator::reference operator*() const {
        return *(this->item_);
    }

    typename CSVIterator::pointer operator->() const {
        return this->item_;
    }

    CSVIterator() = delete;
    CSVIterator & operator=(CSVIterator const &) = delete;
private:
    std::tuple<Types...> * parse_line(std::string & line) {
        std::vector<std::string> cells;
        std::istringstream str(line);
        while (!str.fail()) {
            std::string tmp;
            std::getline(str, tmp, str.widen(','));
            cells.push_back(tmp);
        }
        cells.pop_back();
        return new std::tuple<Types...>(convert_to_tuple<Types...>(cells));
    }

    std::ifstream & file_;
    ssize_t item_pos_;
    ValueT * item_;
};

template <typename... Types>
class CSVParser final {
public:
    using value_type = std::tuple<Types...>;
    using iterator = CSVIterator<value_type, Types...>;
    using const_iterator = CSVIterator<value_type const, Types...>;

    CSVParser(std::ifstream & file, size_t line_offset) : file_(file), beg_pos_(0) {
        this->beg_pos_ = skip_lines(this->file_, line_offset);
    };

    iterator begin() {
        return iterator(this->file_, this->beg_pos_);
    }

    iterator end() {
        return iterator(this->file_, iterator::EOF_POS);
    }

    const_iterator begin() const {
        return const_iterator(this->file_, this->beg_pos_);
    }

    const_iterator end() const {
        return const_iterator(this->file_, const_iterator::EOF_POS);
    }

    ~CSVParser() = default;

    CSVParser() = delete;
    CSVParser(CSVParser const &) = delete;
    CSVParser & operator=(CSVParser const &) = delete;
private:
    std::ifstream & file_;
    size_t beg_pos_;
};
