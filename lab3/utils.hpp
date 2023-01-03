#pragma once

#include <sstream>
#include <string>

namespace utils {
    class TypeConvertingError : public std::exception {
    public:
        explicit TypeConvertingError(std::string const & msg) : _msg(msg) {}
        virtual const char * what() const noexcept override { return _msg.c_str(); }
    private:
        std::string _msg;
    };
}

namespace utils {
    template <typename Type>
    class StrToTypeConverter final {
    public:
        static Type convert(std::string const & str) {
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
    class StrToTypeConverter<std::string> final {
    public:
        static std::string convert(std::string const & str) {
            return str;
        }
    };

    template<typename OStream, typename Type>
    OStream & print(OStream & os, Type const & value) {
        return os << value;
    }

    template<typename OStream>
    OStream & print(OStream & os, char value) {
        return os << "\'" << value << "\'";
    }

    template<typename OStream>
    OStream & print(OStream & os, const char * value) {
        return os << "\"" << value << "\"";
    }

    template<typename OStream>
    OStream & print(OStream & os, std::string const & value) {
        return os << "\"" << value << "\"";
    }

    std::string shrink(std::string const & str);
}
