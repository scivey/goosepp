#include <vector>
#include <string>
#include <sstream>
#include <mitie/conll_tokenizer.h>
#include "WhitespaceTokenizer.h"

using namespace std;
using mitie::conll_tokenizer;

namespace scivey {
namespace goosepp {

WhitespaceTokenizer::WhitespaceTokenizer(){}

void WhitespaceTokenizer::tokenize(const string &text, vector<string> &output) {
    istringstream textStream(text);
    conll_tokenizer tokenStream(textStream);
    vector<string> tokens;
    string token;
    while (tokenStream(token)) {
        output.push_back(token);
    }
}

vector<string> WhitespaceTokenizer::tokenize(const string &text) {
    vector<string> tokens;
    tokenize(text, tokens);
    return tokens;
}

size_t WhitespaceTokenizer::getTokenCount(const string &text) {
    size_t count = 0;
    istringstream textStream(text);
    conll_tokenizer tokenStream(textStream);
    string token;
    while (tokenStream(token)) {
        count++;
    }
    return count;
}

} // goosepp
} // scivey