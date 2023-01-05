#include "Life.hpp"

life::Life & life::Life::init(std::string const & filename) {
    static life::Life instance(life::LifeFileParser::parse(filename));
    return instance;
}

void life::Life::next_generation() {
    life::Life::Line default_line(this->field_cols_, false);
    life::Life::Field new_field(this->field_rows_, default_line);
    for (unsigned long long i = 0; i != this->field_rows_; ++i)
        for (unsigned long long j = 0; j != this->field_cols_; ++j) {
            size_t neighbours = this->count_neighbours(i, j);
            if (not this->field_[i][j] and this->b_rule_[neighbours])
                new_field[i][j] = true;
            if (this->field_[i][j] and this->s_rule_[neighbours])
                new_field[i][j] = true;
        }
    this->field_ = new_field;
}

void life::Life::evolve(size_t stages) {
    for (size_t i = 0; i != stages; ++i)
        this->next_generation();
}

void life::Life::dump(std::string const & filename) {
    std::ofstream file(filename);
    file << life::LIFE_FILE_FORMAT << std::endl;
    file << life::UNIVERSE_NAME_PREFIX << this->universe_name_ << std::endl;
    file << life::UNIVERSE_RULE_PREFIX << this->get_transition_rule() << std::endl;
    file << this->field_rows_ << " " << this->field_cols_;
    for (size_t i = 0; i != this->field_rows_; ++i)
        for (size_t j = 0; j != this->field_cols_; ++j)
            if (this->field_[i][j])
                file << std::endl << i << " " << j;
}

std::string life::Life::get_universe_name() const {
    return this->universe_name_;
}

life::LifeConfig::TrRule life::Life::get_birth_rule() const {
    return this->b_rule_;
}

life::LifeConfig::TrRule life::Life::get_survival_rule() const {
    return this->s_rule_;
}

std::string life::Life::get_transition_rule() const {
    std::string rule = "B";
    for (size_t i = 0; i != 9; ++i)
        if (this->b_rule_[i]) rule += std::to_string(i);
    rule += "/S";
    for (size_t i = 0; i != 9; ++i)
        if (this->s_rule_[i]) rule += std::to_string(i);
    return rule;
}

life::Life::Field life::Life::get_field() const {
    return this->field_;
}

void life::Life::set_cell(long long row, long long col, life::Life::Cell const & value) {
    auto xy = utils::toroidal_xy(row, col, this->field_rows_, this->field_cols_);
    this->field_[xy.first][xy.second] = value;
}

life::Life::Cell life::Life::get_cell(long long row, long long col) const {
    auto xy = utils::toroidal_xy(row, col, this->field_rows_, this->field_cols_);
    return this->field_[xy.first][xy.second];
}

life::Life::Life(life::LifeConfig const & config) {
    this->universe_name_ = config.get_universe_name();
    this->b_rule_ = config.get_birth_rule();
    this->s_rule_ = config.get_survival_rule();
    this->field_rows_ = config.get_field_rows();
    this->field_cols_ = config.get_field_cols();

    life::Life::Line default_line(this->field_cols_, false);
    this->field_ = life::Life::Field(this->field_rows_, default_line);
    for (life::CellPos const & cell_pos : config.get_live_cells())
        this->field_[cell_pos.row()][cell_pos.col()] = true;
}

size_t life::Life::count_neighbours(long long row, long long col) const {
    size_t counter = 0;
    for (long long i = row - 1; i <= row + 1; ++i)
        for (long long j = col - 1; j <= col + 1; ++j)
            if (i != row or j != col)
                counter += (this->get_cell(i, j)) ? 1 : 0;
    return counter;
}
