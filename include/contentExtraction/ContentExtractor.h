#pragma once

#include <gumbo.h>
#include <string>
#include <memory>

#include "NodeScorer.h"
#include "NodeTextCleaner.h"
#include "stopwords/StopwordCounter.h"

namespace scivey {
namespace goosepp {
namespace contentExtraction {

class ContentExtractorIf {
public:
    virtual std::string extract() = 0;
};

class ContentExtractor: public ContentExtractorIf {
protected:
    std::shared_ptr<NodeScorerIf> scorer_;
    std::shared_ptr<NodeTextCleanerIf> cleaner_;
    std::shared_ptr<stopwords::StopwordCounterIf> stopwordCounter_;
    const GumboNode *root_;
public:
    ContentExtractor(std::shared_ptr<NodeScorerIf>,
        std::shared_ptr<NodeTextCleanerIf>,
        std::shared_ptr<stopwords::StopwordCounterIf>,
        const GumboNode *root);
    std::string extract() override;
};


} // contentExtraction
} // goosepp
} // scivey
