#pragma once
#include <map>
#include <set>
#include <vector>
#include <memory>
#include <gumbo.h>
#include <glog/logging.h>

#include "util/gumboUtils.h"
#include "TextNodeCollector.h"
#include "NodeTextCleaner.h"
#include "stopwords/stopwords.h"
#include "stopwords/StopwordCounter.h"

namespace scivey {
namespace goosepp {
namespace contentExtraction {

using stopwords::StopwordCounterIf;

class BoostCheckerIf {
public:
    virtual bool shouldBoost(const GumboNode *node) = 0;
};

class BoostChecker: public BoostCheckerIf {
    std::shared_ptr<NodeTextCleanerIf> cleaner_;
    std::shared_ptr<StopwordCounterIf> stopwordCounter_;
    size_t minStopwords_;
    size_t maxStepsAway_;
public:
    BoostChecker(std::shared_ptr<NodeTextCleanerIf> cleaner,
        std::shared_ptr<StopwordCounterIf> counter,
        size_t minStopwords = 5,
        size_t maxStepsAway = 3);
    bool shouldBoost(const GumboNode *node) override;
};

class BoostCheckerFactoryIf {
public:
    virtual BoostChecker build() = 0;
};

class BoostCheckerFactory: public BoostCheckerFactoryIf {
    std::shared_ptr<NodeTextCleanerIf> cleaner_;
    std::shared_ptr<StopwordCounterIf> stopwordCounter_;
    size_t minStopwords_;
    size_t maxStepsAway_;
public:
    BoostCheckerFactory(std::shared_ptr<NodeTextCleanerIf> cleaner,
        std::shared_ptr<StopwordCounterIf> counter,
        size_t minStopwords = 5,
        size_t maxStepsAway = 3);
    BoostChecker build() override;
};

} // contentExtraction
} // goosepp
} // scivey

