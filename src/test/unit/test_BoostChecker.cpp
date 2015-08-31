#include "gtest/gtest.h"
#include "BoostChecker.h"

using namespace std;
using scivey::goosepp::BoostChecker;

TEST(BoostChecker, BasicTest) {
    string toSplit = "this is a test";
    int five = 5;
    EXPECT_EQ(5, five);
}
