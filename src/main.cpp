#include <iostream>
#include <string>
#include <vector>

#include <set>
#include <fstream>
#include <sstream>

#include <cassert>

#include <gumbo.h>

#include "util/fileUtils.h"
#include "GooseExtractor.h"

using namespace std;
using scivey::goosepp::util::readFile;
using scivey::goosepp::GooseExtractor;

void attempt() {
    auto html = readFile("./resources/additional_html/jezebel1.txt");
    GooseExtractor extractor("http://jezebel.com/something", html);
    GooseExtractor extractor2("http://jezebel.com/something", html);
    auto body = extractor.getBody();
    cout << endl << endl << body << endl;
    cout << endl << "\t[ " << extractor.getTitle() << " ]" << endl;
    cout << endl << "\t[ " << extractor.getPublishDate() << " ]" << endl;
}

int main(int argc, char **argv) {
    cout << endl << "start";
    attempt();
    cout << endl << "end.";
}
