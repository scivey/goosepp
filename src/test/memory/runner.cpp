#include <string>
#include <gumbo.h>
#include <stdlib.h>
#include "GooseExtractor.h"
#include "OpenGraphExtractor.h"
#include "util/LazyGumboParser.h"
#include "util/gooseUtil.h"

#include "tokenizer/WhitespaceTokenizer.h"

using namespace std;
using namespace scivey::goosepp;

int main() {
    {
        string html = "<html><body></body></html>";
        util::LazyGumboParser parser(&html);
        parser.get();
    }
    {
        tokenizer::WhitespaceTokenizer tokenizer;
        tokenizer.tokenize("one two three");
    }
    {
        string html = "<html><body></body></html>";
        util::LazyGumboParser parser(&html);
        OpenGraphExtractor extractor;
        extractor.extract(parser.getShared());
    }
    return 0;
}
