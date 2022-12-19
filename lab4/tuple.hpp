#pragma once

#include <exception>

#include <type_traits>
#include <ostream>
#include <tuple>

#include <sstream>
#include <string>
#include <vector>

namespace tuple {
    struct TupleError : public std::exception {
        explicit TupleError(std::string const & msg) : _msg(msg) {}
        virtual const char * what() const noexcept override { return _msg.c_str(); }
    private:
        std::string _msg;
    };

    struct TypeConvertingError : public TupleError {
        explicit TypeConvertingError(std::string const & msg) : TupleError(msg) {}
    };

    struct NoDataError : public TupleError {
        explicit NoDataError(std::string const & msg) : TupleError(msg) {}
    };
}

namespace tuple {
    struct TuplePrinter final {
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
            return os << std::get<0>(t);
        }

        template <typename OStream, typename Tuple, size_t N, std::enable_if_t<(N > 1), bool> = true>
        static OStream & print_tuple(OStream & os, Tuple & t) {
            return print_tuple<OStream, Tuple, N - 1>(os, t) << ", " << std::get<N - 1>(t);
        }
    };

    template <typename Type>
    std::tuple<Type> vstr_to_tuple(std::vector<std::string> & data) {
        if (data.empty())
            throw NoDataError("data is empty\n");
        std::istringstream str(data.front());
        Type tmp;
        if (!(str >> tmp)) {
            std::string msg = "impossible to convert value \"" + data.front() + "\"";
            msg += std::string(" to type \"") + typeid(Type).name() + "\"\n";
            throw TypeConvertingError(msg);
        }
        data.erase(data.begin());
        return std::make_tuple(tmp);
    }

    template <typename FirstT, typename SecondT, typename... Types>
    std::tuple<FirstT, SecondT, Types...> vstr_to_tuple(std::vector<std::string> & data) {
        if (data.empty())
            throw NoDataError("data is empty\n");
        std::istringstream str(data.front());
        data.erase(data.begin());
        FirstT tmp;
        if (!(str >> tmp)) {
            std::string msg = "impossible to convert \"" + data.front() + "\"";
            msg += std::string(" to type \"") + typeid(FirstT).name() + "\"\n";
            throw TypeConvertingError(msg);
        }
        return std::tuple_cat(std::make_tuple(tmp), vstr_to_tuple<SecondT, Types...>(data));
    }
}

template <typename Ch, typename Tr, typename... Args>
auto operator<<(std::basic_ostream<Ch, Tr> & os, std::tuple<Args...> const & t) -> decltype(os) {
    return tuple::TuplePrinter::print(os, t);
}
