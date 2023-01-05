#pragma once

#include <string>
#include <utility>
#include <bitset>
#include <set>

#include "utils.hpp"

namespace life {
    class CellPos final {
    public:
        CellPos(long long x, long long y) : pos_(std::make_pair(x, y)) {}
        long long & x() { return this->pos_.first; }
        long long x() const { return this->pos_.first; }
        long long & y() { return this->pos_.second; }
        long long y() const { return this->pos_.second; }

        bool operator<(CellPos const & other) const { return this->pos_ < other.pos_; }
        bool operator<=(CellPos const & other) const { return this->pos_ <= other.pos_; }
        bool operator>=(CellPos const & other) const { return this->pos_ >= other.pos_; }
        bool operator>(CellPos const & other) const { return this->pos_ > other.pos_; }
        bool operator!=(CellPos const & other) const { return this->pos_ != other.pos_; }
        bool operator==(CellPos const & other) const { return this->pos_ == other.pos_; }

        ~CellPos() = default;
    private:
        std::pair<long long, long long> pos_;
    };
}

namespace life {
    class LifeConfig final {
    public:
        using TrRule = std::bitset<9>;

        static const std::string DEFAULT_UNIVERSE_NAME;
        static const TrRule DEFAULT_BIRTH_RULE;
        static const TrRule DEFAULT_SURVIVAL_RULE;

        ~LifeConfig() = default;

        void set_universe_name(std::string const & str);
        void set_field_width(unsigned long long width);
        void set_field_height(unsigned long long height);
        void insert_live_cell(long long x, long long y);
        void set_birth_rule(TrRule const & rule);
        void set_survival_rule(TrRule const & rule);

        std::string get_universe_name() const;
        unsigned long long get_field_width() const;
        unsigned long long get_field_height() const;
        std::set<life::CellPos> & get_live_cells();
        std::set<life::CellPos> get_live_cells() const;
        size_t get_live_cells_count() const;
        TrRule get_birth_rule() const;
        TrRule get_survival_rule() const;
    private:
        std::string universe_name_ = LifeConfig::DEFAULT_UNIVERSE_NAME;
        unsigned long long field_width_ = 1;
        unsigned long long field_height_ = 1;
        std::set<life::CellPos> live_cells_;
        TrRule b_rule_ = LifeConfig::DEFAULT_BIRTH_RULE;
        TrRule s_rule_ = LifeConfig::DEFAULT_SURVIVAL_RULE;
    };
}
