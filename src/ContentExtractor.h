#pragma once

#include <gumbo.h>
#include <string>
#include <memory>

#include "NodeScorer.h"
#include "NodeTextCleaner.h"
#include "stopwords/StopwordCounter.h"

namespace scivey {
namespace goosepp {

class ContentExtractor {
protected:
    std::shared_ptr<NodeScorer> scorer_;
    std::shared_ptr<NodeTextCleaner> cleaner_;
    std::shared_ptr<stopwords::StopwordCounter> stopwordCounter_;
    const GumboNode *root_;
public:
    ContentExtractor(std::shared_ptr<NodeScorer>,
        std::shared_ptr<NodeTextCleaner>,
        std::shared_ptr<stopwords::StopwordCounter>,
        const GumboNode *root);
    std::string extract();
};


} // goosepp
} // scivey
