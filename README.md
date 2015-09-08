#goosepp

A c++ port of [python-goose](https://github.com/grangier/python-goose), which is itself a port of the [original Scala Goose project](https://github.com/GravityLabs/goose).

## Case study: Jezebel extraction
`goosepp` takes raw HTML like [this article about Taylor Swift](resources/additional_html/jezebel1.txt) and extracts the most "contenty" text.  It tries to remove navigation, headers, footers, ads, etc., and outputs the cleaned text of the main content like this:

```
Leave it to Taylor to tidy up a  squabble  quickly and, if possible, in the most maximalist way. Mere days after a social media-based tiff, misunderstanding, whatever you want to call it, Taylor invited Avril to the stage at her San Diego show. They performed “Complicated,” Taylor beamed and squeezed Avril — everything was beautiful again.
... [truncated]
So in case you had any further questions, Taylor Swift remains bosom friends with every living creature on the globe—with the exception of Katy Perry, for those wounds will never heal—and will one day invite every one of us on stage with her.
```

It also extracts titles:
```
Avril Lavigne And Taylor Swift Sing 'Complicated,' Are Besties Again
```
Title extraction uses meta tags if possible, falling back on the `<title>` element.  It then tries to remove common title noise, matching patterns like  `Article Title | www.something.com` and `Jezebel: More Taylor Swift News`.

## Code example
```c++
#include <iostream>
#include <string>
#include <goosepp/goosepp.h>
using std::string;
using std::cout;
using std::endl;
using scivey::goosepp::GooseExtractor;

string getHtmlSomehow(string url);

int main() {
    string url = "http://jezebel.com/articles/taylor-swift-9999";
    auto rawHtml = getHtmlSomehow(url);
    GooseExtractor extractor(url, rawHtml);
    cout << extractor.getBody() << endl;
    cout << extractor.getTitle() << endl;
    cout << extractor.getPublishDate() << endl;
    // that's it
}

```

## Dependencies
There are two:
* [MITIE](https://github.com/mit-nlp/MITIE) for tokenization of element text
* [gumbo-parser](https://github.com/google/gumbo-parser) for HTML parsing, DOM, etc.

## Building
Make sure you have the dependencies, then:
```bash
make deps
mkdir build
cd build
cmake ../
sudo make install
```

## Tests
There are lower-level unit tests, functional tests of content extraction, and a set of memory leak checks.
``` bash
make test-unit
make test-functional
make test-mem
```

## Benchmarks
These numbers aren't all that scientific, as the current benchmark is just run repeatedly against a [single example](resources/additional_html/jezebel1.txt).

```
    ./benchmark_runner
    Run on (4 X 3500 MHz CPU s
    2015-09-07 19:56:12
    Benchmark            Time(ns)    CPU(ns) Iterations
    ---------------------------------------------------
    extractJezebelBody    6342558    6334100        100
```

`goosepp` can extract the content of this article in about 0.0063 seconds.
The python implementation takes around 0.09 seconds for the same article.

To run the c++ benchmark yourself (requires [benchmark](https://github.com/google/benchmark)):

```bash
make benchmark
```

To run the same benchmark against `python-goose` (requires `python-goose` in your `PYTHONPATH`):
```bash
python scripts/benchmark_python_goose.py
```
