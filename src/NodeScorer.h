#pragma once

#include <map>
#include <memory>
#include <set>
#include <vector>

#include <gumbo.h>

#include "TextNodeCollector.h"
#include "BoostChecker.h"
#include "stopwords/StopwordCounter.h"
namespace scivey {
namespace goosepp {


class NodeScorer {
protected:
    const GumboNode *root_;
    std::shared_ptr<TextNodeCollector> collector_;
    std::shared_ptr<BoostCheckerFactory> checkerFactory_;
    std::shared_ptr<stopwords::StopwordCounter> stopwordCounter_;

    std::map<const GumboNode*, int> nodeScores_;
    std::map<const GumboNode*, size_t> nodeCounts_;
    std::set<const GumboNode*> parentNodes_;
    int topNodeScore_ {0};
    const GumboNode *topNode_ = nullptr;

    void updateNodeScore(const GumboNode *node, int score);
    void incrNodeCount(const GumboNode *node);
    void updateTextyNode(const GumboNode *node, int boostScore);
    bool isOkToBoost(const GumboNode *node);
public:
    NodeScorer(std::shared_ptr<StopwordCounter>,
        std::shared_ptr<TextNodeCollector>,
        std::shared_ptr<BoostCheckerFactory>,
        const GumboNode *root);

    int getNodeScore(const GumboNode *node);
    void process();
    const GumboNode* getTopNode();
    int getTopNodeScore();
};

} // goosepp
} // scivey