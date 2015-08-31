#include <map>
#include <set>
#include <vector>
#include <memory>

#include <gumbo.h>
#include <glog/logging.h>

#include "BoostChecker.h"
#include "gumboUtils.h"
#include "TextNodeCollector.h"
#include "NodeTextCleaner.h"
#include "stopwords/stopwords.h"
#include "stopwords/StopwordCounter.h"

namespace scivey {
namespace goosepp {

using stopwords::StopwordCounter;

BoostChecker::BoostChecker(shared_ptr<NodeTextCleaner> cleaner,
                            shared_ptr<StopwordCounter> counter,
                            size_t minStopwords,
                            size_t maxStepsAway)
    : stopwordCounter_(counter),
      cleaner_(cleaner),
      minStopwords_(minStopwords),
      maxStepsAway_(maxStepsAway) {}

bool BoostChecker::isOkToBoost(const GumboNode *node) {
    bool isOk = false;
    size_t stepsAway = 0;

    auto visitor = [&isOk, &stepsAway, this](const GumboNode* sibling, function<void()> escape) {
        if (sibling->type == GUMBO_NODE_ELEMENT && sibling->v.element.tag == GUMBO_TAG_P) {
            if (stepsAway >= this->maxStepsAway_) {
                VLOG(4) << "next paragraph is too far away; boosting";
                isOk = false;
                escape();
                return;
            }
            auto siblingText = this->cleaner_->getText(sibling);
            if (this->stopwordCounter_->countStopwords(siblingText) > this->minStopwords_) {
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


BoostCheckerFactory::BoostCheckerFactory(shared_ptr<NodeTextCleaner> cleaner,
                            shared_ptr<StopwordCounter> counter,
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

} // goosepp
} // scivey

