#include <tuple>
#include <vector>
#include <string>
#include <memory>

#include "PublishDateExtractor.h"
#include "util/gumboUtils.h"

using namespace std;

namespace scivey {
namespace goosepp {

vector<DateMetaTagDefinition> PublishDateExtractor::getDefaultDateTagDefs() {
    vector<DateMetaTagDefinition> knownPublishDateTags = {
        DateMetaTagDefinition("property", "rnews:datePublished", "content"),
        DateMetaTagDefinition("property", "article:published_time", "content"),
        DateMetaTagDefinition("name", "OriginalPublicationDate", "content"),
        DateMetaTagDefinition("itemprop", "datePublished", "datetime")
    };
    return std::move(knownPublishDateTags);
}

PublishDateExtractor::PublishDateExtractor(vector<DateMetaTagDefinition> tags)
    : tagDefinitions_(tags) {}

PublishDateExtractor::PublishDateExtractor()
    : tagDefinitions_(PublishDateExtractor::getDefaultDateTagDefs()){}

string PublishDateExtractor::extract(shared_ptr<const GumboNode> doc) {
    auto metaTags = util::collectTag(GUMBO_TAG_META, doc.get());
    for (auto meta: metaTags) {
        for (auto &known: tagDefinitions_) {
            auto attr = gumbo_get_attribute(&meta->v.element.attributes, known.nameKey.c_str());
            if (attr != nullptr) {
                string value = attr->value;
                if (value == known.nameVal) {
                    auto contentAttr = gumbo_get_attribute(&meta->v.element.attributes, known.contentKey.c_str());
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