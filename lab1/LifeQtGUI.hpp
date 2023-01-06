#pragma once

#include "Life.hpp"
#include "LifeView.hpp"
#include "LifePresenter.hpp"

#include <QApplication>
#include <QWidget>

namespace life {
    namespace qt {
        class LifeView final : life::ILifeView {
        public:
            ~LifeView() override = default;

            static LifeView & init(life::Life & model);

            int exec(int argc, char ** arg) override;
        private:
            explicit LifeView(life::Life & model);

            life::LifePresenter & presenter_;
        };

        class AppWindow : public QWidget {
        public:
            explicit AppWindow(QWidget * parent = nullptr);
        };
    }
}
