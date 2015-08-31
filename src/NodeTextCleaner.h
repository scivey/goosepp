#pragma once

#include <regex>
#include <set>
#include <string>
#include <vector>

#include <gumbo.h>

namespace scivey {
namespace goosepp {


namespace detail {
    std::regex getBadClassesRegex();
}

class NodeTextCleaner {
protected:
    std::regex badClasses_;
    std::set<GumboTag> badTagTypes_ {
        GUMBO_TAG_STYLE, GUMBO_TAG_SCRIPT, GUMBO_TAG_EM
    };

    bool isBadTextNode(const GumboNode *node);

public:
    NodeTextCleaner();
    std::string getText(const GumboNode *node);
};

} // goosepp
} // scivey
