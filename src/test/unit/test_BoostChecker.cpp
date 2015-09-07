#include <memory>
#include <string>
#include <vector>

#include <gumbo.h>
#include <glog/logging.h>
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "testHelpers.h"

#include "contentExtraction/BoostChecker.h"
#include "contentExtraction/NodeTextCleaner.h"
#include "stopwords/StopwordCounter.h"

using namespace std;
using scivey::goosepp::contentExtraction::BoostChecker;
using scivey::goosepp::stopwords::StopwordCounterIf;
using scivey::goosepp::contentExtraction::NodeTextCleanerIf;
using ::testing::Return;
using ::testing::_;

class MockStopwordCounter: public StopwordCounterIf {
public:
    MOCK_METHOD1(countStopwords, size_t(const string &text));
    MOCK_METHOD1(countStopwords, size_t(const vector<string> &tokens));
};

class MockNodeTextCleaner: public NodeTextCleanerIf {
public:
    MOCK_METHOD1(getText, string(const GumboNode* node));
};

TEST(BoostChecker, shouldBoostNoGoodSiblings) {
    MockNodeTextCleaner mockCleaner;
    MockStopwordCounter mockCounter;
    shared_ptr<NodeTextCleanerIf> cleaner(&mockCleaner, NonDeleter<NodeTextCleanerIf>());
    shared_ptr<StopwordCounterIf> counter(&mockCounter, NonDeleter<StopwordCounterIf>());
    size_t minStopwords = 5;
    size_t maxDist = 4;
    BoostChecker checker(cleaner, counter, minStopwords, maxDist);

    string document {
        "<div class='outer'>"
            "<p>This is some text</p>"
            "<div class='something'>Nothing here</div>"
            "<div class='something-else'>Ehh</div>"
        "</div>"
    };
    auto gumboed = gumbo_parse(document.c_str());
    auto body = static_cast<const GumboNode*>(gumboed->root->v.element.children.data[1]);
    EXPECT_EQ(GUMBO_TAG_BODY, body->v.element.tag);
    auto para = static_cast<const GumboNode*>(
        ((const GumboNode*) body->v.element.children.data[0])->v.element.children.data[0]
    );
    EXPECT_EQ(GUMBO_TAG_P, para->v.element.tag);
    EXPECT_FALSE(checker.shouldBoost(para));
}

TEST(BoostChecker, shouldBoostMajesticSibling) {
    MockNodeTextCleaner mockCleaner;
    MockStopwordCounter mockCounter;
    shared_ptr<NodeTextCleanerIf> cleaner(&mockCleaner, NonDeleter<NodeTextCleanerIf>());
    shared_ptr<StopwordCounterIf> counter(&mockCounter, NonDeleter<StopwordCounterIf>());

    size_t minStopwords = 2;
    size_t maxDist = 4;
    BoostChecker checker(cleaner, counter, minStopwords, maxDist);

    string document {
        "<div class='outer'>"
            "<p>This is some text</p>"
            "<p>Some sibling text</p>"
            "<div class='something'>Nothing here</div>"
            "<div class='something-else'>Ehh</div>"
        "</div>"
    };
    auto gumboed = gumbo_parse(document.c_str());
    auto body = static_cast<const GumboNode*>(gumboed->root->v.element.children.data[1]);
    EXPECT_EQ(GUMBO_TAG_BODY, body->v.element.tag);
    auto outerDiv = static_cast<const GumboNode*>(body->v.element.children.data[0]);
    auto para = static_cast<const GumboNode*>(outerDiv->v.element.children.data[0]);
    auto firstSibling = static_cast<const GumboNode*>(outerDiv->v.element.children.data[1]);

    EXPECT_EQ(GUMBO_TAG_P, para->v.element.tag);
    EXPECT_EQ(GUMBO_TAG_P, firstSibling->v.element.tag);
    string expectedText = "Some sibling text";
    EXPECT_CALL(mockCounter, countStopwords(expectedText))
        .WillOnce(Return(3));
    EXPECT_CALL(mockCleaner, getText(firstSibling))
        .WillOnce(Return(expectedText));
    EXPECT_TRUE(checker.shouldBoost(para));
}

TEST(BoostChecker, shouldBoostDecentSiblingButTooFar) {
    MockNodeTextCleaner mockCleaner;
    MockStopwordCounter mockCounter;
    shared_ptr<NodeTextCleanerIf> cleaner(&mockCleaner, NonDeleter<NodeTextCleanerIf>());
    shared_ptr<StopwordCounterIf> counter(&mockCounter, NonDeleter<StopwordCounterIf>());

    size_t minStopwords = 3;
    size_t maxDist = 3;
    BoostChecker checker(cleaner, counter, minStopwords, maxDist);

    string document {
        "<div class='outer'>"
            "<p>This is some text</p>"
            "<div class='something'>Nothing here</div>"
            "<div class='something-else'>Ehh</div>"
            "<div class='something-else'>mreh</div>"
            "<div class='something-else'>yes</div>"
            "<p>Some sibling text</p>"
        "</div>"
    };
    auto gumboed = gumbo_parse(document.c_str());
    auto body = static_cast<const GumboNode*>(gumboed->root->v.element.children.data[1]);
    EXPECT_EQ(GUMBO_TAG_BODY, body->v.element.tag);
    auto outerDiv = static_cast<const GumboNode*>(body->v.element.children.data[0]);
    auto para = static_cast<const GumboNode*>(outerDiv->v.element.children.data[0]);
    auto lastSibling = static_cast<const GumboNode*>(outerDiv->v.element.children.data[5]);

    EXPECT_EQ(GUMBO_TAG_P, para->v.element.tag);
    EXPECT_EQ(GUMBO_TAG_P, lastSibling->v.element.tag);
    string expectedText = "Some sibling text";
    EXPECT_FALSE(checker.shouldBoost(para));
}
