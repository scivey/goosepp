#pragma once

#include <vector>
#include <string>

namespace scivey {
namespace goosepp {
namespace tokenizer {

class Tokenizer {
public:
    virtual size_t getTokenCount(const std::string &text) = 0;
    virtual std::vector<std::string> tokenize(const std::string &text) = 0;
};

} // tokenizer
} // goosepp
} // scivey
