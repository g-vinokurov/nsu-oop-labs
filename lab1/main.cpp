#include "App.hpp"

int main(int argc, char ** argv) {
    if (argc == 3 or argc > 6) {
        std::cout << "Invalid number of arguments" << std::endl << std::endl;
        return app::HelpApp::execute(argc, argv);
    } else if (argc == 1 or argc == 2) {
        if (argc == 2 and std::string(argv[1]) == "--help")
            return app::HelpApp::execute(argc, argv);
        else
            return app::OnlineApp::execute(argc, argv);
    } else
        return app::OfflineApp::execute(argc, argv);
}
