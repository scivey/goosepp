#include "gtest/gtest.h"
#include "tokenizer/WhitespaceTokenizer.h"
#include <vector>
#include <string>
using namespace std;
using scivey::goosepp::tokenizer::WhitespaceTokenizer;

TEST(WhitespaceTokenizer, tokenize) {
    string toSplit = "this is a test";
    WhitespaceTokenizer tokenizer;
    auto tokens = tokenizer.tokenize(toSplit);
    EXPECT_EQ(4, tokens.size());
    EXPECT_EQ("this", tokens.at(0));
    EXPECT_EQ("is", tokens.at(1));
    EXPECT_EQ("a", tokens.at(2));
    EXPECT_EQ("test", tokens.at(3));
}

TEST(WhitespaceTokenizer, tokenizeWithPunctuation) {
    string toSplit = "this is, a test";
    WhitespaceTokenizer tokenizer;
    auto tokens = tokenizer.tokenize(toSplit);
    EXPECT_EQ(5, tokens.size());
    EXPECT_EQ("this", tokens.at(0));
    EXPECT_EQ("is", tokens.at(1));
    EXPECT_EQ(",", tokens.at(2));
    EXPECT_EQ("a", tokens.at(3));
    EXPECT_EQ("test", tokens.at(4));
}

TEST(WhitespaceTokenizer, getTokenCountSimple) {
    string toSplit = "this is a test";
    WhitespaceTokenizer tokenizer;
    EXPECT_EQ(4, tokenizer.getTokenCount(toSplit));
}

TEST(WhitespaceTokenizer, getTokenCountWithPunctuation1) {
    string toSplit = "this is, a test";
    WhitespaceTokenizer tokenizer;
    EXPECT_EQ(5, tokenizer.getTokenCount(toSplit));
}

TEST(WhitespaceTokenizer, getTokenCountWithPunctuation2) {
    string toSplit = "this is yet another test, one more, again";
    WhitespaceTokenizer tokenizer;
    EXPECT_EQ(10, tokenizer.getTokenCount(toSplit));
}
