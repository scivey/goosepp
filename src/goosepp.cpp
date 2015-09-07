#include <string>
#include <memory>
#include <gumbo.h>
#include "contentExtraction/ContentExtractor.h"
#include "stopwords/stopwords.h"
#include "contentExtraction/NodeTextCleaner.h"
#include "contentExtraction/BoostChecker.h"
#include "contentExtraction/TextNodeCollector.h"
#include "contentExtraction/NodeScorer.h"

using namespace std;

namespace scivey {
namespace goosepp {

using namespace scivey::goosepp::contentExtraction;

string extractBody(const string &htmlText) {
    auto gumboed = gumbo_parse(htmlText.c_str());
    auto stopwordCounter = stopwords::getEnglishStopwordCounterPtr();
    shared_ptr<NodeTextCleanerIf> cleaner = make_shared<NodeTextCleaner>();
    shared_ptr<BoostCheckerFactoryIf> checkerFactory = make_shared<BoostCheckerFactory>(
        cleaner, stopwordCounter
    );
    shared_ptr<TextNodeCollectorIf> collector = make_shared<TextNodeCollector>();
    shared_ptr<NodeScorerIf> scorer = make_shared<NodeScorer>(
        stopwordCounter, collector, checkerFactory, gumboed->root
    );
    ContentExtractor extractor(
        scorer, cleaner, stopwordCounter, gumboed->root
    );
    std::string result = extractor.extract();
    // gumbo_destroy_output(gumboed);
    return result;
}

} // goosepp
} // scivey


