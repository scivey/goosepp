#include <string>

#include <gumbo.h>

#include <goosepp/GooseExtractor.h>
#include <goosepp/contentExtraction/ContentExtractor.h>
#include <goosepp/stopwords/stopwords.h>
#include <goosepp/contentExtraction/NodeTextCleaner.h>
#include <goosepp/contentExtraction/BoostChecker.h>
#include <goosepp/contentExtraction/TextNodeCollector.h>
#include <goosepp/contentExtraction/NodeScorer.h>
#include <goosepp/tokenizer/Tokenizer.h>
#include <goosepp/tokenizer/WhitespaceTokenizer.h>
#include <goosepp/TitleExtractor.h>
#include <goosepp/OpenGraphExtractor.h>
#include <goosepp/PublishDateExtractor.h>
#include <goosepp/util/LazyGumboParser.h>
#include <goosepp/util/gooseUtil.h>

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
