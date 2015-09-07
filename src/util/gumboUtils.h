#pragma once

#include <functional>
#include <set>
#include <string>
#include <vector>
#include <cassert>
#include <gumbo.h>

namespace scivey {
namespace goosepp {
namespace util {

std::string findTitle(const GumboNode *root);
std::vector<std::string> findLinks(const GumboNode *root);
std::string cleanText(const GumboNode *root);
const GumboNode* findBody(const GumboNode *root);
const GumboNode* findFirst(const GumboNode *root, std::function<bool (const GumboNode*)>);
std::vector<const GumboNode*> collectTags(const std::set<GumboTag> &tags, const GumboNode *root);
std::vector<const GumboNode*> collectTag(GumboTag tag, const GumboNode *root);
void walkSiblings(const GumboNode*, std::function<void (const GumboNode*, std::function<void()>)>);

} // util
} // goosepp
} // scivey