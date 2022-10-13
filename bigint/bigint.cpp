#include "bigint.hpp"

BigInt::BigInt() {
    this->size_ = 1;
    this->data_ = new unsigned int[this->size_];
    this->data_[0] = 0;
}

BigInt::BigInt(int value) {
    this->size_ = 1;
    this->data_ = new unsigned int[this->size_];
    this->data_[0] = value;
}

BigInt::BigInt(std::string str) {
    if (!string_is_bigint(str))
        throw std::invalid_argument("Invalid argument\n");

    size_t str_size = str.size();
    size_t pos = 0;
    while (std::isspace(str[pos]))
        ++pos;
    bool is_neg = (str[pos] == '-');
    pos = (str[pos] == '+' || str[pos] == '-') ? pos + 1 : pos;

    this->size_ = 1;
    this->data_ = new unsigned int[this->size_];
    this->data_[0] = 0;
    for (size_t i = pos; i != str_size; ++i) {
        *this *= 10;
        *this += (str[i] - '0');
    }
    *this = (is_neg) ? -(*this) : *this;
}

BigInt::BigInt(const BigInt & src) {
    this->size_ = src.size_;
    this->data_ = new unsigned int[this->size_];
    for (size_t i = 0; i != this->size_; ++i)
        this->data_[i] = src.data_[i];
}

BigInt::~BigInt() {
    delete [] this->data_;
}

BigInt & BigInt::operator=(const BigInt & src) {
    if (this != &src) {
        delete [] this->data_;
        this->size_ = src.size_;
        this->data_ = new unsigned int[this->size_];
        for (size_t i = 0; i != this->size_; ++i)
            this->data_[i] = src.data_[i];
    }
    return *this;
}

BigInt BigInt::operator~() const {
    BigInt result = *this;
    for (size_t i = 0; i != result.size_; ++i)
        result.data_[i] = ~(result.data_[i]);
    return result;
}

BigInt & BigInt::operator++() {
    return *this += 1;
}

const BigInt BigInt::operator++(int) {
    BigInt copy = *this;
    *this += 1;
    return copy;
}

BigInt & BigInt::operator--() {
    return *this -= 1;
}

const BigInt BigInt::operator--(int) {
    BigInt copy = *this;
    *this -= 1;
    return copy;
}

BigInt & BigInt::operator+=(const BigInt & other) {
    BigInt dst = *this;
    BigInt src = other;
    if (dst.size_ > src.size_)
        src.resize(dst.size_);
    if (src.size_ > dst.size_)
        dst.resize(src.size_);
    bool dst_old_sign = dst.sign();
    bool src_old_sign = src.sign();

    bool overflow = false;
    unsigned long long int s = 0;
    for (size_t i = 0; i != dst.size_; ++i) {
        s = static_cast<unsigned long long int>(dst.data_[i]) + src.data_[i] + overflow;
        overflow = (s > UINT_MAX);
        dst.data_[i] = static_cast<unsigned int>(s);
    }
    if (!(dst_old_sign ^ src_old_sign) && (dst.sign() != dst_old_sign)) {
        dst.resize(dst.size_ + 1);
        dst.data_[dst.size_ - 1] = (dst_old_sign) ? ~0 : 0;
    }
    dst.autofit();
    return (*this = dst);
}

BigInt & BigInt::operator-=(const BigInt & other) {
    return *this += (-other);
}

BigInt & BigInt::operator*=(const BigInt & other) {
    BigInt dst = *this;
    BigInt src = other;
    BigInt res(0);

    bool res_sign = (dst.sign() ^ src.sign());
    dst = (dst.sign()) ? -dst : dst;
    src = (src.sign()) ? -src : src;

    BigInt tmp(0);
    tmp.resize(dst.size_ + src.size_ + 1);
    unsigned long long int mul = 0;
    for (size_t i = 0; i != dst.size_; ++i) {
        for (size_t j = 0; j != src.size_; ++j) {
            mul = static_cast<unsigned long long int>(dst.data_[i]) * src.data_[j];
            tmp.data_[i + j] = static_cast<unsigned int>(mul);
            tmp.data_[i + j + 1] = static_cast<unsigned int>(mul >> INT_BIT);
            res += tmp;
            tmp.data_[i + j] = 0;
            tmp.data_[i + j + 1] = 0;
        }
    }
    res = (res_sign) ? -res : res;
    res.autofit();
    return (*this = res);
}

BigInt & BigInt::operator/=(const BigInt & other) {
    if (other == BigInt(0))
        throw std::domain_error("Division by zero\n");

    BigInt dst = *this;
    BigInt src = other;

    bool res_sign = (dst.sign() ^ src.sign());
    dst = (dst.sign()) ? -dst : dst;
    src = (src.sign()) ? -src : src;
    if (dst < src)
        return (*this = BigInt(0));
    if (dst == src)
        return (*this = (res_sign) ? BigInt(-1) : BigInt(1));
    if (src == BigInt(1))
        return (*this = (res_sign) ? -dst : dst);

    BigInt left(1);
    BigInt right = dst;
    BigInt mid = right + left;
    mid >>= 1;
    while (left != right) {
        BigInt tmp = mid * src;
        if ((tmp <= dst) && (dst - tmp < src))
            return (*this = (res_sign) ? -mid : mid);
        if (tmp < dst)
            left = mid;
        if (tmp > dst)
            right = mid;
        mid = (right + left);
        mid >>= 1;
    }
    return (*this = (res_sign) ? -mid : mid);
}

BigInt & BigInt::operator%=(const BigInt & other) {
    if (other == BigInt(0))
        throw std::domain_error("Division by zero\n");

    BigInt div = (*this) / other;

    BigInt dst_abs = (this->sign()) ? -(*this) : (*this);
    BigInt src_abs = (other.sign()) ? -(other) : (other);
    BigInt div_abs = (div.sign()) ? -(div) : (div);

    BigInt result = (dst_abs - div_abs * src_abs);
    return (*this = (this->sign()) ? -result : result);
}

BigInt & BigInt::operator^=(const BigInt & other) {
    BigInt dst = *this;
    BigInt src = other;
    if (dst.size_ > src.size_)
        src.resize(dst.size_);
    if (src.size_ > dst.size_)
        dst.resize(src.size_);
    for (size_t i = 0; i != dst.size_; ++i)
        dst.data_[i] ^= src.data_[i];
    dst.autofit();
    return (*this = dst);
}

BigInt & BigInt::operator&=(const BigInt & other) {
    BigInt dst = *this;
    BigInt src = other;
    if (dst.size_ > src.size_)
        src.resize(dst.size_);
    if (src.size_ > dst.size_)
        dst.resize(src.size_);
    for (size_t i = 0; i != dst.size_; ++i)
        dst.data_[i] &= src.data_[i];
    dst.autofit();
    return (*this = dst);
}

BigInt & BigInt::operator|=(const BigInt & other) {
    BigInt dst = *this;
    BigInt src = other;
    if (dst.size_ > src.size_)
        src.resize(dst.size_);
    if (src.size_ > dst.size_)
        dst.resize(src.size_);
    for (size_t i = 0; i != dst.size_; ++i)
        dst.data_[i] |= src.data_[i];
    dst.autofit();
    return (*this = dst);
}

BigInt & BigInt::operator<<=(size_t shift) {
    if (shift == 0)
        return *this;
    size_t q = shift / INT_BIT;
    size_t r = shift % INT_BIT;
    size_t new_size = this->size_ + q + (r != 0);
    this->resize(new_size);

    if (q != 0) {
        for (size_t i = new_size - (r != 0) - 1; i != q; --i)
            this->data_[i] = this->data_[i - q];
        this->data_[q] = this->data_[0];
    }
    for (size_t i = 0; i != q; ++i)
        this->data_[i] = 0;

    unsigned int carry_mask = ((~0) << (INT_BIT - r));
    unsigned int carry = 0;
    for (size_t i = new_size - 1; i != q; --i) {
        carry = (this->data_[i - 1] & carry_mask) >> (INT_BIT - r);
        this->data_[i] <<= r;
        this->data_[i] |= carry;
    }
    this->data_[q] <<= r;
    this->autofit();
    return *this;
}

BigInt & BigInt::operator>>=(size_t shift) {
    if (shift == 0)
        return *this;
    size_t q = shift / INT_BIT;
    size_t r = shift % INT_BIT;
    if (this->size_ <= q)
        return (*this = (this->sign()) ? BigInt(-1) : BigInt(0));

    size_t new_size = this->size_ - q;
    if (q != 0) {
        for (size_t i = 0; i != new_size; ++i)
            this->data_[i] = this->data_[i + q];
    }

    unsigned int extended_bits = (this->sign()) ? ~0 : 0;
    for (size_t i = new_size; i != this->size_; ++i)
        this->data_[i] = extended_bits;
    if (r == 0) {
        this->autofit();
        return *this;
    }

    unsigned int carry_mask = ~(~0 << r);
    unsigned int carry = (extended_bits & carry_mask) << (INT_BIT - r);
    for (size_t i = new_size - 1; i != 0; --i) {
        unsigned int tmp_carry = (this->data_[i] & carry_mask) << (INT_BIT - r);
        this->data_[i] >>= r;
        this->data_[i] |= carry;
        carry = tmp_carry;
    }
    this->data_[0] >>= r;
    this->data_[0] |= carry;
    this->autofit();
    return *this;
}

BigInt BigInt::operator+() const {
    return *this;
}

BigInt BigInt::operator-() const {
    return (~(*this)) + BigInt(1);
}

bool BigInt::operator==(const BigInt & other) const {
    BigInt tmp = (*this) ^ other;
    for (size_t i = 0; i != tmp.size_; ++i)
        if (tmp.data_[i] != 0)
            return false;
    return true;
}

bool BigInt::operator!=(const BigInt & other) const {
    return !((*this) == other);
}

bool BigInt::operator<(const BigInt & other) const {
    bool this_sign = this->sign();
    bool other_sign = other.sign();
    if (this_sign && !other_sign)
        return true;
    if (!this_sign && other_sign)
        return false;
    BigInt difference = *this - other;
    return difference.sign();
}

bool BigInt::operator>(const BigInt & other) const {
    bool this_sign = this->sign();
    bool other_sign = other.sign();
    if (this_sign && !other_sign)
        return false;
    if (!this_sign && other_sign)
        return true;
    BigInt difference = other - *this;
    return difference.sign();
}

bool BigInt::operator<=(const BigInt & other) const {
    return !(*this > other);
}

bool BigInt::operator>=(const BigInt & other) const {
    return !(*this < other);
}

BigInt::operator int() const {
    return this->data_[0];
}

BigInt::operator std::string() const {
    std::string result = "";
    const BigInt BIG_TEN(10);
    const BigInt BIG_ZERO(0);
    BigInt abs = (this->sign()) ? -(*this) : (*this);
    do {
        BigInt mod = abs % BIG_TEN;
        switch (static_cast<int>(mod)) {
            case 0: result = "0" + result; break;
            case 1: result = "1" + result; break;
            case 2: result = "2" + result; break;
            case 3: result = "3" + result; break;
            case 4: result = "4" + result; break;
            case 5: result = "5" + result; break;
            case 6: result = "6" + result; break;
            case 7: result = "7" + result; break;
            case 8: result = "8" + result; break;
            case 9: result = "9" + result; break;
            default: result = result; break;
        }
        abs /= BIG_TEN;
    } while (abs != BIG_ZERO);
    if (this->sign())
        result = "-" + result;
    return result;
}

size_t BigInt::size() const {
    return this->size_ * sizeof(unsigned int);
}

bool BigInt::sign() const {
    return (this->data_[this->size_ - 1] >> (INT_BIT - 1)) & 1;
}

void BigInt::resize(size_t new_size) {
    if (new_size == 0)
        return;
    if (this->size_ == new_size)
        return;
    unsigned int * new_data = new unsigned int[new_size];

    size_t copy_length = (this->size_ > new_size) ? new_size : this->size_;
    for (size_t i = 0; i != copy_length; ++i)
        new_data[i] = this->data_[i];

    unsigned int extended_bits = (this->sign()) ? ~0 : 0;
    for (size_t i = copy_length; i != new_size; ++i)
        new_data[i] = extended_bits;

    delete [] this->data_;
    this->data_ = new_data;
    this->size_ = new_size;
}

void BigInt::autofit() {
    bool sign = this->sign();
    unsigned int extended_bits = (sign) ? ~0 : 0;
    size_t cleaned = 0;

    for (size_t i = this->size_ - 1; i != 0; --i) {
        if (this->data_[i] != extended_bits)
            break;
        if (sign != ((this->data_[i - 1] >> (INT_BIT - 1)) & 1))
            break;
        ++cleaned;
    }
    this->resize(this->size_ - cleaned);
}

BigInt operator+(const BigInt & a, const BigInt & b) {
    BigInt result = a;
    result += b;
    return result;
}

BigInt operator-(const BigInt & a, const BigInt & b) {
    BigInt result = a;
    result -= b;
    return result;
}

BigInt operator*(const BigInt & a, const BigInt & b) {
    BigInt result = a;
    result *= b;
    return result;
}

BigInt operator/(const BigInt & a, const BigInt & b) {
    BigInt result = a;
    result /= b;
    return result;
}

BigInt operator%(const BigInt & a, const BigInt & b) {
    BigInt result = a;
    result %= b;
    return result;
}

BigInt operator^(const BigInt & a, const BigInt & b) {
    BigInt result = a;
    result ^= b;
    return result;
}

BigInt operator&(const BigInt & a, const BigInt & b) {
    BigInt result = a;
    result &= b;
    return result;
}

BigInt operator|(const BigInt & a, const BigInt & b) {
    BigInt result = a;
    result |= b;
    return result;
}

std::ostream & operator<<(std::ostream & o, const BigInt & i) {
    return o << static_cast<std::string>(i);
}

bool string_is_bigint(std::string str) {
    size_t str_size = str.size();
    size_t pos = 0;
    while (pos != str_size && std::isspace(str[pos]))
        ++pos;
    if (str_size == pos)
        return false;
    if (str[pos] != '+' && str[pos] != '-' && !std::isdigit(str[pos]))
        return false;
    if (str[pos] == '+' || str[pos] == '-')
        ++pos;
    if (str_size == pos)
        return false;
    for (size_t i = pos; i != str_size; ++i)
        if (!std::isdigit(str[i]))
            return false;
    return true;
}
