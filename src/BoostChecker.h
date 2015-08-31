#pragma once
#include <map>
#include <set>
#include <vector>
#include <memory>
#include <gumbo.h>
#include <glog/logging.h>

#include "gumboUtils.h"
#include "TextNodeCollector.h"
#include "NodeTextCleaner.h"
#include "stopwords/stopwords.h"
#include "stopwords/StopwordCounter.h"

namespace scivey {
namespace goosepp {

using stopwords::StopwordCounter;

class BoostChecker {
    std::shared_ptr<NodeTextCleaner> cleaner_;
    std::shared_ptr<StopwordCounter> stopwordCounter_;
    size_t minStopwords_;
    size_t maxStepsAway_;
public:
    BoostChecker(std::shared_ptr<NodeTextCleaner> cleaner,
        std::shared_ptr<StopwordCounter> counter,
        size_t minStopwords = 5,
        size_t maxStepsAway = 3);
    bool isOkToBoost(const GumboNode *node);
};

class BoostCheckerFactory {
    std::shared_ptr<NodeTextCleaner> cleaner_;
    std::shared_ptr<StopwordCounter> stopwordCounter_;
    size_t minStopwords_;
    size_t maxStepsAway_;
public:
    BoostCheckerFactory(std::shared_ptr<NodeTextCleaner> cleaner,
        std::shared_ptr<StopwordCounter> counter,
        size_t minStopwords = 5,
        size_t maxStepsAway = 3);
    BoostChecker build();
};

} // goosepp
} // scivey

