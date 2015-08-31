#include "gtest/gtest.h"
#include "NodeScorer.h"

using namespace std;
using scivey::goosepp::NodeScorer;

TEST(NodeScorer, BasicTest) {
    string toSplit = "this is a test";
    int five = 5;
    EXPECT_EQ(5, five);
}
