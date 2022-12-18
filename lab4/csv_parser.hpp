#pragma once

#include <iterator>
#include <fstream>
#include <string>
#include <tuple>

template <typename ValueT>
class CSVIterator final : public std::iterator<std::input_iterator_tag, ValueT, ssize_t> {
public:
    CSVIterator(std::ifstream & file, size_t pos) : file_(file), data_(nullptr), pos_(pos) {}
    CSVIterator(CSVIterator const & other) : file_(other.file_), data_(new ValueT(*other.data_)), pos_(other.pos_) {}

    ~CSVIterator() {
        delete this->data_;
    }
    CSVIterator & operator++() {
        this->file_.seekg(this->pos_);
        std::string line;
        std::getline(this->file_, line);
        this->pos_ += line.length() + 1;

        std::cout << line << std::endl;
        return *this;
    }
    bool operator!=(CSVIterator const & other) const {
        return this->pos_ != other.pos_;
    }
    bool operator==(CSVIterator const & other) const {
        return this->pos_ == other.pos_;
    }
    typename CSVIterator::reference operator*() const {
        return *(this->data_);
    }
    typename CSVIterator::pointer operator->() const {
        return this->data_;
    }

    CSVIterator() = delete;
    CSVIterator & operator=(CSVIterator const &) = delete;
private:
    std::ifstream & file_;
    ValueT * data_;
    size_t pos_;
};

template <typename... Types>
class CSVParser final {
public:
    using value_type = std::tuple<Types...>;
    using iterator = CSVIterator<value_type>;
    using const_iterator = CSVIterator<value_type const>;

    CSVParser(std::ifstream & file, size_t line_offset) : file_(file), start_pos_(0), end_pos_(0) {
        file.seekg(0, std::ios_base::beg);
        for (size_t i = 0; i != line_offset; ++i) {
            std::string tmp;
            std::getline(file, tmp);
            this->start_pos_ += tmp.length() + 1;
        }
        file.seekg(0, std::ios_base::end);
        this->end_pos_ = file.tellg();
    };

    iterator begin() { return iterator(this->file_, this->start_pos_); }
    iterator end() { return iterator(this->file_, this->end_pos_); }
    const_iterator begin() const { return const_iterator(this->file_, this->start_pos_); }
    const_iterator end() const { return const_iterator(this->file_, this->end_pos_); }

    ~CSVParser() = default;
    CSVParser() = delete;
    CSVParser(CSVParser const &) = delete;
    CSVParser & operator=(CSVParser const &) = delete;
private:
    std::ifstream & file_;
    size_t start_pos_;
    size_t end_pos_;
};
