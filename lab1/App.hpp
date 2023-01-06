#pragma once

#include <exception>
#include <iostream>
#include <utility>
#include <tuple>

#include "Life.hpp"
#include "LifeQtGUI.hpp"
#include "LifePresenter.hpp"

namespace app {
    const std::string ARG_ITERATIONS_PREFIX_L = "--iterations=";
    const std::string ARG_ITERATIONS_PREFIX_S = "-i";
    const std::string ARG_OUTPUT_PREFIX_L = "--output=";
    const std::string ARG_OUTPUT_PREFIX_S = "-o";
}

namespace app {
    class ArgsParsingError : public std::exception {
    public:
        explicit ArgsParsingError(std::string const & msg) : _msg(msg) {}
        virtual const char * what() const noexcept override { return _msg.c_str(); }
    private:
        std::string _msg;
    };
}

namespace app {
    class App {
    public:
        virtual ~App() = default;
    };

    class HelpApp final : public App {
    public:
        static int execute(int argc, char ** argv);
    };

    class OnlineApp final : public App {
    public:
        static int execute(int argc, char ** argv);
    private:
        static std::string random_scenario();
    };

    class OfflineApp final : public App {
    public:
        using ArgsTuple = std::tuple<size_t, std::string>;
        static int execute(int argc, char ** argv);
    private:
        static void parse_long_prefix_args(std::vector<std::string> & args, std::string & x, std::string & o);
        static void parse_mix_prefix_args(std::vector<std::string> & args, std::string & x, std::string & o);
        static void parse_short_prefix_args(std::vector<std::string> & args, std::string & x, std::string & o);
        static app::OfflineApp::ArgsTuple parse_args(int argc, char ** argv);
    };
}
