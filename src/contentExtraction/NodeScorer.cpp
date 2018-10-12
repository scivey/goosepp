#include <map>
#include <set>
#include <vector>
#include <cmath>

#include <gumbo.h>

#include <goosepp/util/gumboUtils.h>
#include <goosepp/contentExtraction/TextNodeCollector.h>
#include <goosepp/contentExtraction/NodeTextCleaner.h>
#include <goosepp/stopwords/stopwords.h>
#include <goosepp/stopwords/StopwordCounter.h>
#include <goosepp/contentExtraction/NodeScorer.h>
#include <goosepp/contentExtraction/BoostChecker.h>

namespace scivey {
namespace goosepp {
namespace contentExtraction {

using stopwords::StopwordCounter;

NodeScorer::NodeScorer(shared_ptr<StopwordCounterIf> counter,
                        shared_ptr<TextNodeCollectorIf> collector,
                        shared_ptr<BoostCheckerFactoryIf> checkerFactory,
                        const GumboNode *root)
    : stopwordCounter_(counter),
      collector_(collector),
      checkerFactory_(checkerFactory),
      root_(root) {

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

bool NodeScorer::shouldBoost(const GumboNode *node) {
    auto checker = checkerFactory_->build();
    return checker.shouldBoost(node);
}



// Most of the numbers here have no explanation that I'm aware of.
// They're just black magic constants copied from both the Scala
// and Python Goose projects. (They appear to use the same constants.)

void NodeScorer::process() {
    if (topNode_ != nullptr) {
        return;
    }

    // StopwordCounter counter = stopwords::getEnglishStopwordCounter();

    auto nodesWithText = collector_->collect(root_);
    double bottomNodesForNegativeScore = 0.25 * nodesWithText.size();
    double startingBoost = 1.0;
    size_t i = 0;

    for (auto node: nodesWithText) {
        double boostScore = 0;
        if (shouldBoost(node)) {
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
        auto nodeText = util::cleanText(node);
        int upscore = stopwordCounter_->countStopwords(nodeText) + ((int) boostScore);
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

} // contentExtraction
} // goosepp
} // scivey
