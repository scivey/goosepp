#include <tuple>
#include <vector>
#include <string>
#include <memory>

#include "PublishDateExtractor.h"
#include "gumboUtils.h"

using namespace std;

namespace scivey {
namespace goosepp {

string PublishDateExtractor::extract(shared_ptr<const GumboNode> doc) {
    const vector<tuple<string, string, string>> knownPublishDateTags = {
        make_tuple("property", "rnews:datePublished", "content"),
        make_tuple("property", "article:published_time", "content"),
        make_tuple("name", "OriginalPublicationDate", "content"),
        make_tuple("itemprop", "datePublished", "datetime"),
    };
    auto metaTags = collectTag(GUMBO_TAG_META, doc.get());
    for (auto meta: metaTags) {
        for (auto &known: knownPublishDateTags) {
            auto attr = gumbo_get_attribute(&meta->v.element.attributes, std::get<0>(known).c_str());
            if (attr != nullptr) {
                string value = attr->value;
                if (value == std::get<1>(known)) {
                    auto contentAttr = gumbo_get_attribute(&meta->v.element.attributes, std::get<2>(known).c_str());
                    if (contentAttr != nullptr) {
                        string contentValue = contentAttr->value;
                        if (contentValue != "") {
                            return contentValue;
                        }
                    }
                }
            }
        }
    }
    return "";
}

}
}