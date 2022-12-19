#pragma once

#include <iterator>
#include <fstream>
#include <limits>
#include <string>
#include <tuple>

void skip_lines(std::ifstream & file, size_t n) {
    file.clear(); file.seekg(0, std::ios_base::beg);
    for (size_t i = 0; i != n; ++i)
        file.ignore(std::numeric_limits<std::streamsize>::max(), file.widen('\n'));
}

void skip(std::ifstream & file, std::streamoff pos) {
    file.clear(); file.seekg(pos);
}

template <typename ValueT>
class CSVIterator final : public std::iterator<std::input_iterator_tag, ValueT, ssize_t> {
public:
    static ssize_t const EOF_POS = -1;

    CSVIterator(std::ifstream & file, ssize_t pos) : file_(file), item_pos_(pos), item_(nullptr) {}

    CSVIterator(CSVIterator const & src) : file_(src.file_), item_pos_(src.item_pos_) {
        this->item_ = (src.item_ == nullptr) ? nullptr : new ValueT(*(src.item_));
    }

    ~CSVIterator() {
        delete this->item_;
    }

    CSVIterator & operator++() {
        skip(this->file_, this->item_pos_);
        std::string line;
        std::getline(this->file_, line);
        this->file_.peek();
        this->item_pos_ = (this->file_.eof()) ? this->EOF_POS : this->item_pos_ + line.length() + 1;

        std::cout << line << std::endl;
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
    std::ifstream & file_;
    ssize_t item_pos_;
    ValueT * item_;
};

template <typename... Types>
class CSVParser final {
public:
    using value_type = std::tuple<Types...>;
    using iterator = CSVIterator<value_type>;
    using const_iterator = CSVIterator<value_type const>;

    CSVParser(std::ifstream & file, size_t line_offset) : file_(file), beg_pos_(0) {
        skip_lines(this->file_, line_offset);
        this->beg_pos_ = this->file_.tellg();
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
