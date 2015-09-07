#include "TitleExtractor.h"

#include <string>
#include <vector>
#include <memory>
#include <set>
#include <functional>

#include <gumbo.h>

#include "OpenGraphExtractor.h"
#include "gumboUtils.h"
#include "tokenizer/Tokenizer.h"
#include "gooseUtil.h"

using namespace std;

namespace scivey {
namespace goosepp {

string sloppilyExtractDomainFromUrl(const std::string &url) {
    size_t protoIndex = url.find("://");
    if (protoIndex != string::npos) {
        string rest = url.substr(protoIndex + 3);
        size_t pathIndex = rest.find("/");
        if (pathIndex != string::npos) {
            rest = rest.substr(0, pathIndex);
        } else {
            size_t queryIndex = rest.find("?");
            if (queryIndex != string::npos) {
                rest = rest.substr(0, queryIndex);
            }
        }
        return rest;
    }
    return "";
}

TitleExtractor::TitleExtractor(shared_ptr<OpenGraphExtractor> extractor, shared_ptr<tokenizer::Tokenizer> tokenizer):
    openGraphExtractor_(extractor), tokenizer_(tokenizer) {}


string TitleExtractor::clean(const string &input, const string &url, shared_ptr<GumboOutput> doc, const map<string, string> &ogKeys) {
    string title = input;

    // TODO: get domain from url and try to match it
    if (ogKeys.find("site_name") != ogKeys.end()) {
        string ogTitle = ogKeys.at("site_name");
        size_t startIndex;
        if ( (startIndex = title.find(ogTitle)) != string::npos) {
            title = title.replace(startIndex, ogTitle.size(), "");
        }
    }

    auto domain = sloppilyExtractDomainFromUrl(url);
    if (domain != "") {
        size_t startIndex;
        if ((startIndex = title.find(domain)) != string::npos) {
            title = title.replace(startIndex, domain.size(), "");
        }
    }

    set<string> titleSplitters = {"|", "-", "»", ":"};
    auto parts = tokenizer_->tokenize(title);
    if (titleSplitters.find(parts.front()) != titleSplitters.end()) {
        parts[0] = "";
    }
    if (titleSplitters.find(parts.back()) != titleSplitters.end()) {
        parts.pop_back();
    }
    return joinVec(" ", parts);
}

string TitleExtractor::extractInitial(shared_ptr<GumboOutput> doc, const map<string, string> &ogKeys) {
    if (ogKeys.find("title") != ogKeys.end()) {
        return ogKeys.at("title");
    }
    const GumboNode *metaHeadline = findFirst(doc.get()->root, [](const GumboNode *node) {
        if (node->type == GUMBO_NODE_ELEMENT && node->v.element.tag == GUMBO_TAG_META) {
            auto name = gumbo_get_attribute(&node->v.element.attributes, "name");
            if (name != nullptr) {
                string nameVal = name->value;
                if (nameVal == "headline") {
                    return true;
                }
            }
        }
        return false;
    });
    if (metaHeadline != nullptr) {
        auto content = gumbo_get_attribute(&metaHeadline->v.element.attributes, "content");
        if (content != nullptr) {
            return string {content->value};
        }
    }
    return findTitle(doc.get()->root);
}

string TitleExtractor::extract(shared_ptr<GumboOutput> doc, string url) {
    auto ogKeys = openGraphExtractor_->extract(doc);
    auto initial = extractInitial(doc, ogKeys);
    return clean(initial, url, doc, ogKeys);
}

} // goosepp
} // scivey
