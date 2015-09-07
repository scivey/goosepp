#include <string>
#include <gumbo.h>
#include "gooseUtil.h"
#include "gumboUtils.h"
#include "tokenizer/WhitespaceTokenizer.h"

using namespace std;

namespace scivey {
namespace goosepp {
namespace util {

using tokenizer::WhitespaceTokenizer;

// all of this link density logic is straight from the scala version
bool isHighLinkDensity(const GumboNode *node, const string &nodeText) {
    auto allLinks = collectTag(GUMBO_TAG_A, node);
    if (allLinks.size() == 0) {
        return false;
    }
    WhitespaceTokenizer tokenizer;
    size_t linkTokenCount = 0;
    for (auto linkPtr: allLinks) {
        linkTokenCount += tokenizer.getTokenCount(cleanText(linkPtr));
    }
    double linkDivisor = ((double) linkTokenCount) / ((double) tokenizer.getTokenCount(nodeText));
    double score = linkDivisor * ((double) allLinks.size());
    if (score > 1.0) {
        return true;
    }
    return false;
}

bool isHighLinkDensity(const GumboNode *node) {
    return isHighLinkDensity(node, cleanText(node));
}


} // util
} // goosepp
} // scivey