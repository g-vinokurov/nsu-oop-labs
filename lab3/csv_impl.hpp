#pragma once

template <typename ValueT, typename... Types>
csv::CSVIt<ValueT, Types...>::CSVIt(std::ifstream & file, CSVConfig config, ssize_t pos) : file_(file), item_pos_(pos) {
    this->config_ = config;
    if (this->item_pos_ != this->EOF_POS) {
        csv::go_to_pos(this->file_, this->item_pos_);
        std::string line = csv::read_line(this->file_, this->config_.get_str_delimiter());
        while (utils::shrink(line).empty() && !this->file_.eof()) {
            this->item_pos_ += line.length() + 1;
            line = csv::read_line(this->file_, this->config_.get_str_delimiter());
        }
        if (this->file_.eof()) {
            this->item_pos_ = this->EOF_POS;
            this->item_ = nullptr;
        } else {
            this->item_ = this->parse_line(line);
        }
    }
}

template <typename ValueT, typename... Types>
csv::CSVIt<ValueT, Types...>::CSVIt(CSVIt const & src) : file_(src.file_), item_pos_(src.item_pos_), config_(src.config_) {
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
    std::string curr_line = csv::read_line(this->file_, this->config_.get_str_delimiter());
    this->item_pos_ += curr_line.length() + 1;

    std::string next_line = csv::read_line(this->file_, this->config_.get_str_delimiter());
    while (utils::shrink(next_line).empty() && !this->file_.eof()) {
        this->item_pos_ += next_line.length() + 1;
        next_line = csv::read_line(this->file_, this->config_.get_str_delimiter());
    }

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
    auto cells = csv::split(utils::shrink(line), this->config_.get_escape_char(), this->config_.get_col_delimiter());
    std::tuple<Types...> t;
    try {
       t = tuple::VStrToTupleConverter::convert<Types...>(cells);
    }
    catch (tuple::NoDataError & e) {
        std::cerr << e.what() << std::endl;
        std::string msg = "impossible to split line: " + line + "\n";
        throw csv::StringDataTypeError("StringDataTypeError: " + msg);
    }
    catch (tuple::TypeConvertingError & e) {
        std::cerr << e.what() << std::endl;
        std::string msg = "impossible to convert cells of line: " + line + "\n";
        throw csv::ColumnDataTypeError("StringDataTypeError: " + msg);
    }
    return new std::tuple<Types...>(t);
}

template <typename... Types>
csv::CSVParser<Types...>::CSVParser(std::ifstream & file, size_t line_offset) : file_(file), beg_pos_(0) {
    this->beg_pos_ = csv::skip_lines(this->file_, line_offset, this->config_.get_str_delimiter());
}

template <typename... Types>
typename csv::CSVParser<Types...>::iterator csv::CSVParser<Types...>::begin() {
    return csv::CSVParser<Types...>::iterator(this->file_, this->config_, this->beg_pos_);
}

template <typename... Types>
typename csv::CSVParser<Types...>::iterator csv::CSVParser<Types...>::end() {
    return csv::CSVParser<Types...>::iterator(this->file_, this->config_);
}

template <typename... Types>
typename csv::CSVParser<Types...>::const_iterator csv::CSVParser<Types...>::begin() const {
    return csv::CSVParser<Types...>::const_iterator(this->file_, this->config_, this->beg_pos_);
}

template <typename... Types>
typename csv::CSVParser<Types...>::const_iterator csv::CSVParser<Types...>::end() const {
    return csv::CSVParser<Types...>::const_iterator(this->file_, this->config_);
}

template <typename... Types>
void csv::CSVParser<Types...>::set_str_delimiter(char delimiter) {
    this->config_.set_str_delimiter(delimiter);
}

template <typename... Types>
void csv::CSVParser<Types...>::set_col_delimiter(char delimiter) {
    this->config_.set_col_delimiter(delimiter);
}

template <typename... Types>
void csv::CSVParser<Types...>::set_escape_char(char character) {
    this->config_.set_escape_char(character);
}

template <typename... Types>
char csv::CSVParser<Types...>::get_str_delimiter() const {
    return this->config_.get_str_delimiter();
}

template <typename... Types>
char csv::CSVParser<Types...>::get_col_delimiter() const {
    return this->config_.get_col_delimiter();
}

template <typename... Types>
char csv::CSVParser<Types...>::get_escape_char() const {
    return this->config_.get_escape_char();
}

