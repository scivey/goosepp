#include <goosepp/util/fileUtils.h>

#include <string>
#include <fstream>
#include <sstream>

using namespace std;

namespace scivey {
namespace goosepp {
namespace util {

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
string readFile(const string &filePath) {
    return readFile(filePath.c_str());
}

} // util
} // goosepp
} // scivey