#pragma once

#include <ostream>
#include <tuple>
#include <type_traits>

template <typename OStream, typename Tuple, size_t N, std::enable_if_t<N == 0, bool> = true>
OStream & print_tuple(OStream & os, Tuple & t) {
    return os;
}

template <typename OStream, typename Tuple, size_t N, std::enable_if_t<N == 1, bool> = true>
OStream & print_tuple(OStream & os, Tuple & t) {
    return os << std::get<0>(t);
}

template <typename OStream, typename Tuple, size_t N, std::enable_if_t<(N > 1), bool> = true>
OStream & print_tuple(OStream & os, Tuple & t) {
    return print_tuple<OStream, Tuple, N - 1>(os, t) << ", " << std::get<N - 1>(t);
}

template <typename Ch, typename Tr, typename... Args>
auto operator<<(std::basic_ostream<Ch, Tr> & os, std::tuple<Args...> const & t) -> decltype(os) {
    return print_tuple<decltype(os), decltype(t), sizeof...(Args)>(os << "(", t) << ")";
}
