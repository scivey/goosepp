#include <map>
#include <set>
#include <vector>
#include <memory>

#include <gumbo.h>

#include <goosepp/contentExtraction/BoostChecker.h>
#include <goosepp/contentExtraction/NodeTextCleaner.h>
#include <goosepp/contentExtraction/TextNodeCollector.h>
#include <goosepp/util/gumboUtils.h>
#include <goosepp/stopwords/stopwords.h>
#include <goosepp/stopwords/StopwordCounter.h>

namespace scivey {
namespace goosepp {
namespace contentExtraction {

using stopwords::StopwordCounter;

BoostChecker::BoostChecker(shared_ptr<NodeTextCleanerIf> cleaner,
                            shared_ptr<StopwordCounterIf> counter,
                            size_t minStopwords,
                            size_t maxStepsAway)
    : stopwordCounter_(counter),
      cleaner_(cleaner),
      minStopwords_(minStopwords),
      maxStepsAway_(maxStepsAway) {}

bool BoostChecker::shouldBoost(const GumboNode *node) {
    bool isOk = false;
    size_t stepsAway = 0;

    auto visitor = [&isOk, &stepsAway, this](const GumboNode* sibling, function<void()> escape) {
        if (sibling->type == GUMBO_NODE_ELEMENT && sibling->v.element.tag == GUMBO_TAG_P) {
            if (stepsAway >= this->maxStepsAway_) {
                isOk = false;
                escape();
                return;
            }
            auto siblingText = this->cleaner_->getText(sibling);
            if (this->stopwordCounter_->countStopwords(siblingText) > this->minStopwords_) {
                isOk = true;
                escape();
                return;
            }
            stepsAway++;
        }
    };
    util::walkSiblings(node, visitor);
    return isOk;
}


BoostCheckerFactory::BoostCheckerFactory(shared_ptr<NodeTextCleanerIf> cleaner,
                            shared_ptr<StopwordCounterIf> counter,
                            size_t minStopwords,
                            size_t maxStepsAway)
    : stopwordCounter_(counter),
      cleaner_(cleaner),
      minStopwords_(minStopwords),
      maxStepsAway_(maxStepsAway) {}

BoostChecker BoostCheckerFactory::build() {
    BoostChecker checker(cleaner_, stopwordCounter_, minStopwords_, maxStepsAway_);
    return checker;
}

} // contentExraction
} // goosepp
} // scivey

