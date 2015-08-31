#pragma once

#include <string>
#include <gumbo.h>

namespace scivey {
namespace goosepp {

bool isHighLinkDensity(const GumboNode *node, const std::string &nodeText);
bool isHighLinkDensity(const GumboNode *node);

} // goosepp
} // scivey