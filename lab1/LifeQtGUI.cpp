#include "LifeQtGUI.hpp"

life::qt::LifeView & life::qt::LifeView::init(life::Life & model) {
    static life::qt::LifeView instance(model);
    return instance;
}

life::qt::LifeView::LifeView(life::Life & model) : presenter_(life::LifePresenter::init(model, *this)) {}

int life::qt::LifeView::exec(int argc, char ** argv) {
    QApplication app(argc, argv);
    life::qt::AppWindow window;
    window.show();
    return app.exec();
}

life::qt::AppWindow::AppWindow(QWidget * parent) : QWidget(parent) { }
