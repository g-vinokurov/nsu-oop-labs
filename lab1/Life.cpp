#include "Life.hpp"

life::Life & life::Life::init(std::string const & filename) {
    static life::Life instance(life::LifeFileParser::parse(filename));
    return instance;
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

life::Life::Field life::Life::get_field() const {
    return this->field_;
}

life::Life::Life(life::LifeConfig const & config) {
    this->universe_name_ = config.get_universe_name();
    this->b_rule_ = config.get_birth_rule();
    this->s_rule_ = config.get_survival_rule();
    auto field_w = config.get_field_width();
    auto field_h = config.get_field_height();
    this->field_ = life::Life::Field(field_h, life::Life::Line(field_w, false));
    for (auto & live_cell : config.get_live_cells())
        this->field_[live_cell.first][live_cell.second] = true;
}
