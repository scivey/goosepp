#pragma once

#include <set>
#include <vector>

#include <gumbo.h>

namespace scivey {
namespace goosepp {

class TextNodeCollectorIf {
public:
    virtual std::vector<const GumboNode*> collect(const GumboNode *root) = 0;
};

class TextNodeCollector: public TextNodeCollectorIf {
protected:
    std::set<GumboTag> topNodeTags_ {
        GUMBO_TAG_P,
        GUMBO_TAG_PRE,
        GUMBO_TAG_TD
    };
public:
    TextNodeCollector();
    std::vector<const GumboNode*> collect(const GumboNode *root) override;
};

} // goosepp
} // scivey

