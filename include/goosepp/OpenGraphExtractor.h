#pragma once
#include <map>
#include <memory>
#include <gumbo.h>

namespace scivey {
namespace goosepp {

class OpenGraphExtractorIf {
public:
    virtual std::map<std::string, std::string> extract(std::shared_ptr<GumboOutput>) = 0;
};

class OpenGraphExtractor: public OpenGraphExtractorIf {
public:
    std::map<std::string, std::string> extract(std::shared_ptr<GumboOutput> gumboDoc_);
};

} // goosepp
} // scivey
