#include "App.hpp"

int app::HelpApp::execute(int argc, char ** argv) {
    std::cout << "Usage: " << argv[0] << " [file] [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  --help              Display this information." << std::endl;
    std::cout << "  --iterations=<x>    Evolve <x> stages." << std::endl;
    std::cout << "  --output=<filename> Save the universe to <filename>." << std::endl;
    std::cout << "  -i <x>              Evolve <x> stages." << std::endl;
    std::cout << "  -o <filename>       Save the universe to <filename>." << std::endl;
    std::cout << "Use options \"--iterations (-i)\" and \"--output (-o)\" together" << std::endl;
    return 0;
}

int app::OnlineApp::execute(int argc, char ** argv) {
    /*
    std::string scenario = (argc == 2) ? argv[1] : app::OnlineApp::random_scenario();
    life::Life & life_instance = life::Life::init(scenario);
    qt5gui::App & qt5_app_instance = qt5gui::App::init(argc, argv);
    mvp::Presenter & presenter = mvp::Presenter::init(life_instance, qt5_app_instance);
    return presenter.exec();*/
    return 0;
}

int app::OfflineApp::execute(int argc, char ** argv) {
    try {
        app::OfflineApp::parse_args(argc, argv);
    } catch (app::ArgsParsingError & e) {
        std::cout << e.what() << std::endl << std::endl;
        return app::HelpApp().execute(argc, argv);
    }
    std::string scenario = argv[1];
    life::Life & life_instance = life::Life::init(scenario);
    life_instance.evolve(this->stages_);
    life_instance.dump(this->output_);
    return 0;
}

std::string app::OnlineApp::random_scenario() {
    static const size_t SCENARIO_NUMBER = 12;
    size_t scenario_index = std::rand() % SCENARIO_NUMBER;
    switch (scenario_index) {
        case 0: return "examples/_Block.life";
        case 1: return "examples/_BeeHive.life";
        case 2: return "examples/_Boat.life";
        case 3: return "examples/_Blinker.life";
        case 4: return "examples/_Toad.life";
        case 5: return "examples/_Beacon.life";
        case 6: return "examples/_Pulsar.life";
        case 7: return "examples/_PentaDecathlon.life";
        case 8: return "examples/_Glider.life";
        case 9: return "examples/_LightSpaceship.life";
        case 10: return "examples/_MiddleSpaceship.life";
        case 11: return "examples/_GosperGliderGun.life";
        default: return "examples/_Default.life";
    }
}

void app::OfflineApp::parse_args(int argc, char ** argv) {
    std::string scenario = argv[1];
    std::vector<std::string> args;
    for (int i = 2; i != argc; ++i)
        args.emplace_back(argv[i]);
    std::string iterations_val, output_val;
    switch (args.size()) {
        case 2: parse_long_prefix_args(args, iterations_val, output_val); break;
        case 3: parse_mix_prefix_args(args, iterations_val, output_val); break;
        case 4: parse_short_prefix_args(args, iterations_val, output_val); break;
        default: throw app::ArgsParsingError("Invalid number of arguments");
    }
    size_t stages_char_processed = 0;
    unsigned long long stages = 0;
    try {
        stages = std::stoull(iterations_val, &stages_char_processed);
    } catch (...) {
        throw app::ArgsParsingError("Arg of \"--iterations (-i)\" must be integer");
    }
    if (stages_char_processed != iterations_val.length())
        throw app::ArgsParsingError("Arg of \"--iterations (-i)\" must be integer");
    this->stages_ = stages;
    this->output_ = output_val;
}

void app::OfflineApp::parse_long_prefix_args(std::vector<std::string> & args, std::string & x, std::string & o) {
    bool fst_is_iterations = utils::starts_with(args[0], app::ARG_ITERATIONS_PREFIX_L);
    bool fst_is_output = utils::starts_with(args[0], app::ARG_OUTPUT_PREFIX_L);
    bool snd_is_iterations = utils::starts_with(args[1], app::ARG_ITERATIONS_PREFIX_L);
    bool snd_is_output = utils::starts_with(args[1], app::ARG_OUTPUT_PREFIX_L);
    if (not fst_is_iterations and not snd_is_iterations)
        throw app::ArgsParsingError("Invalid arguments");
    if (not fst_is_output and not snd_is_output)
        throw app::ArgsParsingError("Invalid arguments");
    if (fst_is_iterations) {
        x = args[0].erase(0, app::ARG_ITERATIONS_PREFIX_L.length());
        o = args[1].erase(0, app::ARG_OUTPUT_PREFIX_L.length());
    } else {
        x = args[1].erase(0, app::ARG_ITERATIONS_PREFIX_L.length());
        o = args[0].erase(0, app::ARG_OUTPUT_PREFIX_L.length());
    }
}

void app::OfflineApp::parse_mix_prefix_args(std::vector<std::string> & args, std::string & x, std::string & o) {
    bool fst_is_iterations_long = utils::starts_with(args[0], app::ARG_ITERATIONS_PREFIX_L);
    bool fst_is_iterations_short = (args[0] == app::ARG_ITERATIONS_PREFIX_S);
    bool fst_is_output_long = utils::starts_with(args[0], app::ARG_OUTPUT_PREFIX_L);
    bool fst_is_output_short = (args[0] == app::ARG_OUTPUT_PREFIX_S);
    bool snd_is_iterations_long = utils::starts_with(args[2], app::ARG_ITERATIONS_PREFIX_L);
    bool snd_is_iterations_short = (args[1] == app::ARG_ITERATIONS_PREFIX_S);
    bool snd_is_output_long = utils::starts_with(args[2], app::ARG_OUTPUT_PREFIX_L);
    bool snd_is_output_short = (args[1] == app::ARG_OUTPUT_PREFIX_S);
    bool fst_is_iterations = (fst_is_iterations_long or fst_is_iterations_short);
    bool fst_is_output = (fst_is_output_long or fst_is_output_short);
    bool snd_is_iterations = (snd_is_iterations_long or snd_is_iterations_short);
    bool snd_is_output = (snd_is_output_long or snd_is_output_short);
    if (not fst_is_iterations and not snd_is_iterations)
        throw app::ArgsParsingError("Invalid arguments");
    if (not fst_is_output and not snd_is_output)
        throw app::ArgsParsingError("Invalid arguments");
    if (fst_is_iterations_long) {
        x = args[0].erase(0, app::ARG_ITERATIONS_PREFIX_L.length());
        o = args[2];
    } else if (fst_is_output_long) {
        x = args[2];
        o = args[0].erase(0, app::ARG_OUTPUT_PREFIX_L.length());
    } else if (fst_is_iterations_short) {
        if (not snd_is_output_long)
            throw app::ArgsParsingError("Invalid arguments");
        else
            o = args[2].erase(0, app::ARG_OUTPUT_PREFIX_L.length());
        x = args[1];
    } else if (fst_is_output_short) {
        if (not snd_is_iterations_long)
            throw app::ArgsParsingError("Invalid arguments");
        else
            x = args[2].erase(0, app::ARG_ITERATIONS_PREFIX_L.length());
        o = args[1];
    }
}

void app::OfflineApp::parse_short_prefix_args(std::vector<std::string> & args, std::string & x, std::string & o) {
    bool fst_is_iterations = (args[0] == app::ARG_ITERATIONS_PREFIX_S);
    bool fst_is_output = (args[0] == app::ARG_OUTPUT_PREFIX_S);
    bool snd_is_iterations = (args[2] == app::ARG_ITERATIONS_PREFIX_S);
    bool snd_is_output = (args[2] == app::ARG_OUTPUT_PREFIX_S);
    if (not fst_is_iterations and not snd_is_iterations)
        throw app::ArgsParsingError("Invalid arguments");
    if (not fst_is_output and not snd_is_output)
        throw app::ArgsParsingError("Invalid arguments");
    x = (fst_is_iterations) ? args[1] : args[3];
    o = (fst_is_iterations) ? args[3] : args[1];
}
