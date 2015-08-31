#pragma once

#include <gumbo.h>
#include <string>

#include "NodeScorer.h"
#include "NodeTextCleaner.h"
#include "stopwords/StopwordCounter.h"

namespace scivey {
namespace goosepp {

class ContentExtractor {
protected:
    NodeScorer scorer_;
    NodeTextCleaner cleaner_;
    stopwords::StopwordCounter stopwordCounter_;
    const GumboNode *root_;
public:
    ContentExtractor(const GumboNode *root);
    std::string extract();
};


} // goosepp
} // scivey
