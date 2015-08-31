#include "fileUtils.h"

#include <string>
#include <fstream>
#include <sstream>

using namespace std;

namespace scivey {
namespace goosepp {

string readFile(const char *filePath) {
    ostringstream oss;
    ifstream ifs {filePath};
    char c;
    while (ifs.good()) {
        ifs.get(c);
        oss << c;
    }
    return oss.str();
}

} // goosepp
} // scivey
