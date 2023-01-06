#pragma once

#include "LifeView.hpp"
#include "Life.hpp"

namespace life {
    class LifePresenter final {
    public:
        ~LifePresenter() = default;

        static LifePresenter & init(life::Life & model, life::ILifeView & view);

        LifePresenter(LifePresenter const &) = delete;
        LifePresenter & operator=(LifePresenter const &) = delete;
    private:
        LifePresenter(life::Life & model, life::ILifeView & view);

        life::ILifeView & view_;
        life::Life & model_;
    };
}
