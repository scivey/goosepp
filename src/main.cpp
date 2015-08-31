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
#include "goosepp.h"


using namespace std;
using scivey::goosepp::readFile;
using scivey::goosepp::extractBody;

void attempt() {
    auto html = readFile("./resources/additional_html/jezebel1.txt");
    auto body = extractBody(html);
    cout << endl << endl << body << endl;
}

int main(int argc, char **argv) {
    LOG(INFO) << "start";
    attempt();
    LOG(INFO) << "end";
}
