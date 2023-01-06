#include "LifePresenter.hpp"

life::LifePresenter & life::LifePresenter::init(life::Life & model, life::ILifeView & view) {
    static life::LifePresenter instance(model, view);
    return instance;
}

life::LifePresenter::LifePresenter(life::Life & model, life::ILifeView & view) : model_(model), view_(view) {}
