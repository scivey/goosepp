#pragma once
#include <vector>
#include <string>

namespace scivey {
namespace goosepp {

class WhitespaceTokenizer {
public:
    WhitespaceTokenizer();
    std::vector<std::string> tokenize(const std::string &text);
    void tokenize(const std::string &text, std::vector<std::string> &output);
    size_t getTokenCount(const std::string &text);
};

} // goosepp
} // scivey