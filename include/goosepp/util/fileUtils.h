#pragma once

#include <string>

namespace scivey {
namespace goosepp {
namespace util {

std::string readFile(const char *filePath);
std::string readFile(const std::string &filePath);


} // util
} // goosepp
} // scivey