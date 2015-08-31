#include "StopwordCounter.h"
#include "englishStopwordSet.h"
#include "stopwords.h"

namespace scivey {
namespace goosepp {
namespace stopwords {

StopwordCounter getEnglishStopwordCounter() {
    return StopwordCounter(isEnglishStopword);
}

} // stopwords
} // goosepp
} // scivey