#include <memory>
#include "StopwordCounter.h"
#include "englishStopwordSet.h"
#include "stopwords.h"

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