#pragma once

#include <string>
#include <vector>

#include "utils.hpp"

#include "LifeConfig.hpp"
#include "LifeFileParser.hpp"

namespace life {
    class Life final {
    public:
        using Cell = bool;
        using Line = std::vector<Cell>;
        using Field = std::vector<Line>;

        ~Life() = default;

        static Life & init(std::string const & filename = "");
        void next_generation();
        void evolve(size_t stages = 1);

        std::string get_universe_name() const;
        life::LifeConfig::TrRule get_birth_rule() const;
        life::LifeConfig::TrRule get_survival_rule() const;
        Field get_field() const;

        void set_cell(long long row, long long col, Cell const & value);
        Cell get_cell(long long row, long long col) const;

        Life(Life const &) = delete;
        Life & operator=(Life const &) = delete;
    private:
        explicit Life(life::LifeConfig const & config);
        size_t count_neighbours(long long row, long long col) const;

        std::string universe_name_;
        Field field_;
        unsigned long long field_width_;
        unsigned long long field_height_;
        life::LifeConfig::TrRule b_rule_;
        life::LifeConfig::TrRule s_rule_;
    };
}
