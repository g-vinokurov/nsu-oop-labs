#include "LifeConfig.hpp"

const std::string life::LifeConfig::DEFAULT_UNIVERSE_NAME("Universe");
const life::LifeConfig::TrRule life::LifeConfig::DEFAULT_BIRTH_RULE("000100000");
const life::LifeConfig::TrRule life::LifeConfig::DEFAULT_SURVIVAL_RULE("001100000");

void life::LifeConfig::set_universe_name(std::string const & str) {
    this->universe_name_ = str;
}

void life::LifeConfig::set_field_width(unsigned long long width) {
    this->field_width_ = (width > 0) ? width : this->field_width_;
}

void life::LifeConfig::set_field_height(unsigned long long height) {
    this->field_height_ = (height > 0) ? height : this->field_height_;
}

void life::LifeConfig::insert_live_cell(long long x, long long y) {
    auto real_x = (this->field_width_ - x % this->field_width_) % this->field_width_;
    auto real_y = (this->field_height_ - y % this->field_height_) % this->field_height_;
    this->live_cells_.insert(std::make_pair(real_x, real_y));
}

void life::LifeConfig::set_birth_rule(life::LifeConfig::TrRule const & rule) {
    this->b_rule_ = rule;
}

void life::LifeConfig::set_survival_rule(life::LifeConfig::TrRule const & rule) {
    this->s_rule_ = rule;
}

std::string life::LifeConfig::get_universe_name() const {
    return this->universe_name_;
}

unsigned long long life::LifeConfig::get_field_width() const {
    return this->field_width_;
}

unsigned long long life::LifeConfig::get_field_height() const {
    return this->field_height_;
}

std::pair<unsigned long long, unsigned long long> life::LifeConfig::get_field_size() const {
    return std::make_pair(this->field_width_, this->field_height_);
}

std::set<std::pair<long long, long long>> & life::LifeConfig::get_live_cells() {
    return this->live_cells_;
}

std::set<std::pair<long long, long long>> life::LifeConfig::get_live_cells() const {
    return this->live_cells_;
}

size_t life::LifeConfig::get_live_cells_count() const {
    return this->live_cells_.size();
}

life::LifeConfig::TrRule life::LifeConfig::get_birth_rule() const {
    return this->b_rule_;
}

life::LifeConfig::TrRule life::LifeConfig::get_survival_rule() const {
    return this->s_rule_;
}
