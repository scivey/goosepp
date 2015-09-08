#pragma once
#include <gumbo.h>
#include <string>
#include <memory>
#include <goosepp/util/gooseUtil.h>

namespace scivey {
namespace goosepp {
namespace util {

class LazyGumboParser {
protected:
    GumboOutput *gumboOutput_;
    const std::string *html_;
public:
    LazyGumboParser(const std::string *html): html_(html) {
        gumboOutput_ = nullptr;
    }
    GumboOutput* get() {
        if (gumboOutput_ == nullptr) {
            gumboOutput_ = gumbo_parse(html_->c_str());
        }
        return gumboOutput_;
    }
    std::shared_ptr<GumboOutput> getShared() {
        std::shared_ptr<GumboOutput> output(
            get(),
            NonDeleter<GumboOutput>()
        );
        return output;
    }
    std::shared_ptr<GumboNode> getSharedRoot() {
        std::shared_ptr<GumboNode> output(
            get()->root,
            NonDeleter<GumboNode>()
        );
        return output;
    }
    ~LazyGumboParser() {
        if (gumboOutput_ != nullptr) {
            gumbo_destroy_output(&kGumboDefaultOptions, gumboOutput_);
        }
    }
};

}
}
}