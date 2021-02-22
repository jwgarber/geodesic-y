#include <fstream>
#include <sstream>
#include <iostream>

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

std::string read_file(const std::string& path) {

    try {
        std::ifstream file{};
        file.exceptions(std::ifstream::badbit | std::ifstream::failbit | std::ifstream::eofbit);

        file.open(path);

        std::stringstream buff{};
        buff << file.rdbuf();

        return buff.str();

    } catch (const std::runtime_error& err) {
        std::cout << "error: unable to read file " + path << std::endl;
        throw err;
    }
}

static bool is_space(const char c) {
    switch (c) {
        case ' ':
        case '\n':
        case '\r':
        case '\t':
        case '\v':
        case '\f':
            return true;
        default:
            return false;
    }
}

// Remove leading and trailing whitespace from a string
std::string trim_copy(std::string str) {

    auto it = str.begin();
    while ((it != str.end()) && is_space(*it)) {
        ++it;
    }

    str.erase(str.begin(), it);

    auto rit = str.rbegin();
    while ((rit != str.rend()) && is_space(*rit)) {
        ++rit;
    }

    str.erase(rit.base(), str.end());

    return str;
}

// Replace all occurences of 'search' with 'replace' in 'str' and return a new copy
std::string replace_copy(std::string str, const std::string& search, const std::string& replace) {

    size_t pos = str.find(search);

    while (pos != std::string::npos) {
        str.replace(pos, search.size(), replace);
        pos = str.find(search, pos + replace.size());
    }

    return str;
}
