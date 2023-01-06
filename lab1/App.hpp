#pragma once

#include <exception>
#include <iostream>
#include <utility>

#include "Life.hpp"
#include "Qt5GUI.hpp"
#include "Presenter.hpp"

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
    class IApp {
    public:
        virtual int execute(int argc, char ** argv) = 0;
        virtual ~IApp() = default;
    };

    class HelpApp final : public IApp {
    public:
        int execute(int argc, char ** argv) override;
    };

    class OnlineApp final : public IApp {
    public:
        int execute(int argc, char ** argv) override;
    private:
        static std::string random_scenario();
    };

    class OfflineApp final : public IApp {
    public:
        int execute(int argc, char ** argv) override;
    private:
        static void parse_long_prefix_args(std::vector<std::string> & args, std::string & x, std::string & o);
        static void parse_mix_prefix_args(std::vector<std::string> & args, std::string & x, std::string & o);
        static void parse_short_prefix_args(std::vector<std::string> & args, std::string & x, std::string & o);
        void parse_args(int argc, char ** argv);

        size_t stages_;
        std::string output_;
    };
}
