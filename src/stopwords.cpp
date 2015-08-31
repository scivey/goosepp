#include <string>
#include <vector>
#include "stopwords.h"
#include "stopwordSet.h"
#include "WhitespaceTokenizer.h"

using namespace std;

namespace scivey {
namespace goosepp {

size_t countStopwords(const vector<string> &tokens) {
    size_t count = 0;
    for (auto &token: tokens) {
        if (isStopword(token)) {
            count++;
        }
    }
    return count;
}

size_t countStopwords(const string &text) {
    WhitespaceTokenizer tokenizer;
    vector<string> tokens = tokenizer.tokenize(text);
    return countStopwords(tokens);
}

} // goosepp
} // scivey
