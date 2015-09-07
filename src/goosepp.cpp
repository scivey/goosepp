#include <string>
#include <memory>
#include <gumbo.h>
#include "contentExtraction/ContentExtractor.h"
#include "stopwords/stopwords.h"
#include "contentExtraction/NodeTextCleaner.h"
#include "contentExtraction/BoostChecker.h"
#include "contentExtraction/TextNodeCollector.h"
#include "contentExtraction/NodeScorer.h"
#include "GooseExtractor.h"
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


