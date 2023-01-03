#include <sstream>
#include <string>
#include <tuple>

#include "utils.hpp"
#include "tuple.hpp"
#include "csv.hpp"

#include "googletest/googletest/include/gtest/gtest.h"

using StrToInt = utils::StrToTypeConverter<int>;
using StrToChar = utils::StrToTypeConverter<char>;
using StrToDouble = utils::StrToTypeConverter<double>;

template <typename... Types>
std::string to_str(std::tuple<Types...> const & t) {
    std::basic_stringstream<char> ss;
    std::string str;
    ss << t << '\0';
    std::getline(ss, str, '\0');
    return str;
}

class ITupleToStr {
public:
    virtual std::string print() const = 0;
    virtual ~ITupleToStr() = default;
};

template <typename... Types>
class TupleToStr : public ITupleToStr {
public:
    explicit TupleToStr(std::tuple<Types...> const & value) : data_(value) {}
    std::string print() const override { return to_str(this->data_); }
private:
    std::tuple<Types...> data_;
};

class UtilsShrink : public ::testing::TestWithParam<std::tuple<std::string, std::string>> {};
class UtilsStrToInt : public ::testing::TestWithParam<std::tuple<std::string, int>> {};
class UtilsStrToChar : public ::testing::TestWithParam<std::tuple<std::string, char>> {};
class UtilsStrToDouble : public ::testing::TestWithParam<std::tuple<std::string, double>> {};

class TuplePrinterOneElementTuple : public ::testing::TestWithParam<std::tuple<ITupleToStr *, std::string>> {};

TEST_P(UtilsShrink, NoException) {
    EXPECT_TRUE(utils::shrink(std::get<0>(GetParam())) == std::get<1>(GetParam()));
}

TEST_P(UtilsStrToInt, NoException) {
    EXPECT_TRUE(StrToInt::convert(std::get<0>(GetParam())) == std::get<1>(GetParam()));
}

TEST(UtilsStrToInt, WithException) {
    EXPECT_THROW(StrToInt::convert("a123z"), utils::TypeConvertingError);
}

TEST_P(UtilsStrToChar, NoException) {
    EXPECT_TRUE(StrToChar::convert(std::get<0>(GetParam())) == std::get<1>(GetParam()));
}

TEST_P(UtilsStrToDouble, NoException) {
    EXPECT_TRUE(StrToDouble::convert(std::get<0>(GetParam())) == std::get<1>(GetParam()));
}

TEST(UtilsStrToDouble, WithException) {
    EXPECT_THROW(StrToDouble::convert("123e"), utils::TypeConvertingError);
}

TEST(TuplePrinterEmptyTuple, NoException) {
    EXPECT_TRUE(to_str(std::make_tuple()) == "()");
}

TEST_P(TuplePrinterOneElementTuple, NoException) {
    ITupleToStr * tuple_value = std::get<0>(GetParam());
    std::string expected = std::get<1>(GetParam());
    EXPECT_TRUE(tuple_value->print() == expected);
    delete tuple_value;
}

TEST(TuplePrinterPolyElementTuple, NoException) {
    auto t0 = std::make_tuple();
    auto t1 = std::make_tuple(2.4);
    auto t2 = std::make_tuple("abc");
    auto t3 = std::make_tuple('x');
    auto t4 = std::make_tuple(t0, 123, std::string("Inter\"stel\nlar"));
    auto t5 = std::make_tuple(t4, t1, t2, 'a', "b", std::string("c"), t3);
    auto str = "(((), 123, \"Inter\"stel\nlar\"), (2.4), (\"abc\"), 'a', \"b\", \"c\", ('x'))";
    EXPECT_TRUE(to_str(t5) == str);
}

TEST(CSVParser, test1) {
    using str = std::string;

    std::vector<str> tuples_as_str = {
        "(43, 37, 48, \"\"N\"\", 89, 46, 11, \"\"W\"\", \"\"Wisconsin Dells\"\", WI)",
        "(36, 5, 59, \"\"N\"\", 80, 15, 0, \"\"W\"\", \"\"Winston-Salem\"\", NC)"
    };
    size_t i = 0;

    std::ifstream file("test1.csv");
    csv::CSVConfig conf('\n', ';', '\"');
    csv::CSVParser<int, int, int, str, int, int, int, str, str, str> parser(file, 1, conf);
    for (std::tuple<int, int, int, str, int, int, int, str, str, str> t : parser)
        EXPECT_TRUE(to_str(t) == tuples_as_str[i++]);
}

TEST(CSVParser, test2) {
    using str = std::string;

    std::vector<str> tuples_as_str = {
            "(\"\"Alex\"\", \"\"M\"\", 41, 74, 170)",
            "(\"\"Fran\"\", \"\"F\"\", 33, 66, 115)"
    };
    size_t i = 0;

    std::ifstream file("test2.csv");
    csv::CSVParser<str, str, int, int, int> parser(file, 1);
    for (std::tuple<str, str, int, int, int> t : parser)
        EXPECT_TRUE(to_str(t) == tuples_as_str[i++]);
}

TEST(CSVParser, test3) {
    using str = std::string;

    std::vector<str> tuples_as_str = {
            R"(("John", "Doe", "120 jefferson st.", "Riverside", "NJ", 8075))",
            R"(("Jack", "McGinnis", "220 hobo Av.", "Phila", "PA", 9119))",
            R"((""John "Da Man""", "Repici", "120 Jefferson St.", "Riverside", "NJ", 8075))",
            R"(("Stephen", "Tyler", ""7452 Terrace "At the Plaza" road"", "SomeTown", "SD", 91234))",
            R"(("", "Blankman", "", "SomeTown", "SD", 298))",
            R"((""Joan "the bone", Anne"", "Jet", ""9th, at Terrace plc"", "Desert City", "CO", 123))"
    };
    size_t i = 0;

    std::ifstream file("test3.csv");
    csv::CSVParser<str, str, str, str, str, int> parser(file, 0);
    for (std::tuple<str, str, str, str, str, int> t : parser)
        EXPECT_TRUE(to_str(t) == tuples_as_str[i++]);
}

INSTANTIATE_TEST_SUITE_P(
    UtilsShrinkTests,
    UtilsShrink,
    ::testing::Values(
        std::make_tuple(std::string("abc"), std::string("abc")),
        std::make_tuple(std::string("   123z"), std::string("123z")),
        std::make_tuple(std::string("\t   2.4  g\t"), std::string("2.4  g")),
        std::make_tuple(std::string("abc\t\n\t  \t"), std::string("abc\t\n")),
        std::make_tuple(std::string("-5\n"), std::string("-5\n"))
));

INSTANTIATE_TEST_SUITE_P(
    UtilsStrToIntTests,
    UtilsStrToInt,
    ::testing::Values(
        std::make_tuple("123", 123),
        std::make_tuple("   123z", 123),
        std::make_tuple("2.4", 2),
        std::make_tuple("+12", 12),
        std::make_tuple("-5", -5)
));

INSTANTIATE_TEST_SUITE_P(
    UtilsStrToCharTests,
    UtilsStrToChar,
    ::testing::Values(
        std::make_tuple("a", 'a'),
        std::make_tuple("\tz", 'z'),
        std::make_tuple("\nx", 'x')
));

INSTANTIATE_TEST_SUITE_P(
    UtilsStrToDoubleTests,
    UtilsStrToDouble,
    ::testing::Values(
        std::make_tuple("123", 123.0),
        std::make_tuple("   123z", 123.0),
        std::make_tuple("2.4", 2.4),
        std::make_tuple("+12.3", 12.3),
        std::make_tuple("-5", -5.0)
));

INSTANTIATE_TEST_SUITE_P(
    OneElementTuplePrinterTests,
    TuplePrinterOneElementTuple,
    ::testing::Values(
        std::make_tuple( new TupleToStr<std::tuple<>>(std::make_tuple(std::make_tuple())),       "(())"),
        std::make_tuple( new TupleToStr<int>(std::make_tuple(123)),                              "(123)"),
        std::make_tuple( new TupleToStr<double>(std::make_tuple(2.4)),                           "(2.4)"),
        std::make_tuple( new TupleToStr<const char *>(std::make_tuple("Hi!\t\n")),               "(\"Hi!\t\n\")"),
        std::make_tuple( new TupleToStr<std::string>(std::make_tuple(std::string("Hi!"))),       "(\"Hi!\")"),
        std::make_tuple( new TupleToStr<std::tuple<int>>(std::make_tuple(std::make_tuple(123))), "((123))")
));


int main(int argc, char ** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
