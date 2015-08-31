#pragma once
#include <string>
#include <vector>
#include <functional>

#include "tokenizer/WhitespaceTokenizer.h"

using namespace std;

namespace scivey {
namespace goosepp {
namespace stopwords {

class StopwordCounter {
protected:
    tokenizer::WhitespaceTokenizer tokenizer_;
    std::function<bool (const std::string&)> stopwordPredicate_;
public:
    StopwordCounter(function<bool (const std::string&)> predicate)
        : stopwordPredicate_(predicate) {}

    size_t countStopwords(const std::vector<std::string> &tokens) {
        size_t count = 0;
        for (auto &token: tokens) {
            if (stopwordPredicate_(token)) {
                count++;
            }
        }
        return count;
    }

    size_t countStopwords(const std::string &text) {
        std::vector<std::string> tokens = tokenizer_.tokenize(text);
        return countStopwords(tokens);
    }
};

} // stopwords
} // goosepp
} // scivey
