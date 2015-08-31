#include <cassert>
#include <functional>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include <gumbo.h>
#include <glog/logging.h>

#include "gumboUtils.h"

using namespace std;

namespace scivey {
namespace goosepp {


void visitDepthFirstWithEscapeAndFilter(
        const GumboNode *node,
        function<void (const GumboNode*, function<void ()>)> callback,
        const bool &keepGoing,
        function<void()> escapeFunc,
        function<bool (const GumboNode*)> shouldRecurse) {

    callback(node, escapeFunc);
    if (keepGoing && node->type == GUMBO_NODE_ELEMENT && shouldRecurse(node)) {
        const GumboVector *children = &node->v.element.children;
        for (size_t i = 0; i < children->length; ++i) {
            auto child = static_cast<const GumboNode*>(children->data[i]);
            visitDepthFirstWithEscapeAndFilter(
                child,callback, keepGoing,
                escapeFunc, shouldRecurse
            );
        }
    }
}


// `callback` gets the current GumboNode pointer and a `void()` function,
// which it can call to halt recursion.
//
// `shouldRecurse` is a hook allowing control over which elements
// are recursed into.  This is useful e.g. to avoid getting text
// from <script> or <style> tags, as their parent node isn't necessarily obvious
// when they're currently being visited. (Better to make sure they aren't visited at all.)
void visitDepthFirstWithEscapeAndFilter(
        const GumboNode *node,
        function<void (const GumboNode*, function<void()>)> callback,
        function<bool (const GumboNode*)> shouldRecurse) {

    bool keepGoing = true;
    auto escapeFunc = [&keepGoing](){
        keepGoing = false;
    };
    visitDepthFirstWithEscapeAndFilter(node, callback, keepGoing, escapeFunc, shouldRecurse);
}

bool alwaysVisit(const GumboNode *node) {
    return true;
}

void visitDepthFirst(const GumboNode *node, function<void (const GumboNode*)> callback) {
    auto escapedCallback = [callback](const GumboNode* node, function<void()> escape) {
        ((void) escape); // ignore unused parameter
        callback(node);
    };
    visitDepthFirstWithEscapeAndFilter(node, escapedCallback, alwaysVisit);
}

void visitDepthFirstWithEscape(const GumboNode *node, function<void (const GumboNode*, function<void ()>)> callback) {
    visitDepthFirstWithEscapeAndFilter(node, callback, alwaysVisit);
}

const GumboNode* findFirstTag(GumboTag tag, const GumboNode *root) {
    const GumboNode* result = nullptr;
    auto visitor = [&result, tag](const GumboNode *node, function<void()> escape) {
        if (node->type == GUMBO_NODE_ELEMENT && node->v.element.tag == tag) {
            result = node;
            escape();
        }
    };
    visitDepthFirstWithEscape(root, visitor);
    if (result == nullptr) {
        LOG(INFO) << "findFirstTag returned nullptr!";
    }
    return result;
}

string findTitle(const GumboNode *root) {
    const GumboNode *titleNode = findFirstTag(GUMBO_TAG_TITLE, root);
    string result;
    if (titleNode == nullptr) {
        result = "<NO TITLE TAG FOUND>";
    } else if (titleNode->v.element.children.length != 1) {
        result = "<EMPTY TITLE>";
    } else {
        auto titleText = static_cast<const GumboNode*>(titleNode->v.element.children.data[0]);
        assert(titleText->type == GUMBO_NODE_TEXT || titleText->type == GUMBO_NODE_WHITESPACE);
        result = titleText->v.text.text;
    }
    return result;
}

string cleanText(const GumboNode *node) {
    if (node->type == GUMBO_NODE_TEXT) {
        return string {node->v.text.text};
    }
    if (node->type == GUMBO_NODE_ELEMENT
            && node->v.element.tag != GUMBO_TAG_SCRIPT
            && node->v.element.tag != GUMBO_TAG_STYLE) {
        string contents = "";
        const GumboVector *children = &node->v.element.children;
        for (size_t i = 0; i < children->length; ++i) {
            const string text = cleanText(static_cast<GumboNode*>(children->data[i]));
            if (i != 0 && !text.empty()) {
                contents.append(" ");
            }
            contents.append(text);
        }
        return contents;
    }
    return "";
}

const GumboNode* findBody(const GumboNode *node) {
    return findFirstTag(GUMBO_TAG_BODY, node);
}


vector<string> findLinks(const GumboNode *node) {
    auto links = collectTag(GUMBO_TAG_A, node);
    vector<string> linkHrefs;
    for (auto &elem: links) {
        GumboAttribute *href = nullptr;
        href = gumbo_get_attribute(&elem->v.element.attributes, "href");
        if (href != nullptr) {
            linkHrefs.push_back(std::move(string {href->value}));
        }
    }
    return linkHrefs;
}



vector<const GumboNode*> collectTags(const set<GumboTag> &tags, const GumboNode *root) {
    vector<const GumboNode*> output;
    auto visitor = [&tags, &output](const GumboNode *node) {
        if (node->type == GUMBO_NODE_ELEMENT
                && tags.find(node->v.element.tag) != tags.end()) {
            output.push_back(node);
        }
    };
    visitDepthFirst(root, visitor);
    return output;
}

vector<const GumboNode*> collectTag(GumboTag tag, const GumboNode *root) {
    set<GumboTag> tags {tag};
    return collectTags(tags, root);
}

void walkSiblings(const GumboNode *node, function<void (const GumboNode*, function<void()>)> callback) {
    if (!node->parent) {
        return;
    }
    bool keepGoing = true;
    auto escapeFunc = [&keepGoing]() {
        keepGoing = false;
    };
    const GumboVector *children = &node->parent->v.element.children;
    for (size_t i = 0; i < children->length; ++i) {
        if (!keepGoing) {
            break;
        }
        auto child = static_cast<const GumboNode*>(children->data[i]);
        if (child != node) {
            callback(child, escapeFunc);
        }
    }
}


} // goosepp
} // scivey