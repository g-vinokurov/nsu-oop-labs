#pragma once

#include <cstdlib>
#include <climits>
#include <cctype>

#include <string>
#include <stdexcept>
#include <iostream>

#define INT_BIT (sizeof(int) * CHAR_BIT)

class BigInt {
public:
    BigInt();
    BigInt(int);
    BigInt(std::string);
    BigInt(const BigInt &);
    ~BigInt();

    BigInt & operator=(const BigInt &);

    BigInt operator~() const;

    BigInt & operator++();
    const BigInt operator++(int);
    BigInt & operator--();
    const BigInt operator--(int);

    BigInt & operator+=(const BigInt &);
    BigInt & operator-=(const BigInt &);
    BigInt & operator*=(const BigInt &);
    BigInt & operator/=(const BigInt &);
    BigInt & operator%=(const BigInt &);

    BigInt & operator^=(const BigInt &);
    BigInt & operator&=(const BigInt &);
    BigInt & operator|=(const BigInt &);

    BigInt & operator<<=(size_t);
    BigInt & operator>>=(size_t);

    BigInt operator+() const;
    BigInt operator-() const;

    bool operator==(const BigInt &) const;
    bool operator!=(const BigInt &) const;
    bool operator<(const BigInt &) const;
    bool operator>(const BigInt &) const;
    bool operator<=(const BigInt &) const;
    bool operator>=(const BigInt &) const;

    operator int() const;
    operator std::string() const;

    size_t size() const;
    bool sign() const;
private:
    void resize(size_t);
    void autofit();

    size_t size_;
    unsigned int * data_;
};

BigInt operator+(const BigInt &, const BigInt &);
BigInt operator-(const BigInt &, const BigInt &);
BigInt operator*(const BigInt &, const BigInt &);
BigInt operator/(const BigInt &, const BigInt &);
BigInt operator%(const BigInt &, const BigInt &);

BigInt operator^(const BigInt &, const BigInt &);
BigInt operator&(const BigInt &, const BigInt &);
BigInt operator|(const BigInt &, const BigInt &);

std::ostream & operator<<(std::ostream & o, const BigInt & i);
bool string_is_bigint(std::string);
