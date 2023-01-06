#pragma once

#include <QApplication>
#include <QWidget>
#include <QGridLayout>

namespace qt5gui {
    class App final {
    public:
        ~App() = default;

        static App & init(int argc, char ** argv);
        int exec();

        App(App const &) = delete;
        App & operator=(App const &) = delete;
    private:
        App(int argc, char ** argv);

        int argc_;
        char ** argv_;
    };

    class MainWindow : public QWidget {
    public:
        explicit MainWindow(QWidget * parent = nullptr);
    };
}
