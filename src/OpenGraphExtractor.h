#pragma once
#include <map>
#include <memory>
#include <gumbo.h>

namespace scivey {
namespace goosepp {

class OpenGraphExtractorIf {
public:
    virtual std::map<std::string, std::string> extract() = 0;
};

class OpenGraphExtractor: public OpenGraphExtractorIf {
    std::shared_ptr<GumboOutput> gumboDoc_;
public:
    OpenGraphExtractor(std::shared_ptr<GumboOutput>);
    std::map<std::string, std::string> extract();
};

} // goosepp
} // scivey
