#pragma once

#include <iterator>
#include <tuple>

template <typename T>
class CSVIterator : public std::iterator<std::input_iterator_tag, T, ssize_t> {
public:
    CSVIterator & operator++();
    CSVIterator & operator++(int);
    bool operator!=(CSVIterator const &) const;
    bool operator==(CSVIterator const &) const;
    typename CSVIterator::reference operator*() const;
    typename CSVIterator::pointer operator->() const;
private:
    // data_ *
    // Copier
    // Defaulter
    // Destructor
};

template <typename... Types>
class CSVParser {
public:
    using value_type = std::tuple<Types...>;
    using iterator = CSVIterator<value_type>;
    using const_iterator = CSVIterator<value_type const>;

    iterator begin() { return iterator(); }
    iterator end() { return iterator(); }
    const_iterator begin() const { return const_iterator(); }
    const_iterator end() const { return const_iterator(); }
};
