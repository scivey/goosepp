#include <gumbo.h>
#include <string>
#include <vector>

#include <goosepp/contentExtraction/ContentExtractor.h>
#include <goosepp/stopwords/stopwords.h>
#include <goosepp/util/gooseUtil.h>

using namespace std;

namespace scivey {
namespace goosepp {
namespace contentExtraction {

ContentExtractor::ContentExtractor(std::shared_ptr<NodeScorerIf> scorer,
                                    std::shared_ptr<NodeTextCleanerIf> cleaner,
                                    std::shared_ptr<stopwords::StopwordCounterIf> counter,
                                    const GumboNode *root)
    : scorer_(scorer),
      cleaner_(cleaner),
      stopwordCounter_(counter),
      root_(root) {}


string ContentExtractor::extract() {
    auto topNode = scorer_->getTopNode();
    int topNodeScore = scorer_->getTopNodeScore();
    vector<const GumboNode*> goodNodes;
    {
        const GumboVector *children = &topNode->v.element.children;
        double thresholdScore = ((double) topNodeScore) * 0.08;
        for (size_t i = 0; i < children->length; ++i) {
            auto node = static_cast<const GumboNode*>(children->data[i]);
            if (node->type == GUMBO_NODE_ELEMENT && node->v.element.tag != GUMBO_TAG_P) {
                if (util::isHighLinkDensity(node)) {
                    continue;
                }
                if (node->v.element.tag != GUMBO_TAG_TD) {
                    double nodeScore = (double) scorer_->getNodeScore(node);
                    if (nodeScore < thresholdScore) {
                        continue;
                    }
                }
            }
            auto nodeText = cleaner_->getText(node);
            if (stopwordCounter_->countStopwords(nodeText) < 3) {
                continue;
            }
            goodNodes.push_back(node);
        }
    }
    ostringstream bodyStream;
    for (auto &elem: goodNodes) {
        bodyStream << cleaner_->getText(elem);
    }
    return bodyStream.str();
}

} // contentExtraction
} // goosepp
} // scivey
