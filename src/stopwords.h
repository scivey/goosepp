#include <string>
#include <vector>

namespace scivey {
namespace goosepp {

size_t countStopwords(const std::vector<std::string> &tokens);
size_t countStopwords(const std::string &text);

} // goosepp
} // scivey