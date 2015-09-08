#include <string>
#include <memory>
#include "util/LazyGumboParser.h"
#include "util/gooseUtil.h"

namespace scivey {
namespace goosepp {

class GooseExtractor {
protected:
    const std::string url_;
    const std::string html_;
    util::LazyGumboParser gumboParser_;
public:
    GooseExtractor(const std::string &url, const std::string &html)
        : url_(url), html_(html), gumboParser_(&html_) {}
    std::string getBody();
    std::string getTitle();
    std::string getPublishDate();
};

}
}
