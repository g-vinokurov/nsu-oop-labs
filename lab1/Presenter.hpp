#pragma once

#include "Life.hpp"
#include "Qt5GUI.hpp"

namespace mvp {
    class Presenter final {
    public:
        ~Presenter() = default;

        static Presenter & init(life::Life & model, qt5gui::App & view);
        int exec();

        Presenter(Presenter const &) = delete;
        Presenter & operator=(Presenter const &) = delete;
    private:
        Presenter(life::Life & model, qt5gui::App & view);

        life::Life & model_;
        qt5gui::App & view_;
    };
}
