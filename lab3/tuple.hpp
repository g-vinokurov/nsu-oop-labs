#pragma once

#include <exception>

#include <type_traits>
#include <ostream>
#include <tuple>

#include <string>
#include <vector>

#include "utils.hpp"

namespace tuple {
    class TupleError : public std::exception {
    public:
        explicit TupleError(std::string const & msg) : _msg(msg) {}
        virtual const char * what() const noexcept override { return _msg.c_str(); }
    private:
        std::string _msg;
    };

    class TupleConvertingError : public TupleError {
    public:
        explicit TupleConvertingError(std::string const & msg) : TupleError(msg) {}
    };

    class NoDataError : public TupleConvertingError {
    public:
        explicit NoDataError(std::string const & msg) : TupleConvertingError(msg) {}
    };

    class TypeConvertingError : public TupleConvertingError {
    public:
        explicit TypeConvertingError(std::string const & msg) : TupleConvertingError(msg) {}
    };
}

namespace tuple {
    class TuplePrinter final {
    public:
        template<typename Ch, typename Tr, typename... Args>
        static auto print(std::basic_ostream<Ch, Tr> & os, std::tuple<Args...> const & t) -> decltype(os) {
            return print_tuple<decltype(os), decltype(t), sizeof...(Args)>(os << "(", t) << ")";
        }
    private:
        template <typename OStream, typename Tuple, size_t N, std::enable_if_t<N == 0, bool> = true>
        static OStream & print_tuple(OStream & os, Tuple & t) {
            return os;
        }

        template <typename OStream, typename Tuple, size_t N, std::enable_if_t<N == 1, bool> = true>
        static OStream & print_tuple(OStream & os, Tuple & t) {
            return utils::print(os, std::get<0>(t));
        }

        template <typename OStream, typename Tuple, size_t N, std::enable_if_t<(N > 1), bool> = true>
        static OStream & print_tuple(OStream & os, Tuple & t) {
            OStream & rec = print_tuple<OStream, Tuple, N - 1>(os, t) << ", ";
            return utils::print(rec, std::get<N - 1>(t));
        }
    };

    class VStrToTupleConverter final {
    public:
        template <typename FirstT, typename SecondT, typename... Types>
        static std::tuple<FirstT, SecondT, Types...> convert(std::vector<std::string> & data) {
            std::tuple<FirstT> t = VStrToTupleConverter::convert_to_tuple<FirstT>(data);
            return std::tuple_cat(t, VStrToTupleConverter::convert<SecondT, Types...>(data));
        }

        template <typename Type>
        static std::tuple<Type> convert(std::vector<std::string> & data) {
            return VStrToTupleConverter::convert_to_tuple<Type>(data);
        }
    private:
        template <typename Type>
        static std::tuple<Type> convert_to_tuple(std::vector<std::string> & data) {
            if (data.empty())
                throw tuple::NoDataError("NoDataError: " + std::string("data is empty\n"));
            Type converted_value;
            try {
                converted_value = utils::StrToTypeConverter<Type>::convert(data.front());
            }
            catch (utils::TypeConvertingError & e) {
                throw tuple::TypeConvertingError(e.what());
            }
            std::tuple<Type> t = std::make_tuple(converted_value);
            data.erase(data.begin());
            return t;
        }
    };
}

template <typename Ch, typename Tr, typename... Args>
auto operator<<(std::basic_ostream<Ch, Tr> & os, std::tuple<Args...> const & t) -> decltype(os) {
    return tuple::TuplePrinter::print(os, t);
}
