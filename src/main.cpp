#include <iostream>
#include <string>
#include <vector>

#include <set>
#include <fstream>
#include <sstream>

#include <cassert>

#include <glog/logging.h>
#include <gumbo.h>

#include "fileUtils.h"
#include "gumboUtils.h"
#include "gooseFuncs.h"
#include "stopwords.h"
#include "stopwordSet.h"


using namespace std;
using scivey::goosepp::readFile;
using scivey::goosepp::findTitle;
using scivey::goosepp::findLinks;
using scivey::goosepp::findBody;
using scivey::goosepp::cleanText;
using scivey::goosepp::isStopword;
using scivey::goosepp::countStopwords;

using scivey::goosepp::collectTags;
using scivey::goosepp::extractBody;

void attempt() {
    auto html = readFile("./resources/additional_html/jezebel1.txt");
    auto body = extractBody(html);
    cout << endl << endl << body << endl;
    // auto gumboed = gumbo_parse(html.c_str());
    // LOG(INFO) << findTitle(gumboed->root);
    // auto body = findBody(gumboed->root);
    // LOG(INFO) << cleanText(body);
    // set<GumboTag> tags {GUMBO_TAG_P};
    // auto paragraphs = collectTags(tags, gumboed->root);
    // for(auto &elem: paragraphs) {
    //     LOG(INFO) << cleanText(elem);
    // }
    // auto links = findLinks(gumboed->root);
    // for (auto &elem: links) {
    //     LOG(INFO) << elem;
    // }
}

int main(int argc, char **argv) {
    LOG(INFO) << "start";
    attempt();
    LOG(INFO) << "end";
}
