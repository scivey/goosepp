#pragma once

#include <map>
#include <memory>
#include <set>
#include <vector>

#include <gumbo.h>

#include <goosepp/contentExtraction/TextNodeCollector.h>
#include <goosepp/contentExtraction/BoostChecker.h>
#include <goosepp/stopwords/StopwordCounter.h>

namespace scivey {
namespace goosepp {
namespace contentExtraction {

class NodeScorerIf {
public:
    virtual int getNodeScore(const GumboNode *node) = 0;
    virtual void process() = 0;
    virtual const GumboNode* getTopNode() = 0;
    virtual int getTopNodeScore() = 0;
};

class NodeScorer: public NodeScorerIf {
protected:
    const GumboNode *root_;
    std::shared_ptr<TextNodeCollectorIf> collector_;
    std::shared_ptr<BoostCheckerFactoryIf> checkerFactory_;
    std::shared_ptr<stopwords::StopwordCounterIf> stopwordCounter_;

    std::map<const GumboNode*, int> nodeScores_;
    std::map<const GumboNode*, size_t> nodeCounts_;
    std::set<const GumboNode*> parentNodes_;
    int topNodeScore_ {0};
    const GumboNode *topNode_ = nullptr;

    void updateNodeScore(const GumboNode *node, int score);
    void incrNodeCount(const GumboNode *node);
    void updateTextyNode(const GumboNode *node, int boostScore);
    bool shouldBoost(const GumboNode *node);
public:
    NodeScorer(std::shared_ptr<StopwordCounterIf>,
        std::shared_ptr<TextNodeCollectorIf>,
        std::shared_ptr<BoostCheckerFactoryIf>,
        const GumboNode *root);

    int getNodeScore(const GumboNode *node) override;
    void process() override;
    const GumboNode* getTopNode() override;
    int getTopNodeScore() override;
};

} // contentExtraction
} // goosepp
} // scivey
