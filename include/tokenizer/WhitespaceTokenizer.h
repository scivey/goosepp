#pragma once
#include <vector>
#include <string>

#include "Tokenizer.h"

namespace scivey {
namespace goosepp {
namespace tokenizer {

class WhitespaceTokenizer: public Tokenizer {
public:
    std::vector<std::string> tokenize(const std::string &text) override;
    size_t getTokenCount(const std::string &text) override;
};

} // tokenizer
} // goosepp
} // scivey
