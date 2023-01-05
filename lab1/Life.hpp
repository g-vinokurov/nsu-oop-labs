#pragma once

#include <string>

#include "LifeConfig.hpp"
#include "LifeFileParser.hpp"

namespace life {
    class Life final {
    public:
        using Line = std::vector<bool>;
        using Field = std::vector<Line>;

        static Life & init(std::string const & filename = "");

        std::string get_universe_name() const;
        life::LifeConfig::TrRule get_birth_rule() const;
        life::LifeConfig::TrRule get_survival_rule() const;
        Field get_field() const;

        Life(Life const &) = delete;
        Life & operator=(Life const &) = delete;
    private:
        explicit Life(life::LifeConfig const & config);

        std::string universe_name_;
        Field field_;
        life::LifeConfig::TrRule b_rule_;
        life::LifeConfig::TrRule s_rule_;
    };
}
