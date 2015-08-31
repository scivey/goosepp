#goosepp

A c++ port of [python-goose](https://github.com/grangier/python-goose), which is itself a port of the [origin Scala Goose project](https://github.com/GravityLabs/goose).

It has two dependencies:
* [MITIE](https://github.com/mit-nlp/MITIE) for tokenization of element text
* [gumbo-parser](https://github.com/google/gumbo-parser) for HTML parsing, DOM, etc.

```c++
#include <iostream>
#include <string>
#include <goosepp/goosepp.h>
using std::string;
using scivey::goosepp::extractBody;

string getHtmlSomehow();

int main() {
    auto rawHtml = getHtmlSomehow();
    auto body = extractBody(rawHtml);
    cout << endl << body << endl;
    // that's it
}

```
