#pragma once

#include <string>
#include <gumbo.h>
#include <goosepp/GooseExtractor.h>

namespace scivey {
namespace goosepp {

std::string extractBody(const std::string &htmlText);

} // goosepp
} // scivey
