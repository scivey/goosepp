#include <string>
#include <benchmark/benchmark.h>
#include "util/fileUtils.h"
#include "GooseExtractor.h"
using namespace std;
using namespace scivey::goosepp;

string jezebelHtml;

static void extractJezebelBody(benchmark::State &state) {
    GooseExtractor extractor("http://jezebel.com/something", jezebelHtml);
    while (state.KeepRunning()) {
        extractor.getBody();
    }
}
BENCHMARK(extractJezebelBody);

void load() {
    jezebelHtml = util::readFile("resources/additional_html/jezebel1.txt");
}

int main(int argc, const char **argv) {
    load();
    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();
}
