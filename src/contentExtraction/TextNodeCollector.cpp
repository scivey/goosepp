#include <set>
#include <vector>
#include <gumbo.h>
#include <goosepp/contentExtraction/TextNodeCollector.h>
#include <goosepp/util/gumboUtils.h>
#include <goosepp/util/gooseUtil.h>
#include <goosepp/stopwords/stopwords.h>

namespace scivey {
namespace goosepp {
namespace contentExtraction {

TextNodeCollector::TextNodeCollector(){}

vector<const GumboNode*> TextNodeCollector::collect(const GumboNode *root) {
    vector<const GumboNode*> nodesWithText;
    auto stopwordCounter = stopwords::getEnglishStopwordCounter();
    for (auto nodePtr: util::collectTags(topNodeTags_, root)) {
        string nodeText = util::cleanText(nodePtr);
        if (!util::isHighLinkDensity(nodePtr, nodeText) && stopwordCounter.countStopwords(nodeText) > 2) {
            nodesWithText.push_back(nodePtr);
        }
    }
    return nodesWithText;
}

} // contentExtraction
} // goosepp
} // scivey
