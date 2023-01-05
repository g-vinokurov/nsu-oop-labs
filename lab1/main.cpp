#include "App.hpp"

int main(int argc, char ** argv) {
    if (argc == 3 or argc > 6) {
        app::HelpApp();
    } else if (argc == 1 or argc == 2) {
        app::OnlineApp(argc, argv);
    } else
        app::OfflineApp(argc, argv);
    return 0;
}
