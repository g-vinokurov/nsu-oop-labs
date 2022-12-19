#pragma once

#include <sstream>
#include <string>

namespace utils {
    struct TypeConvertingError : public std::exception {
        explicit TypeConvertingError(std::string const & msg) : _msg(msg) {}
        virtual const char * what() const noexcept override { return _msg.c_str(); }
    private:
        std::string _msg;
    };
}

namespace utils {
    template <typename Type>
    struct StrToTypeConverter final {
        static Type convert(std::string & str) {
            std::istringstream iss(str);
            Type result;
            if (!(iss >> result)) {
                std::string msg = "impossible to convert \"" + str + "\"";
                msg += std::string(" to type \"") + typeid(Type).name() + "\"\n";
                throw TypeConvertingError("TypeConvertingError: " + msg);
            }
            return result;
        }
    };

    template <>
    struct StrToTypeConverter<std::string> final {
        static std::string convert(std::string & str) {
            return str;
        }
    };
}
