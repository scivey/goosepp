#include <memory>
#include <goosepp/stopwords/StopwordCounter.h>
#include <goosepp/stopwords/englishStopwordSet.h>
#include <goosepp/stopwords/stopwords.h>

using namespace std;

namespace scivey {
namespace goosepp {
namespace stopwords {

StopwordCounter getEnglishStopwordCounter() {
    return StopwordCounter(isEnglishStopword);
}

shared_ptr<StopwordCounter> getEnglishStopwordCounterPtr() {
    return make_shared<StopwordCounter>(isEnglishStopword);
}

} // stopwords
} // goosepp
} // scivey