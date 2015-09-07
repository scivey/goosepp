#include <string>
#include "GooseExtractor.h"

#include <gumbo.h>
#include "contentExtraction/ContentExtractor.h"
#include "stopwords/stopwords.h"
#include "contentExtraction/NodeTextCleaner.h"
#include "contentExtraction/BoostChecker.h"
#include "contentExtraction/TextNodeCollector.h"
#include "contentExtraction/NodeScorer.h"
#include "tokenizer/Tokenizer.h"
#include "tokenizer/WhitespaceTokenizer.h"
#include "TitleExtractor.h"
#include "OpenGraphExtractor.h"
#include "PublishDateExtractor.h"
#include "util/LazyGumboParser.h"
#include "util/gooseUtil.h"

using namespace std;
namespace scivey {
namespace goosepp {

using namespace scivey::goosepp::contentExtraction;

string GooseExtractor::getPublishDate() {
    PublishDateExtractor extractor;
    return extractor.extract(gumboParser_.getSharedRoot());
}

string GooseExtractor::getTitle() {
    tokenizer::WhitespaceTokenizer tokenizerImpl;
    shared_ptr<tokenizer::Tokenizer> tokenizerPtr(
        (tokenizer::Tokenizer*) &tokenizerImpl,
        util::NonDeleter<tokenizer::Tokenizer>()
    );
    OpenGraphExtractor openGraphExtractorImpl;
    shared_ptr<OpenGraphExtractorIf> openGraphExtractor(
        (OpenGraphExtractorIf*) &openGraphExtractorImpl,
        util::NonDeleter<OpenGraphExtractorIf>()
    );
    TitleExtractor titleExtractor(openGraphExtractor, tokenizerPtr);
    return titleExtractor.extract(gumboParser_.getShared(), url_);
}

string GooseExtractor::getBody() {
    auto stopwordCounter = stopwords::getEnglishStopwordCounterPtr();
    NodeTextCleaner cleanerImpl;
    shared_ptr<NodeTextCleanerIf> cleaner((NodeTextCleanerIf*) &cleanerImpl, util::NonDeleter<NodeTextCleanerIf>());

    BoostCheckerFactory checkerFactoryImpl(cleaner, stopwordCounter);
    shared_ptr<BoostCheckerFactoryIf> checkerFactory(
        (BoostCheckerFactoryIf*) &checkerFactoryImpl,
        util::NonDeleter<BoostCheckerFactoryIf>()
    );

    TextNodeCollector collectorImpl;
    shared_ptr<TextNodeCollectorIf> collector(
        (TextNodeCollectorIf*) &collectorImpl,
        util::NonDeleter<TextNodeCollectorIf>()
    );

    auto gumboDoc = gumboParser_.get();

    NodeScorer scorerImpl(
        stopwordCounter,
        collector,
        checkerFactory,
        gumboDoc->root
    );

    shared_ptr<NodeScorerIf> scorer(
        (NodeScorerIf*) &scorerImpl,
        util::NonDeleter<NodeScorerIf>()
    );

    ContentExtractor extractor(
        scorer, cleaner, stopwordCounter, gumboDoc->root
    );
    std::string result = extractor.extract();
    return result;
}

}
}
