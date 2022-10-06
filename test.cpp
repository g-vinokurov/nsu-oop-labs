#include <climits>
#include <iostream>
#include <string>

#include "googletest/googletest/include/gtest/gtest.h"
#include "bigint/bigint.hpp"

TEST(Constructors, Default) {
    BigInt item;

    EXPECT_TRUE(static_cast<std::string>(item) == "0");
}

TEST(Constructors, FromInt) {
    BigInt item0(0);
    BigInt item1(256);
    BigInt item2(-256);
    BigInt item3(INT_MAX);
    BigInt item4(INT_MIN);

    EXPECT_TRUE(static_cast<std::string>(item0) == "0");
    EXPECT_TRUE(static_cast<std::string>(item1) == "256");
    EXPECT_TRUE(static_cast<std::string>(item2) == "-256");
    EXPECT_TRUE(static_cast<std::string>(item3) == "2147483647");
    EXPECT_TRUE(static_cast<std::string>(item4) == "-2147483648");
}

TEST(Constructors, FromString) {
    BigInt item0("0");
    BigInt item1("256");
    BigInt item2("-256");
    BigInt item3("2147483647");
    BigInt item4("-2147483648");
    BigInt item5("2147483648");
    BigInt item6("1234567890987654321234567890987654321234567890987654321234567890");
    BigInt item7("-1234567890987654321234567890987654321234567890987654321234567890");
    BigInt item8("    -123");
    BigInt item9("  \t +256");
    BigInt item10(" \n\r\t 65536");

    EXPECT_TRUE(static_cast<std::string>(item0) == "0");
    EXPECT_TRUE(static_cast<std::string>(item1) == "256");
    EXPECT_TRUE(static_cast<std::string>(item2) == "-256");
    EXPECT_TRUE(static_cast<std::string>(item3) == "2147483647");
    EXPECT_TRUE(static_cast<std::string>(item4) == "-2147483648");
    EXPECT_TRUE(static_cast<std::string>(item5) == "2147483648");
    EXPECT_TRUE(static_cast<std::string>(item6) == "1234567890987654321234567890987654321234567890987654321234567890");
    EXPECT_TRUE(static_cast<std::string>(item7) == "-1234567890987654321234567890987654321234567890987654321234567890");
    EXPECT_TRUE(static_cast<std::string>(item8) == "-123");
    EXPECT_TRUE(static_cast<std::string>(item9) == "256");
    EXPECT_TRUE(static_cast<std::string>(item10) == "65536");
    EXPECT_THROW(BigInt item11(" \n\t\r25678    "), std::invalid_argument);
    EXPECT_THROW(BigInt item12("abc123"), std::invalid_argument);
    EXPECT_THROW(BigInt item13("-"), std::invalid_argument);
    EXPECT_THROW(BigInt item14("+"), std::invalid_argument);
    EXPECT_THROW(BigInt item15(""), std::invalid_argument);
}

TEST(Constructors, CopyConstructor) {
    BigInt src0("0");
    BigInt src1("-1234567890");
    BigInt src2("1234567890");
    BigInt src3("2147483648");
    BigInt src4("1234567890987654321234567890987654321234567890987654321234567890");
    BigInt src5("-1234567890987654321234567890987654321234567890987654321234567890");

    BigInt dst0 = src0;
    BigInt dst1 = src1;
    BigInt dst2 = src2;
    BigInt dst3 = src3;
    BigInt dst4 = src4;
    BigInt dst5 = src5;

    EXPECT_TRUE(static_cast<std::string>(dst0) == "0");
    EXPECT_TRUE(static_cast<std::string>(dst1) == "-1234567890");
    EXPECT_TRUE(static_cast<std::string>(dst2) == "1234567890");
    EXPECT_TRUE(static_cast<std::string>(dst3) == "2147483648");
    EXPECT_TRUE(static_cast<std::string>(dst4) == "1234567890987654321234567890987654321234567890987654321234567890");
    EXPECT_TRUE(static_cast<std::string>(dst5) == "-1234567890987654321234567890987654321234567890987654321234567890");
}

TEST(Operators, Assignment) {
    BigInt src0("0");
    BigInt src1("-1234567890");
    BigInt src2("1234567890");
    BigInt src3("2147483648");
    BigInt src4("1234567890987654321234567890987654321234567890987654321234567890");
    BigInt src5("-1234567890987654321234567890987654321234567890987654321234567890");
    BigInt dst0, dst1, dst2, dst3, dst4, dst5;
    BigInt dst6("12345");

    dst0 = src0;
    dst1 = src1;
    dst2 = src2;
    dst3 = src3;
    dst4 = src4;
    dst5 = src5;
    dst6 = dst6;

    EXPECT_TRUE(static_cast<std::string>(dst0) == "0");
    EXPECT_TRUE(static_cast<std::string>(dst1) == "-1234567890");
    EXPECT_TRUE(static_cast<std::string>(dst2) == "1234567890");
    EXPECT_TRUE(static_cast<std::string>(dst3) == "2147483648");
    EXPECT_TRUE(static_cast<std::string>(dst4) == "1234567890987654321234567890987654321234567890987654321234567890");
    EXPECT_TRUE(static_cast<std::string>(dst5) == "-1234567890987654321234567890987654321234567890987654321234567890");
    EXPECT_TRUE(static_cast<std::string>(dst6) == "12345");
}

TEST(Operators, BitInversion) {
    BigInt src0("0");
    BigInt src1("-1234567890");
    BigInt src2("1234567890");
    BigInt src3("2147483648");
    BigInt src4("1234567890987654321234567890987654321234567890987654321234567890");
    BigInt src5("-1234567890987654321234567890987654321234567890987654321234567890");

    BigInt dst0 = ~src0;
    BigInt dst1 = ~src1;
    BigInt dst2 = ~src2;
    BigInt dst3 = ~src3;
    BigInt dst4 = ~src4;
    BigInt dst5 = ~src5;

    EXPECT_TRUE(static_cast<std::string>(dst0) == "-1");
    EXPECT_TRUE(static_cast<std::string>(dst1) == "1234567889");
    EXPECT_TRUE(static_cast<std::string>(dst2) == "-1234567891");
    EXPECT_TRUE(static_cast<std::string>(dst3) == "-2147483649");
    EXPECT_TRUE(static_cast<std::string>(dst4) == "-1234567890987654321234567890987654321234567890987654321234567891");
    EXPECT_TRUE(static_cast<std::string>(dst5) == "1234567890987654321234567890987654321234567890987654321234567889");
}

TEST(Operators, ToInt) {
    BigInt src0("0");
    BigInt src1("-1234567890");
    BigInt src2("1234567890");
    BigInt src3("2147483648");

    EXPECT_TRUE(static_cast<int>(src0) == 0);
    EXPECT_TRUE(static_cast<int>(src1) == -1234567890);
    EXPECT_TRUE(static_cast<int>(src2) == 1234567890);
    EXPECT_TRUE(static_cast<int>(src3) == -2147483648);
}

TEST(Methods, Sign) {
    BigInt src0("0");
    BigInt src1("-1234567890");
    BigInt src2("1234567890");
    BigInt src3("2147483648");
    BigInt src4("1234567890987654321234567890987654321234567890987654321234567890");
    BigInt src5("-1234567890987654321234567890987654321234567890987654321234567890");

    EXPECT_TRUE(src0.sign() == false);
    EXPECT_TRUE(src1.sign() == true);
    EXPECT_TRUE(src2.sign() == false);
    EXPECT_TRUE(src3.sign() == false);
    EXPECT_TRUE(src4.sign() == false);
    EXPECT_TRUE(src5.sign() == true);
}

TEST(Methods, Size) {
    BigInt src0("0");
    BigInt src1("-1234567890");
    BigInt src2("1234567890");
    BigInt src3("2147483648");
    BigInt src4("1234567890987654321234567890987654321234567890987654321234567890");
    BigInt src5("-1234567890987654321234567890987654321234567890987654321234567890");

    EXPECT_TRUE(src0.size() == 1 * sizeof(int));
    EXPECT_TRUE(src1.size() == 1 * sizeof(int));
    EXPECT_TRUE(src2.size() == 1 * sizeof(int));
    EXPECT_TRUE(src3.size() == 2 * sizeof(int));
    EXPECT_TRUE(src4.size() == 7 * sizeof(int));
    EXPECT_TRUE(src5.size() == 7 * sizeof(int));
}

// You should change the accessebility of methods "resize" and "autofit"
// from "private" to "public" in order to test them
/*
TEST(Methods, Resize) {
    BigInt src0("0");
    BigInt src1("-1234567890");
    BigInt src2("1234567890");
    BigInt src3("2147483648");
    BigInt src4("1234567890987654321234567890987654321234567890987654321234567890");
    BigInt src5("-1234567890987654321234567890987654321234567890987654321234567890");

    src0.resize(0);
    src1.resize(25);
    src2.resize(1);
    src3.resize(1);
    src4.resize(25);
    src5.resize(4);

    EXPECT_TRUE(src0.size() == 1 * sizeof(int));
    EXPECT_TRUE(src1.size() == 25 * sizeof(int));
    EXPECT_TRUE(src2.size() == 1 * sizeof(int));
    EXPECT_TRUE(src3.size() == 1 * sizeof(int));
    EXPECT_TRUE(src4.size() == 25 * sizeof(int));
    EXPECT_TRUE(src5.size() == 4 * sizeof(int));
}

TEST(Methods, Autofit) {
    BigInt src0("0");
    BigInt src1("-1234567890");
    BigInt src2("1234567890");
    BigInt src3("2147483648");
    BigInt src4("1234567890987654321234567890987654321234567890987654321234567890");
    BigInt src5("-1234567890987654321234567890987654321234567890987654321234567890");

    src0.resize(0);
    src1.resize(25);
    src2.resize(1);
    src3.resize(1);
    src4.resize(25);
    src5.resize(4);

    src0.autofit();
    src1.autofit();
    src2.autofit();
    src3.autofit();
    src4.autofit();
    src5.autofit();

    EXPECT_TRUE(src0.size() == 1 * sizeof(int));
    EXPECT_TRUE(src1.size() == 1 * sizeof(int));
    EXPECT_TRUE(src2.size() == 1 * sizeof(int));
    EXPECT_TRUE(src3.size() == 1 * sizeof(int));
    EXPECT_TRUE(src4.size() == 7 * sizeof(int));
    EXPECT_TRUE(src5.size() == 4 * sizeof(int));
}
*/

TEST(Operators, PlusAndAssign) {
    BigInt dst0("0"), src0("0");
    BigInt dst1("0"), src1("7");
    BigInt dst2("0"), src2("-7");
    BigInt dst3("3"), src3("7");
    BigInt dst4("-3"), src4("-7");
    BigInt dst5("5"), src5("-7");
    BigInt dst6("-5"), src6("7");
    BigInt dst7("2147483648"), src7("2147483648");
    BigInt dst8("-2147483648"), src8("-2147483648");

    dst0 += src0;
    dst1 += src1;
    dst2 += src2;
    dst3 += src3;
    dst4 += src4;
    dst5 += src5;
    dst6 += src6;
    dst7 += src7;
    dst8 += src8;

    EXPECT_TRUE(static_cast<std::string>(dst0) == "0");
    EXPECT_TRUE(static_cast<std::string>(dst1) == "7");
    EXPECT_TRUE(static_cast<std::string>(dst2) == "-7");
    EXPECT_TRUE(static_cast<std::string>(dst3) == "10");
    EXPECT_TRUE(static_cast<std::string>(dst4) == "-10");
    EXPECT_TRUE(static_cast<std::string>(dst5) == "-2");
    EXPECT_TRUE(static_cast<std::string>(dst6) == "2");
    EXPECT_TRUE(static_cast<std::string>(dst7) == "4294967296");
    EXPECT_TRUE(static_cast<std::string>(dst8) == "-4294967296");
}

TEST(Operators, MinusAndAssign) {
    BigInt dst0("0"), src0("0");
    BigInt dst1("0"), src1("-7");
    BigInt dst2("0"), src2("7");
    BigInt dst3("3"), src3("-7");
    BigInt dst4("-3"), src4("7");
    BigInt dst5("5"), src5("7");
    BigInt dst6("-5"), src6("-7");
    BigInt dst7("2147483648"), src7("-2147483648");
    BigInt dst8("-2147483648"), src8("2147483648");

    dst0 -= src0;
    dst1 -= src1;
    dst2 -= src2;
    dst3 -= src3;
    dst4 -= src4;
    dst5 -= src5;
    dst6 -= src6;
    dst7 -= src7;
    dst8 -= src8;

    EXPECT_TRUE(static_cast<std::string>(dst0) == "0");
    EXPECT_TRUE(static_cast<std::string>(dst1) == "7");
    EXPECT_TRUE(static_cast<std::string>(dst2) == "-7");
    EXPECT_TRUE(static_cast<std::string>(dst3) == "10");
    EXPECT_TRUE(static_cast<std::string>(dst4) == "-10");
    EXPECT_TRUE(static_cast<std::string>(dst5) == "-2");
    EXPECT_TRUE(static_cast<std::string>(dst6) == "2");
    EXPECT_TRUE(static_cast<std::string>(dst7) == "4294967296");
    EXPECT_TRUE(static_cast<std::string>(dst8) == "-4294967296");
}

TEST(Operators, UnaryMinus) {
    BigInt src0("0");
    BigInt src1("-1234567890");
    BigInt src2("1234567890");
    BigInt src3("2147483648");
    BigInt src4("1234567890987654321234567890987654321234567890987654321234567890");
    BigInt src5("-1234567890987654321234567890987654321234567890987654321234567890");

    src0 = -src0;
    src1 = -src1;
    src2 = -src2;
    src3 = -src3;
    src4 = -src4;
    src5 = -src5;

    EXPECT_TRUE(static_cast<std::string>(src0) == "0");
    EXPECT_TRUE(static_cast<std::string>(src1) == "1234567890");
    EXPECT_TRUE(static_cast<std::string>(src2) == "-1234567890");
    EXPECT_TRUE(static_cast<std::string>(src3) == "-2147483648");
    EXPECT_TRUE(static_cast<std::string>(src4) == "-1234567890987654321234567890987654321234567890987654321234567890");
    EXPECT_TRUE(static_cast<std::string>(src5) == "1234567890987654321234567890987654321234567890987654321234567890");
}

TEST(Operators, IncrementAndDecrement) {
    BigInt dst(0), src(1234567890);

    dst++;
    EXPECT_TRUE(static_cast<std::string>(dst) == "1");
    dst--;
    EXPECT_TRUE(static_cast<std::string>(dst) == "0");
    ++dst;
    EXPECT_TRUE(static_cast<std::string>(dst) == "1");
    --dst;
    EXPECT_TRUE(static_cast<std::string>(dst) == "0");
    dst = src++;
    EXPECT_TRUE(static_cast<std::string>(dst) == "1234567890");
    dst = src--;
    EXPECT_TRUE(static_cast<std::string>(dst) == "1234567891");
    dst = ++src;
    EXPECT_TRUE(static_cast<std::string>(dst) == "1234567891");
    dst = --src;
    EXPECT_TRUE(static_cast<std::string>(dst) == "1234567890");

    dst = BigInt("2147483647");
    dst++;
    EXPECT_TRUE(static_cast<std::string>(dst) == "2147483648");
    dst--;
    EXPECT_TRUE(static_cast<std::string>(dst) == "2147483647");
    --dst;
    EXPECT_TRUE(static_cast<std::string>(dst) == "2147483646");
    ++dst;
    EXPECT_TRUE(static_cast<std::string>(dst) == "2147483647");

    dst = BigInt("-2147483647");
    dst++;
    EXPECT_TRUE(static_cast<std::string>(dst) == "-2147483646");
    dst--;
    EXPECT_TRUE(static_cast<std::string>(dst) == "-2147483647");
    --dst;
    EXPECT_TRUE(static_cast<std::string>(dst) == "-2147483648");
    ++dst;
    EXPECT_TRUE(static_cast<std::string>(dst) == "-2147483647");
}

TEST(Operators, BitXorAndAssign) {
    BigInt dst0(0), src0(0);
    BigInt dst1(2), src1(2);
    BigInt dst2(-2), src2(-2);
    BigInt dst3(4), src3(3);
    BigInt dst4(3), src4(4);
    BigInt dst5(-2), src5(1);
    BigInt dst6(1), src6(-2);
    BigInt dst7("2147483648"), src7("2147483647");
    BigInt dst8("-2147483648"), src8("2147483647");

    dst0 ^= src0;
    dst1 ^= src1;
    dst2 ^= src2;
    dst3 ^= src3;
    dst4 ^= src4;
    dst5 ^= src5;
    dst6 ^= src6;
    dst7 ^= src7;
    dst8 ^= src8;

    EXPECT_TRUE(static_cast<std::string>(dst0) == "0");
    EXPECT_TRUE(static_cast<std::string>(dst1) == "0");
    EXPECT_TRUE(static_cast<std::string>(dst2) == "0");
    EXPECT_TRUE(static_cast<std::string>(dst3) == "7");
    EXPECT_TRUE(static_cast<std::string>(dst4) == "7");
    EXPECT_TRUE(static_cast<std::string>(dst5) == "-1");
    EXPECT_TRUE(static_cast<std::string>(dst6) == "-1");
    EXPECT_TRUE(static_cast<std::string>(dst7) == "4294967295");
    EXPECT_TRUE(static_cast<std::string>(dst8) == "-1");
}

TEST(Operators, BitConjunctionAndAssign) {
    BigInt dst0(0), src0(0);
    BigInt dst1(2), src1(2);
    BigInt dst2(-2), src2(-2);
    BigInt dst3(4), src3(3);
    BigInt dst4(3), src4(4);
    BigInt dst5(-2), src5(1);
    BigInt dst6(1), src6(-2);
    BigInt dst7("2147483648"), src7("2147483647");
    BigInt dst8("-2147483648"), src8("2147483647");

    dst0 &= src0;
    dst1 &= src1;
    dst2 &= src2;
    dst3 &= src3;
    dst4 &= src4;
    dst5 &= src5;
    dst6 &= src6;
    dst7 &= src7;
    dst8 &= src8;

    EXPECT_TRUE(static_cast<std::string>(dst0) == "0");
    EXPECT_TRUE(static_cast<std::string>(dst1) == "2");
    EXPECT_TRUE(static_cast<std::string>(dst2) == "-2");
    EXPECT_TRUE(static_cast<std::string>(dst3) == "0");
    EXPECT_TRUE(static_cast<std::string>(dst4) == "0");
    EXPECT_TRUE(static_cast<std::string>(dst5) == "0");
    EXPECT_TRUE(static_cast<std::string>(dst6) == "0");
    EXPECT_TRUE(static_cast<std::string>(dst7) == "0");
    EXPECT_TRUE(static_cast<std::string>(dst8) == "0");
}

TEST(Operators, BitDisjunctionAndAssign) {
    BigInt dst0(0), src0(0);
    BigInt dst1(2), src1(2);
    BigInt dst2(-2), src2(-2);
    BigInt dst3(4), src3(3);
    BigInt dst4(3), src4(4);
    BigInt dst5(-2), src5(1);
    BigInt dst6(1), src6(-2);
    BigInt dst7("2147483648"), src7("2147483647");
    BigInt dst8("-2147483648"), src8("2147483647");

    dst0 |= src0;
    dst1 |= src1;
    dst2 |= src2;
    dst3 |= src3;
    dst4 |= src4;
    dst5 |= src5;
    dst6 |= src6;
    dst7 |= src7;
    dst8 |= src8;

    EXPECT_TRUE(static_cast<std::string>(dst0) == "0");
    EXPECT_TRUE(static_cast<std::string>(dst1) == "2");
    EXPECT_TRUE(static_cast<std::string>(dst2) == "-2");
    EXPECT_TRUE(static_cast<std::string>(dst3) == "7");
    EXPECT_TRUE(static_cast<std::string>(dst4) == "7");
    EXPECT_TRUE(static_cast<std::string>(dst5) == "-1");
    EXPECT_TRUE(static_cast<std::string>(dst6) == "-1");
    EXPECT_TRUE(static_cast<std::string>(dst7) == "4294967295");
    EXPECT_TRUE(static_cast<std::string>(dst8) == "-1");
}

TEST(Operators, Eq) {
    BigInt a0(3), b0(5);
    BigInt a1(1), b1(1);
    BigInt a2(-1), b2(-3);
    BigInt a3(-1), b3(-1);

    BigInt a4("1234567890987654321234567890987654321234567890987654321234567890");
    BigInt b4("1234567890987654321234567000007654321234567890987654321234567890");
    BigInt a5("1234567890987654321234567890987654321234567890987654321234567890");
    BigInt b5("1234567890987654321234567890987654321234567890987654321234567890");
    BigInt a6("-1234567890987654321234567890987654321234567890987654321234567890");
    BigInt b6("-1234567890987654321234567000007654321234567890987654321234567890");
    BigInt a7("-1234567890987654321234567890987654321234567890987654321234567890");
    BigInt b7("-1234567890987654321234567890987654321234567890987654321234567890");

    EXPECT_FALSE(a0 == b0);
    EXPECT_TRUE(a1 == b1);
    EXPECT_FALSE(a2 == b2);
    EXPECT_TRUE(a3 == b3);
    EXPECT_FALSE(a4 == b4);
    EXPECT_TRUE(a5 == b5);
    EXPECT_FALSE(a6 == b6);
    EXPECT_TRUE(a7 == b7);
}

TEST(Operators, NotEq) {
    BigInt a0(3), b0(5);
    BigInt a1(1), b1(1);
    BigInt a2(-1), b2(-3);
    BigInt a3(-1), b3(-1);

    BigInt a4("1234567890987654321234567890987654321234567890987654321234567890");
    BigInt b4("1234567890987654321234567000007654321234567890987654321234567890");
    BigInt a5("1234567890987654321234567890987654321234567890987654321234567890");
    BigInt b5("1234567890987654321234567890987654321234567890987654321234567890");
    BigInt a6("-1234567890987654321234567890987654321234567890987654321234567890");
    BigInt b6("-1234567890987654321234567000007654321234567890987654321234567890");
    BigInt a7("-1234567890987654321234567890987654321234567890987654321234567890");
    BigInt b7("-1234567890987654321234567890987654321234567890987654321234567890");

    EXPECT_TRUE(a0 != b0);
    EXPECT_FALSE(a1 != b1);
    EXPECT_TRUE(a2 != b2);
    EXPECT_FALSE(a3 != b3);
    EXPECT_TRUE(a4 != b4);
    EXPECT_FALSE(a5 != b5);
    EXPECT_TRUE(a6 != b6);
    EXPECT_FALSE(a7 != b7);
}

TEST(Operators, LessThan) {
    BigInt a0(3), b0(5);
    BigInt a1(1), b1(1);
    BigInt a2(-1), b2(-3);
    BigInt a3(-1), b3(-1);

    BigInt a4("1234567890987654321234567890987654321234567890987654321234567890");
    BigInt b4("1234567890987654321234567000007654321234567890987654321234567890");
    BigInt a5("1234567890987654321234567890987654321234567890987654321234567890");
    BigInt b5("1234567890987654321234567890987654321234567890987654321234567890");
    BigInt a6("-1234567890987654321234567890987654321234567890987654321234567890");
    BigInt b6("-1234567890987654321234567000007654321234567890987654321234567890");
    BigInt a7("-1234567890987654321234567890987654321234567890987654321234567890");
    BigInt b7("-1234567890987654321234567890987654321234567890987654321234567890");

    EXPECT_TRUE(a0 < b0);
    EXPECT_FALSE(b0 < a0);
    EXPECT_FALSE(a1 < b1);
    EXPECT_FALSE(b1 < a1);
    EXPECT_FALSE(a2 < b2);
    EXPECT_TRUE(b2 < a2);
    EXPECT_FALSE(a3 < b3);
    EXPECT_FALSE(b3 < a3);
    EXPECT_FALSE(a4 < b4);
    EXPECT_TRUE(b4 < a4);
    EXPECT_FALSE(a5 < b5);
    EXPECT_FALSE(b5 < a5);
    EXPECT_TRUE(a6 < b6);
    EXPECT_FALSE(b6 < a6);
    EXPECT_FALSE(a7 < b7);
    EXPECT_FALSE(b7 < a7);
}

TEST(Operators, GreaterThan) {
    BigInt a0(3), b0(5);
    BigInt a1(1), b1(1);
    BigInt a2(-1), b2(-3);
    BigInt a3(-1), b3(-1);

    BigInt a4("1234567890987654321234567890987654321234567890987654321234567890");
    BigInt b4("1234567890987654321234567000007654321234567890987654321234567890");
    BigInt a5("1234567890987654321234567890987654321234567890987654321234567890");
    BigInt b5("1234567890987654321234567890987654321234567890987654321234567890");
    BigInt a6("-1234567890987654321234567890987654321234567890987654321234567890");
    BigInt b6("-1234567890987654321234567000007654321234567890987654321234567890");
    BigInt a7("-1234567890987654321234567890987654321234567890987654321234567890");
    BigInt b7("-1234567890987654321234567890987654321234567890987654321234567890");

    EXPECT_FALSE(a0 > b0);
    EXPECT_TRUE(b0 > a0);
    EXPECT_FALSE(a1 > b1);
    EXPECT_FALSE(b1 > a1);
    EXPECT_TRUE(a2 > b2);
    EXPECT_FALSE(b2 > a2);
    EXPECT_FALSE(a3 > b3);
    EXPECT_FALSE(b3 > a3);
    EXPECT_TRUE(a4 > b4);
    EXPECT_FALSE(b4 > a4);
    EXPECT_FALSE(a5 > b5);
    EXPECT_FALSE(b5 > a5);
    EXPECT_FALSE(a6 > b6);
    EXPECT_TRUE(b6 > a6);
    EXPECT_FALSE(a7 > b7);
    EXPECT_FALSE(b7 > a7);
}

TEST(Operators, LessEq) {
    BigInt a0(3), b0(5);
    BigInt a1(1), b1(1);
    BigInt a2(-1), b2(-3);
    BigInt a3(-1), b3(-1);

    BigInt a4("1234567890987654321234567890987654321234567890987654321234567890");
    BigInt b4("1234567890987654321234567000007654321234567890987654321234567890");
    BigInt a5("1234567890987654321234567890987654321234567890987654321234567890");
    BigInt b5("1234567890987654321234567890987654321234567890987654321234567890");
    BigInt a6("-1234567890987654321234567890987654321234567890987654321234567890");
    BigInt b6("-1234567890987654321234567000007654321234567890987654321234567890");
    BigInt a7("-1234567890987654321234567890987654321234567890987654321234567890");
    BigInt b7("-1234567890987654321234567890987654321234567890987654321234567890");

    EXPECT_TRUE(a0 <= b0);
    EXPECT_FALSE(b0 <= a0);
    EXPECT_TRUE(a1 <= b1);
    EXPECT_TRUE(b1 <= a1);
    EXPECT_FALSE(a2 <= b2);
    EXPECT_TRUE(b2 <= a2);
    EXPECT_TRUE(a3 <= b3);
    EXPECT_TRUE(b3 <= a3);
    EXPECT_FALSE(a4 <= b4);
    EXPECT_TRUE(b4 <= a4);
    EXPECT_TRUE(a5 <= b5);
    EXPECT_TRUE(b5 <= a5);
    EXPECT_TRUE(a6 <= b6);
    EXPECT_FALSE(b6 <= a6);
    EXPECT_TRUE(a7 <= b7);
    EXPECT_TRUE(b7 <= a7);
}

TEST(Operators, GreaterEq) {
    BigInt a0(3), b0(5);
    BigInt a1(1), b1(1);
    BigInt a2(-1), b2(-3);
    BigInt a3(-1), b3(-1);

    BigInt a4("1234567890987654321234567890987654321234567890987654321234567890");
    BigInt b4("1234567890987654321234567000007654321234567890987654321234567890");
    BigInt a5("1234567890987654321234567890987654321234567890987654321234567890");
    BigInt b5("1234567890987654321234567890987654321234567890987654321234567890");
    BigInt a6("-1234567890987654321234567890987654321234567890987654321234567890");
    BigInt b6("-1234567890987654321234567000007654321234567890987654321234567890");
    BigInt a7("-1234567890987654321234567890987654321234567890987654321234567890");
    BigInt b7("-1234567890987654321234567890987654321234567890987654321234567890");

    EXPECT_FALSE(a0 >= b0);
    EXPECT_TRUE(b0 >= a0);
    EXPECT_TRUE(a1 >= b1);
    EXPECT_TRUE(b1 >= a1);
    EXPECT_TRUE(a2 >= b2);
    EXPECT_FALSE(b2 >= a2);
    EXPECT_TRUE(a3 >= b3);
    EXPECT_TRUE(b3 >= a3);
    EXPECT_TRUE(a4 >= b4);
    EXPECT_FALSE(b4 >= a4);
    EXPECT_TRUE(a5 >= b5);
    EXPECT_TRUE(b5 >= a5);
    EXPECT_FALSE(a6 >= b6);
    EXPECT_TRUE(b6 >= a6);
    EXPECT_TRUE(a7 >= b7);
    EXPECT_TRUE(b7 >= a7);
}

TEST(Operators, MulAndAssign) {
    BigInt dst0(0), src0(0);
    BigInt dst1(0), src1(7);
    BigInt dst2(0), src2(-7);
    BigInt dst3(7), src3(0);
    BigInt dst4(-7), src4(0);

    BigInt dst5(INT_MAX), src5(1);
    BigInt dst6(INT_MIN), src6(1);
    BigInt dst7(1), src7(INT_MAX);
    BigInt dst8(1), src8(INT_MIN);

    BigInt dst9(3), src9(INT_MAX);
    BigInt dst10(3), src10(INT_MIN);
    BigInt dst11(INT_MAX), src11(3);
    BigInt dst12(INT_MIN), src12(3);

    BigInt dst13("2"), src13("2147483648");
    BigInt dst14("2147483648"), src14("2");

    BigInt dst15(INT_MAX), src15(-1);
    BigInt dst16(INT_MIN), src16(-1);
    BigInt dst17(-1), src17(INT_MAX);
    BigInt dst18(-1), src18(INT_MIN);

    BigInt dst19("2"), src19("2147483647");
    BigInt dst20("2147483647"), src20("2");
    BigInt dst21("2"), src21("-2147483648");
    BigInt dst22("-2147483648"), src22("2");
    BigInt dst23("-2147483648"), src23("-2");
    BigInt dst24("-2"), src24("-2147483648");

    dst0 *= src0;
    dst1 *= src1;
    dst2 *= src2;
    dst3 *= src3;
    dst4 *= src4;
    dst5 *= src5;
    dst6 *= src6;
    dst7 *= src7;
    dst8 *= src8;
    dst9 *= src9;
    dst10 *= src10;
    dst11 *= src11;
    dst12 *= src12;
    dst13 *= src13;
    dst14 *= src14;
    dst15 *= src15;
    dst16 *= src16;
    dst17 *= src17;
    dst18 *= src18;
    dst19 *= src19;
    dst20 *= src20;
    dst21 *= src21;
    dst22 *= src22;
    dst23 *= src23;
    dst24 *= src24;

    EXPECT_TRUE(static_cast<std::string>(dst0) == "0");
    EXPECT_TRUE(static_cast<std::string>(dst1) == "0");
    EXPECT_TRUE(static_cast<std::string>(dst2) == "0");
    EXPECT_TRUE(static_cast<std::string>(dst3) == "0");
    EXPECT_TRUE(static_cast<std::string>(dst4) == "0");
    EXPECT_TRUE(static_cast<std::string>(dst5) == "2147483647");
    EXPECT_TRUE(static_cast<std::string>(dst6) == "-2147483648");
    EXPECT_TRUE(static_cast<std::string>(dst7) == "2147483647");
    EXPECT_TRUE(static_cast<std::string>(dst8) == "-2147483648");
    EXPECT_TRUE(static_cast<std::string>(dst9) == "6442450941");
    EXPECT_TRUE(static_cast<std::string>(dst10) == "-6442450944");
    EXPECT_TRUE(static_cast<std::string>(dst11) == "6442450941");
    EXPECT_TRUE(static_cast<std::string>(dst12) == "-6442450944");
    EXPECT_TRUE(static_cast<std::string>(dst13) == "4294967296");
    EXPECT_TRUE(static_cast<std::string>(dst14) == "4294967296");
    EXPECT_TRUE(static_cast<std::string>(dst15) == "-2147483647");
    EXPECT_TRUE(static_cast<std::string>(dst16) == "2147483648");
    EXPECT_TRUE(static_cast<std::string>(dst17) == "-2147483647");
    EXPECT_TRUE(static_cast<std::string>(dst18) == "2147483648");
    EXPECT_TRUE(static_cast<std::string>(dst19) == "4294967294");
    EXPECT_TRUE(static_cast<std::string>(dst20) == "4294967294");
    EXPECT_TRUE(static_cast<std::string>(dst21) == "-4294967296");
    EXPECT_TRUE(static_cast<std::string>(dst22) == "-4294967296");
    EXPECT_TRUE(static_cast<std::string>(dst23) == "4294967296");
    EXPECT_TRUE(static_cast<std::string>(dst24) == "4294967296");
}

TEST(Operators, DivAndAssign) {
    BigInt dst0(25), src0(3);
    BigInt dst1(25), src1(-3);
    BigInt dst2(-25), src2(3);
    BigInt dst3(-25), src3(-3);
    BigInt dst4(25), src4(1);
    BigInt dst5(1), src5(25);
    BigInt dst6(-25), src6(1);
    BigInt dst7(1), src7(-25);

    BigInt dst8("123456789012"), src8("123");
    BigInt dst9("123456789012"), src9("-123");
    BigInt dst10("-123456789012"), src10("123");
    BigInt dst11("-123456789012"), src11("-123");

    BigInt dst12("123456789012"), src12(0);

    dst0 /= src0;
    dst1 /= src1;
    dst2 /= src2;
    dst3 /= src3;
    dst4 /= src4;
    dst5 /= src5;
    dst6 /= src6;
    dst7 /= src7;
    dst8 /= src8;
    dst9 /= src9;
    dst10 /= src10;
    dst11 /= src11;

    EXPECT_TRUE(static_cast<std::string>(dst0) == "8");
    EXPECT_TRUE(static_cast<std::string>(dst1) == "-8");
    EXPECT_TRUE(static_cast<std::string>(dst2) == "-8");
    EXPECT_TRUE(static_cast<std::string>(dst3) == "8");
    EXPECT_TRUE(static_cast<std::string>(dst4) == "25");
    EXPECT_TRUE(static_cast<std::string>(dst5) == "0");
    EXPECT_TRUE(static_cast<std::string>(dst6) == "-25");
    EXPECT_TRUE(static_cast<std::string>(dst7) == "0");
    EXPECT_TRUE(static_cast<std::string>(dst8) == "1003713731");
    EXPECT_TRUE(static_cast<std::string>(dst9) == "-1003713731");
    EXPECT_TRUE(static_cast<std::string>(dst10) == "-1003713731");
    EXPECT_TRUE(static_cast<std::string>(dst11) == "1003713731");

    EXPECT_THROW(dst12 /= src12, std::domain_error);
}

TEST(Operators, ModAndAssign) {
    BigInt dst0(25), src0(3);
    BigInt dst1(25), src1(-3);
    BigInt dst2(-25), src2(3);
    BigInt dst3(-25), src3(-3);
    BigInt dst4(25), src4(1);
    BigInt dst5(1), src5(25);
    BigInt dst6(-25), src6(1);
    BigInt dst7(1), src7(-25);

    BigInt dst8("123456789012"), src8("123");
    BigInt dst9("123456789012"), src9("-123");
    BigInt dst10("-123456789012"), src10("123");
    BigInt dst11("-123456789012"), src11("-123");

    BigInt dst12("123456789012"), src12(0);

    dst0 %= src0;
    dst1 %= src1;
    dst2 %= src2;
    dst3 %= src3;
    dst4 %= src4;
    dst5 %= src5;
    dst6 %= src6;
    dst7 %= src7;
    dst8 %= src8;
    dst9 %= src9;
    dst10 %= src10;
    dst11 %= src11;

    EXPECT_TRUE(static_cast<std::string>(dst0) == "1");
    EXPECT_TRUE(static_cast<std::string>(dst1) == "1");
    EXPECT_TRUE(static_cast<std::string>(dst2) == "-1");
    EXPECT_TRUE(static_cast<std::string>(dst3) == "-1");
    EXPECT_TRUE(static_cast<std::string>(dst4) == "0");
    EXPECT_TRUE(static_cast<std::string>(dst5) == "1");
    EXPECT_TRUE(static_cast<std::string>(dst6) == "0");
    EXPECT_TRUE(static_cast<std::string>(dst7) == "1");
    EXPECT_TRUE(static_cast<std::string>(dst8) == "99");
    EXPECT_TRUE(static_cast<std::string>(dst9) == "99");
    EXPECT_TRUE(static_cast<std::string>(dst10) == "-99");
    EXPECT_TRUE(static_cast<std::string>(dst11) == "-99");

    EXPECT_THROW(dst12 %= src12, std::domain_error);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
