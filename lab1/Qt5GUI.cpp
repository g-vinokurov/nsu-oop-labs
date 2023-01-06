#include "Qt5GUI.hpp"

qt5gui::App & qt5gui::App::init(int argc, char ** argv) {
    static qt5gui::App instance(argc, argv);
    return instance;
}

int qt5gui::App::exec() {
    QApplication qt5_app(this->argc_, this->argv_);
    qt5gui::MainWindow window;
    window.show();
    return qt5_app.exec();
}

qt5gui::App::App(int argc, char ** argv) : argc_(argc), argv_(argv) { }

qt5gui::MainWindow::MainWindow(QWidget * parent) : QWidget(parent) { }
