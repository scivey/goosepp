#include <memory>
#include <string>
#include <vector>
#include <map>
#include <gumbo.h>
#include <glog/logging.h>
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "testHelpers.h"

#include "TitleExtractor.h"

using namespace std;
using scivey::goosepp::sloppilyExtractDomainFromUrl;
using scivey::goosepp::TitleExtractor;
using ::testing::Return;
using ::testing::_;

TEST(TitleExtractor, sloppyDomainExtraction) {
    map<string, string> truthyExpectations = {
        {"www.jezebel.com", "http://www.jezebel.com/something"},
        {"www.jezebel.com", "https://www.jezebel.com/something"},
        {"www.jezebel.com", "http://www.jezebel.com?something"},
        {"www.jezebel.com", "https://www.jezebel.com?something"},
        {"www.jezebel.com", "http://www.jezebel.com/something?yes"},
        {"www.jezebel.com", "https://www.jezebel.com/something?yes"},
        {"www.jezebel.com", "http://www.jezebel.com"},
        {"www.jezebel.com", "https://www.jezebel.com"}
    };
    for (auto &expectation: truthyExpectations) {
        EXPECT_EQ(expectation.first, sloppilyExtractDomainFromUrl(expectation.second));
    }
}
