#include "gtest/gtest.h"
#include "NodeTextCleaner.h"

using namespace std;
using scivey::goosepp::NodeTextCleaner;

TEST(NodeTextCleaner, BasicTest) {
    string toSplit = "this is a test";
    int five = 5;
    EXPECT_EQ(5, five);
}
