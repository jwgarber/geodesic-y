#include "util.hpp"

// Split a string on the given delimiter, ignoring any empty strings produced
std::vector<std::string> split(const std::string& str, const char delim) {

    const auto size = str.size();

    std::vector<std::string> vec{};

    size_t pos = 0;
    while (pos < size) {
        size_t count = 0;

        // Iterate forward until we reach the end of the string or find the next delimiter
        while ((pos + count < size) && (str.at(pos + count) != delim)) {
            ++count;
        }

        if (count != 0) {
            // If count == 0, str.at(pos, count) will return an empty string
            vec.push_back(str.substr(pos, count));
        }

        // Iterate forward, skipping the delimiter
        pos = pos + count + 1;
    }

    return vec;
}
