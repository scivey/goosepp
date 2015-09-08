#pragma once

#include <memory>
#include <goosepp/stopwords/StopwordCounter.h>

namespace scivey {
namespace goosepp {
namespace stopwords {

StopwordCounter getEnglishStopwordCounter();
std::shared_ptr<StopwordCounter> getEnglishStopwordCounterPtr();

} // stopwords
} // goosepp
} // scivey