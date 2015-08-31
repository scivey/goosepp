#pragma once

#include <string>
#include <gumbo.h>
#include "ContentExtractor.h"

namespace scivey {
namespace goosepp {

std::string extractBody(const std::string &htmlText) {
    auto gumboed = gumbo_parse(htmlText.c_str());
    ContentExtractor extractor(gumboed->root);
    std::string result = extractor.extract();
    // gumbo_destroy_output(gumboed);
    return result;
}

} // goosepp
} // scivey
