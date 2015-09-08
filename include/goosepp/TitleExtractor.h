#pragma once
#include <string>
#include <vector>
#include <memory>
#include <goosepp/OpenGraphExtractor.h>
#include <goosepp/tokenizer/Tokenizer.h>

namespace scivey {
namespace goosepp {

std::string sloppilyExtractDomainFromUrl(const std::string&);

class TitleExtractorIf {
public:
    virtual std::string extract(std::shared_ptr<GumboOutput> doc, std::string url) = 0;
};

class TitleExtractor: public TitleExtractorIf {
protected:
    std::shared_ptr<OpenGraphExtractorIf> openGraphExtractor_;
    std::shared_ptr<tokenizer::Tokenizer> tokenizer_;
    std::string clean(const std::string&, const std::string&, std::shared_ptr<GumboOutput>, const std::map<std::string, std::string>&);
    std::string extractInitial(std::shared_ptr<GumboOutput> doc, const std::map<std::string, std::string> &ogKeys);
public:
    TitleExtractor(std::shared_ptr<OpenGraphExtractorIf>, std::shared_ptr<tokenizer::Tokenizer>);
    std::string extract(std::shared_ptr<GumboOutput> doc, std::string url) override;
};

} // goosepp
} // scivey
