#pragma once

#include <map>
#include <set>
#include <vector>

#include <gumbo.h>

#include "TextNodeCollector.h"

namespace scivey {
namespace goosepp {


class NodeScorer {
protected:
    const GumboNode *root_;
    TextNodeCollector collector_;
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
    NodeScorer(const GumboNode *root);
    int getNodeScore(const GumboNode *node);
    void process();
    const GumboNode* getTopNode();
    int getTopNodeScore();
};

} // goosepp
} // scivey