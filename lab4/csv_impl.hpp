#pragma once

template <typename ValueT, typename... Types>
csv::CSVIt<ValueT, Types...>::CSVIt(std::ifstream & file, ssize_t pos) : file_(file), item_pos_(pos), item_(nullptr) {
    if (this->item_pos_ != this->EOF_POS) {
        csv::go_to_pos(this->file_, this->item_pos_);
        this->item_ = this->parse_line(csv::read_line(this->file_));
    }
}

template <typename ValueT, typename... Types>
csv::CSVIt<ValueT, Types...>::CSVIt(CSVIt const & src) : file_(src.file_), item_pos_(src.item_pos_) {
    this->item_ = (src.item_ == nullptr) ? nullptr : new ValueT(*(src.item_));
}

template <typename ValueT, typename... Types>
csv::CSVIt<ValueT, Types...>::~CSVIt() {
    delete this->item_;
}

template <typename ValueT, typename... Types>
csv::CSVIt<ValueT, Types...> & csv::CSVIt<ValueT, Types...>::operator++() {
    if (this->item_pos_ == this->EOF_POS)
        return *this;
    csv::go_to_pos(this->file_, this->item_pos_);
    std::string curr_line = csv::read_line(this->file_);
    this->item_pos_ += curr_line.length() + 1;

    std::string next_line = csv::read_line(this->file_);
    if (this->file_.eof())
        this->item_pos_ = this->EOF_POS;

    delete this->item_;

    if (this->item_pos_ != this->EOF_POS)
        this->item_ = this->parse_line(next_line);
    else
        this->item_ = nullptr;
    return *this;
}

template <typename ValueT, typename... Types>
bool csv::CSVIt<ValueT, Types...>::operator!=(CSVIt const & other) const {
    return this->item_pos_ != other.item_pos_;
}

template <typename ValueT, typename... Types>
bool csv::CSVIt<ValueT, Types...>::operator==(CSVIt const & other) const {
    return this->item_pos_ == other.item_pos_;
}

template <typename ValueT, typename... Types>
typename csv::CSVIt<ValueT, Types...>::reference csv::CSVIt<ValueT, Types...>::operator*() const {
    return *(this->item_);
}

template <typename ValueT, typename... Types>
typename csv::CSVIt<ValueT, Types...>::pointer csv::CSVIt<ValueT, Types...>::operator->() const {
    return this->item_;
}

template <typename ValueT, typename... Types>
std::tuple<Types...> * csv::CSVIt<ValueT, Types...>::parse_line(std::string const & line) {
    std::vector<std::string> cells = csv::split(line, ',');
    return new std::tuple<Types...>(tuple::vstr_to_tuple<Types...>(cells));
}

template <typename... Types>
csv::CSVParser<Types...>::CSVParser(std::ifstream & file, size_t line_offset) : file_(file), beg_pos_(0) {
    this->beg_pos_ = csv::skip_lines(this->file_, line_offset);
}

template <typename... Types>
typename csv::CSVParser<Types...>::iterator csv::CSVParser<Types...>::begin() {
    return csv::CSVParser<Types...>::iterator(this->file_, this->beg_pos_);
}

template <typename... Types>
typename csv::CSVParser<Types...>::iterator csv::CSVParser<Types...>::end() {
    return csv::CSVParser<Types...>::iterator(this->file_);
}

template <typename... Types>
typename csv::CSVParser<Types...>::const_iterator csv::CSVParser<Types...>::begin() const {
    return csv::CSVParser<Types...>::const_iterator(this->file_, this->beg_pos_);
}

template <typename... Types>
typename csv::CSVParser<Types...>::const_iterator csv::CSVParser<Types...>::end() const {
    return csv::CSVParser<Types...>::const_iterator(this->file_);
}
