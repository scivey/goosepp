#include <string>
#include <memory>
#include <gumbo.h>
#include "ContentExtractor.h"
#include "stopwords/stopwords.h"
#include "NodeTextCleaner.h"
#include "BoostChecker.h"
#include "TextNodeCollector.h"
#include "NodeScorer.h"

using namespace std;

namespace scivey {
namespace goosepp {

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


