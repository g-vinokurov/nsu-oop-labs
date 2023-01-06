#include "Presenter.hpp"

mvp::Presenter & mvp::Presenter::init(life::Life & model, qt5gui::App & view) {
    static mvp::Presenter instance(model, view);
    return instance;
}

int mvp::Presenter::exec() {
    // sth
    return this->view_.exec();
}

mvp::Presenter::Presenter(life::Life & model, qt5gui::App & view) : model_(model), view_(view) {}

