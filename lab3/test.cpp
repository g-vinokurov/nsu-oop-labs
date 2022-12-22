#include <sstream>
#include <string>
#include <tuple>

#include "utils.hpp"
#include "tuple.hpp"
#include "csv.hpp"

#include "googletest/googletest/include/gtest/gtest.h"

TEST(TuplePrinter, EmptyTuple) {
    std::basic_stringstream<char> ss;
    std::string str;

    ss << std::make_tuple();
    
    std::getline(ss, str);
    EXPECT_TRUE(str == "()");
}


int main(int argc, char ** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
