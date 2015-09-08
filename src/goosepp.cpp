#include <string>
#include <memory>
#include <gumbo.h>
#include <goosepp/contentExtraction/ContentExtractor.h>
#include <goosepp/stopwords/stopwords.h>
#include <goosepp/contentExtraction/NodeTextCleaner.h>
#include <goosepp/contentExtraction/BoostChecker.h>
#include <goosepp/contentExtraction/TextNodeCollector.h>
#include <goosepp/contentExtraction/NodeScorer.h>
#include <goosepp/GooseExtractor.h>
using namespace std;

namespace scivey {
namespace goosepp {

using namespace scivey::goosepp::contentExtraction;

string extractBody(const string &htmlText) {
    auto url = "http://www.nonsense.com";
    GooseExtractor extractor(url, htmlText);
    return extractor.getBody();
}

} // goosepp
} // scivey


