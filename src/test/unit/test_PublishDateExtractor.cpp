#include <memory>
#include <string>
#include <vector>
#include <map>
#include <gumbo.h>
#include <glog/logging.h>
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "testHelpers.h"

#include "PublishDateExtractor.h"

using namespace std;
using scivey::goosepp::PublishDateExtractor;
using ::testing::Return;
using ::testing::_;

TEST(PublishDateExtractor, noMetaTags) {
    string doc = {
        "<html><head></head><body></body></html>"
    };
    PublishDateExtractor extractor;
    auto gumboed = gumbo_parse(doc.c_str());
    shared_ptr<const GumboNode> docPtr(gumboed->root, NonDeleter<const GumboNode>());
    EXPECT_EQ("", extractor.extract(docPtr));
}

TEST(PublishDateExtractor, badMetaTag) {
    string doc = {
        "<html>"
        "<head>"
        "<meta property='x' content='y'/>"
        "</head>"
        "<body></body>"
        "</html>"
    };
    PublishDateExtractor extractor;
    auto gumboed = gumbo_parse(doc.c_str());
    shared_ptr<const GumboNode> docPtr(gumboed->root, NonDeleter<const GumboNode>());
    EXPECT_EQ("", extractor.extract(docPtr));
}

TEST(PublishDateExtractor, rnewsTag) {
    string doc = {
        "<html>"
        "<head>"
        "<meta property='rnews:datePublished' content='DATE'/>"
        "</head>"
        "<body></body>"
        "</html>"
    };
    PublishDateExtractor extractor;
    auto gumboed = gumbo_parse(doc.c_str());
    shared_ptr<const GumboNode> docPtr(gumboed->root, NonDeleter<const GumboNode>());
    EXPECT_EQ("DATE", extractor.extract(docPtr));
}

TEST(PublishDateExtractor, pubTimeTag) {
    string doc = {
        "<html>"
        "<head>"
        "<meta property='article:published_time' content='DATE'/>"
        "</head>"
        "<body></body>"
        "</html>"
    };
    PublishDateExtractor extractor;
    auto gumboed = gumbo_parse(doc.c_str());
    shared_ptr<const GumboNode> docPtr(gumboed->root, NonDeleter<const GumboNode>());
    EXPECT_EQ("DATE", extractor.extract(docPtr));
}

TEST(PublishDateExtractor, OriginalPublicationDate) {
    string doc = {
        "<html>"
        "<head>"
        "<meta name='OriginalPublicationDate' content='DATE'/>"
        "</head>"
        "<body></body>"
        "</html>"
    };
    PublishDateExtractor extractor;
    auto gumboed = gumbo_parse(doc.c_str());
    shared_ptr<const GumboNode> docPtr(gumboed->root, NonDeleter<const GumboNode>());
    EXPECT_EQ("DATE", extractor.extract(docPtr));
}

TEST(PublishDateExtractor, datePublished) {
    string doc = {
        "<html>"
        "<head>"
        "<meta itemprop='datePublished' datetime='DATE'/>"
        "</head>"
        "<body></body>"
        "</html>"
    };
    PublishDateExtractor extractor;
    auto gumboed = gumbo_parse(doc.c_str());
    shared_ptr<const GumboNode> docPtr(gumboed->root, NonDeleter<const GumboNode>());
    EXPECT_EQ("DATE", extractor.extract(docPtr));
}
