#include <map>
#include <string>
#include <memory>
#include <vector>
#include <gumbo.h>
#include "OpenGraphExtractor.h"
#include "gumboUtils.h"

using namespace std;

namespace scivey {
namespace goosepp {


map<string, string> OpenGraphExtractor::extract(shared_ptr<GumboOutput> doc) {
    map<string, string> results;
    auto metaTags = collectTag(GUMBO_TAG_META, doc.get()->root);
    for (auto &elem: metaTags) {
        auto metaName = gumbo_get_attribute(&elem->v.element.attributes, "property");
        if (metaName != nullptr) {
            string metaNameVal = metaName->value;
            if (metaNameVal.find("og:") == 0) {
                auto metaContent = gumbo_get_attribute(&elem->v.element.attributes, "content");
                if (metaContent != nullptr) {
                    string metaContentVal = metaContent->value;
                    results[metaNameVal.substr(3)] = metaContentVal;
                }
            }
        }
    }
    return results;
}

} // goosepp
} // scivey