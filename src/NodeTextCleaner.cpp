#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include <gumbo.h>

#include "NodeTextCleaner.h"

using namespace std;

namespace scivey {
namespace goosepp {

    namespace detail {

        regex getBadClassesRegex() {
            const vector<string> badClasses = {
                "^side$|combx|retweet|mediaarticlerelated|menucontainer|",
                "navbar|storytopbar-bucket|utility-bar|inline-share-tools",
                "|comment|PopularQuestions|contact|foot|footer|Footer|footnote",
                "|cnn_strycaptiontxt|cnn_html_slideshow|cnn_strylftcntnt",
                "|^links$|meta$|shoutbox|sponsor",
                "|tags|socialnetworking|socialNetworking|cnnStryHghLght",
                "|cnn_stryspcvbx|^inset$|pagetools|post-attributes",
                "|welcome_form|contentTools2|the_answers",
                "|communitypromo|runaroundLeft|subscribe|vcard|articleheadings",
                "|date|^print$|popup|author-dropdown|tools|socialtools|byline",
                "|konafilter|KonaFilter|breadcrumbs|^fn$|wp-caption-text",
                "|legende|ajoutVideo|timestamp|js_replies"
            };
            ostringstream oss;
            for (auto &elem: badClasses) {
                oss << elem;
            }
            string reggie = oss.str();
            regex bad_re(reggie);
            return bad_re;
        }

    } // detail

bool NodeTextCleaner::isBadTextNode(const GumboNode *node) {
    if (node->type == GUMBO_NODE_ELEMENT) {
        if (badTagTypes_.find(node->v.element.tag) != badTagTypes_.end()) {
            return true;
        }
        auto idAttr = gumbo_get_attribute(&node->v.element.attributes, "id");
        if (idAttr != nullptr) {
            string ids = idAttr->value;
            if (regex_search(ids, badClasses_)) {
                return true;
            }
        }
        auto classesAttr = gumbo_get_attribute(&node->v.element.attributes, "class");
        if (classesAttr != nullptr) {
            string classes = classesAttr->value;
            if (regex_search(classes, badClasses_)) {
                return true;
            }
        }
    }
    return false;
}

NodeTextCleaner::NodeTextCleaner(){
    badClasses_ = detail::getBadClassesRegex();
}

string NodeTextCleaner::getText(const GumboNode *node) {
    string result {""};
    if (node->type == GUMBO_NODE_TEXT) {
        result = node->v.text.text;
    } else if (node->type == GUMBO_NODE_ELEMENT) {
        if (!isBadTextNode(node)) {
            ostringstream output;
            const GumboVector *children = &node->v.element.children;
            for (size_t i = 0; i < children->length; ++i) {
                const string text = getText(static_cast<GumboNode*>(children->data[i]));
                if (i != 0 && !text.empty()) {
                    output << " ";
                }
                output << text;
            }
            result = output.str();
        }
    }
    return result;
}

} // goosepp
} // scivey
