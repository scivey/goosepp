#pragma once
#include <memory>
#include <map>
#include <string>

#include <gumbo.h>
namespace scivey {
namespace goosepp {

struct DateMetaTagDefinition {
    const std::string nameKey, nameVal, contentKey;
    DateMetaTagDefinition(const std::string &nameK, const std::string &nameV, const std::string &contentK)
        : nameKey(nameK), nameVal(nameV), contentKey(contentK) {}
};

class PublishDateExtractorIf {
public:
    virtual std::string extract(std::shared_ptr<const GumboNode>) = 0;
};

class PublishDateExtractor: public PublishDateExtractorIf {
protected:
    std::vector<DateMetaTagDefinition> tagDefinitions_;
public:
    static std::vector<DateMetaTagDefinition> getDefaultDateTagDefs();
    PublishDateExtractor();
    PublishDateExtractor(std::vector<DateMetaTagDefinition>);
    std::string extract(std::shared_ptr<const GumboNode>) override;
};

}
}