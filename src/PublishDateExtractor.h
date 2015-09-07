#pragma once
#include <memory>
#include <gumbo.h>
namespace scivey {
namespace goosepp {

class PublishDateExtractorIf {
public:
    virtual std::string extract(std::shared_ptr<const GumboNode>) = 0;
};

class PublishDateExtractor: public PublishDateExtractorIf {
public:
    std::string extract(std::shared_ptr<const GumboNode>) override;
};

}
}