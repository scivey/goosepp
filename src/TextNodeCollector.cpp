#include <set>
#include <vector>
#include <gumbo.h>
#include "TextNodeCollector.h"
#include "gumboUtils.h"
#include "gooseUtil.h"
#include "stopwords/stopwords.h"

namespace scivey {
namespace goosepp {

TextNodeCollector::TextNodeCollector(){};

vector<const GumboNode*> TextNodeCollector::collect(const GumboNode *root) {
    vector<const GumboNode*> nodesWithText;
    auto stopwordCounter = stopwords::getEnglishStopwordCounter();
    for (auto nodePtr: collectTags(topNodeTags_, root)) {
        string nodeText = cleanText(nodePtr);
        if (!isHighLinkDensity(nodePtr, nodeText) && stopwordCounter.countStopwords(nodeText) > 2) {
            nodesWithText.push_back(nodePtr);
        }
    }
    return nodesWithText;
}

} // goosepp
} // scivey

