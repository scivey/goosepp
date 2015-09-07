#include <memory>
#include <string>
#include <vector>
#include <map>
#include <gumbo.h>
#include <glog/logging.h>
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "testHelpers.h"

#include "OpenGraphExtractor.h"

using namespace std;
using scivey::goosepp::OpenGraphExtractor;

TEST(OpenGraphExtractor, shouldNotBeWrong) {
    string document {
        "<html>"
        "<head>"
        "<meta property='og:something' content='something-value'/>"
        "<script src='something.js'></script>"
        "<meta property='og:otherthing' content='other-value'/>"
        "<meta property='not-og-prop' content='bad'/>"
        "<title>Page Title</title>"
        "</head>"
        "<body>"
        "<div> some text </div>"
        "</body>"
        "</html>"
    };

    auto gumboed = gumbo_parse(document.c_str());
    shared_ptr<GumboOutput> docPtr(gumboed, NonDeleter<GumboOutput>());
    OpenGraphExtractor extractor(docPtr);
    auto result = extractor.extract();
    EXPECT_EQ(2, result.size());
    EXPECT_TRUE(result.find("something") != result.end());
    EXPECT_TRUE(result.find("otherthing") != result.end());
    EXPECT_TRUE(result.find("not-og-prop") == result.end());
    EXPECT_EQ("something-value", result["something"]);
    EXPECT_EQ("other-value", result["otherthing"]);
}

TEST(OpenGraphExtractor, noOpenGraphTags) {
    string document {
        "<html>"
        "<head>"
        "<meta property='something' content='something-value'/>"
        "<script src='something.js'></script>"
        "<meta property='otherthing' content='other-value'/>"
        "<meta property='not-og-prop' content='bad'/>"
        "<title>Page Title</title>"
        "</head>"
        "<body>"
        "<div> some text </div>"
        "</body>"
        "</html>"
    };

    auto gumboed = gumbo_parse(document.c_str());
    shared_ptr<GumboOutput> docPtr(gumboed, NonDeleter<GumboOutput>());
    OpenGraphExtractor extractor(docPtr);
    auto result = extractor.extract();
    EXPECT_EQ(0, result.size());
}

TEST(OpenGraphExtractor, noMetaTagsAtAll) {
    string document {
        "<html>"
        "<head>"
        "<script src='something.js'></script>"
        "<title>Page Title</title>"
        "</head>"
        "<body>"
        "<div> some text </div>"
        "</body>"
        "</html>"
    };

    auto gumboed = gumbo_parse(document.c_str());
    shared_ptr<GumboOutput> docPtr(gumboed, NonDeleter<GumboOutput>());
    OpenGraphExtractor extractor(docPtr);
    auto result = extractor.extract();
    EXPECT_EQ(0, result.size());
}
