#pragma once

#include <string>
#include <sstream>
#include <gumbo.h>
#include <vector>
namespace scivey {
namespace goosepp {
namespace util {

bool isHighLinkDensity(const GumboNode *node, const std::string &nodeText);
bool isHighLinkDensity(const GumboNode *node);

template<typename T>
std::string joinVec(const std::string &joinWith, const std::vector<T> &vec) {
    std::ostringstream oss;
    size_t last = vec.size() - 1;
    for (size_t i = 0; i <= last; i++) {
        std::string current = vec.at(i);
        if (current.size()) {
            oss << vec.at(i);
            if (i != last) {
                oss << joinWith;
            }
        }
    }
    return oss.str();
}

// used to make phony shared_ptr<T> instances that don't call `free()`
template<typename T>
struct NonDeleter {
    void operator()(T* t) const {}
};


} // util
} // goosepp
} // scivey