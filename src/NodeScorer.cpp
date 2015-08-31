#include <map>
#include <set>
#include <vector>

#include <gumbo.h>
#include <glog/logging.h>

#include "gumboUtils.h"
#include "TextNodeCollector.h"
#include "NodeTextCleaner.h"
#include "stopwords/stopwords.h"
#include "stopwords/StopwordCounter.h"

#include "NodeScorer.h"


namespace scivey {
namespace goosepp {

using stopwords::StopwordCounter;

class BoostChecker {
    NodeTextCleaner cleaner_;
    StopwordCounter stopwordCounter_;
    size_t minStopwords_;
    size_t maxStepsAway_;
public:
    BoostChecker(NodeTextCleaner cleaner, StopwordCounter counter, size_t minStopwords = 5, size_t maxStepsAway = 3)
        : stopwordCounter_(counter),
          cleaner_(cleaner),
          minStopwords_(minStopwords),
          maxStepsAway_(maxStepsAway) {}

    bool isOkToBoost(const GumboNode *node) {
        bool isOk = false;
        size_t stepsAway = 0;
        size_t minimumStopwordCount = 5;
        size_t maxStepsAway = 3;

        auto visitor = [&isOk, &stepsAway, this](const GumboNode* sibling, function<void()> escape) {
            if (sibling->type == GUMBO_NODE_ELEMENT && sibling->v.element.tag == GUMBO_TAG_P) {
                if (stepsAway >= this->maxStepsAway_) {
                    VLOG(4) << "next paragraph is too far away; boosting";
                    isOk = false;
                    escape();
                    return;
                }
                auto siblingText = this->cleaner_.getText(sibling);
                if (this->stopwordCounter_.countStopwords(siblingText) > this->minStopwords_) {
                    VLOG(4) << "sibling has a lot of stopwords; boosting";
                    isOk = true;
                    escape();
                    return;
                }
                stepsAway++;
            }
        };
        walkSiblings(node, visitor);
        return isOk;
    }
};

NodeScorer::NodeScorer(const GumboNode *root): root_(root){
    process();
}

int NodeScorer::getNodeScore(const GumboNode *node) {
    if (nodeScores_.find(node) == nodeScores_.end()) {
        return 0;
    }
    return nodeScores_[node];
}

void NodeScorer::updateNodeScore(const GumboNode *node, int score) {
    if (nodeScores_.find(node) == nodeScores_.end()) {
        nodeScores_[node] = 0;
    }
    nodeScores_[node] += score;
}

void NodeScorer::incrNodeCount(const GumboNode *node) {
    if (nodeCounts_.find(node) == nodeCounts_.end()) {
        nodeCounts_[node] = 0;
    }
    nodeCounts_[node]++;
}

void NodeScorer::updateTextyNode(const GumboNode *node, int boostScore) {
    if (node->parent != nullptr) {
        updateNodeScore(node->parent, boostScore);
        incrNodeCount(node->parent);
        parentNodes_.insert(node->parent);
    }
    if (node->parent->parent != nullptr) {
        updateNodeScore(node->parent->parent, boostScore / 2);
        incrNodeCount(node->parent->parent);
        parentNodes_.insert(node->parent->parent);
    }
}

const GumboNode* NodeScorer::getTopNode() {
    return topNode_;
}

int NodeScorer::getTopNodeScore() {
    return topNodeScore_;
}

bool NodeScorer::isOkToBoost(const GumboNode *node) {
    StopwordCounter counter = stopwords::getEnglishStopwordCounter();
    NodeTextCleaner cleaner;
    BoostChecker checker(cleaner, counter);
    return checker.isOkToBoost(node);
}



// Most of the numbers here have no explanation that I'm aware of.
// They're just black magic constants copied from both the Scala
// and Python Goose projects. (They appear to use the same constants.)

void NodeScorer::process() {
    if (topNode_ != nullptr) {
        return;
    }

    StopwordCounter counter = stopwords::getEnglishStopwordCounter();

    auto nodesWithText = collector_.collect(root_);
    double bottomNodesForNegativeScore = 0.25 * nodesWithText.size();
    double startingBoost = 1.0;
    size_t i = 0;

    for (auto node: nodesWithText) {
        double boostScore = 0;
        if (isOkToBoost(node)) {
            boostScore = ((1.0 / startingBoost) * 50);
            startingBoost += 1.0;
        }
        if (nodesWithText.size() > 15) {
            if ((nodesWithText.size() - i) <= bottomNodesForNegativeScore) {
                double booster = bottomNodesForNegativeScore - ((double) nodesWithText.size() - i);
                boostScore = -1.0 * pow(booster, 2.0);
                double negScore = abs(boostScore);
                if (negScore > 40) {
                    boostScore = 5.0;
                }
            }
        }
        VLOG(4) << "boostScore [" << i << "] : " << boostScore;
        auto nodeText = cleanText(node);
        int upscore = counter.countStopwords(nodeText) + ((int) boostScore);
        updateTextyNode(node, upscore);
        i++;
    }

    int topNodeScore = 0;
    const GumboNode *topNode = nullptr;
    for (auto node: parentNodes_) {
        int score = nodeScores_[node];
        if (score > topNodeScore) {
            topNode = node;
            topNodeScore = score;
        } else if (topNode == nullptr) {
            topNode = node;
        }
    }
    topNode_ = topNode;
    topNodeScore_ = topNodeScore;
}

} // goosepp
} // scivey