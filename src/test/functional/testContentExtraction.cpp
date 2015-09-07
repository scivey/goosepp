#include <string>
#include <vector>
#include <cassert>

#include <glog/logging.h>

#include "goosepp.h"
#include "util/fileUtils.h"

using namespace std;
using scivey::goosepp::util::readFile;
using scivey::goosepp::extractBody;

class StartAndEndTest {
    string filePath_;
    string expectedStart_;
    string expectedEnd_;

public:
    StartAndEndTest(const string &filePath,
        const string &expectedStart,
        const string &expectedEnd)
        :   filePath_(filePath),
            expectedStart_(expectedStart),
            expectedEnd_(expectedEnd) {}

    void run() {
        LOG(INFO) << "testing: " << filePath_;
        auto text = readFile(filePath_);
        auto extracted = extractBody(text);

        LOG(INFO) << "\texpected start is '" << expectedStart_ << "'";
        assert(extracted.find(expectedStart_) == 0);
        LOG(INFO) << "\t\t[ok]";

        LOG(INFO) << "\texpect end is '" << expectedEnd_ << "'";
        size_t endPos = extracted.find(expectedEnd_);
        size_t expectedPos = extracted.size() - expectedEnd_.size();
        LOG(INFO) << "\t\texpected ending string at index " << expectedPos << " [actual = " << endPos << "]";
        assert(expectedPos == endPos);
        LOG(INFO) << "\t\t[ok]";
    }
};

void test_Jezebel1() {
    auto fileName = "resources/additional_html/jezebel1.txt";
    LOG(INFO) << "testing: " << fileName;
    auto text = readFile(fileName);
    auto extracted = extractBody(text);
    string expectedStart {"Leave it to Taylor"};
    LOG(INFO) << "expected start: " << expectedStart;
    assert(extracted.find(expectedStart) == 0);
    LOG(INFO) << "[ok]";
}

int main(int argc, char **argv) {
    vector<StartAndEndTest> tests = {
        StartAndEndTest("resources/additional_html/jezebel1.txt", "Leave it to Taylor", "every one of us on stage with her.\n\n")
    };
    for (auto &test: tests) {
        test.run();
    }
}